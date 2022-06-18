#ifndef NODESTARTER_CLASS_H
#define NODESTARTER_CLASS_H

#include "SingletonTemplate.h"

// This class is designed to delay when process functions should be called.
// It's a simple state machine that wil enable or disable process funcitons.
// Start needs to be called when the node enters the scene tree.
// Exit needs to be called when the node exits the tree.
// The user will need to call update in the relative process function, otherwise nothing will happen.
class NodeStarter
{
	// State Interface for statemachine use
	class State
	{
	public:
		virtual void Enter(NodeStarter* invoker) {}
		virtual void Exit(NodeStarter* invoker) {}
		virtual void Invoke(NodeStarter* invoker, double delta) {}
	};

	State* m_currentState;

	class UnStartedState : public State, public PublicSingleton<UnStartedState>
	{

	};

	class RunningState : public State, public PublicSingleton<RunningState>
	{
		void Enter(NodeStarter* invoker) override
		{
			invoker->OnStart();
		}

		virtual void Exit(NodeStarter* invoker) override
		{
			invoker->OnExit();
		}

		void Invoke(NodeStarter* invoker, double delta) override
		{
			invoker->OnUpdate(delta);
		}
	};

	void ChangeState(State* targetState)
	{
		m_currentState->Exit(this);
		m_currentState = targetState;
		m_currentState->Enter(this);
	}

public:
	NodeStarter()
	{
		m_currentState = &UnStartedState::GetInstance();
	}

	~NodeStarter()
	{
		if (m_currentState == &RunningState::GetInstance())
		{
			m_currentState->Exit(this);
		}
	}

	void Start() 
	{
		ChangeState(&RunningState::GetInstance());
	}

	void ProcessUpdate(double delta)
	{
		m_currentState->Invoke(this, delta);
	}

	void Exit()
	{
		ChangeState(&UnStartedState::GetInstance());
	}

	virtual void OnStart() = 0;
	virtual void OnUpdate(double delta) = 0;
	virtual void OnExit() = 0;
};

#endif // ! NODESTARTER_CLASS_H