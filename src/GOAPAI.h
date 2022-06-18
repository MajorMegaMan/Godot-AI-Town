#ifndef GOAPAI_H
#define GOAPAI_H
#include "GOAPagents.h"

#ifndef STATEMACHINE_H
#define STATEMACHINE_H
// State Interface for statemachine use
template <class T>
class State
{
public:
	virtual void Enter(T* invoker) {}
	virtual void Exit(T* invoker) {}
	virtual void Invoke(T* invoker) {}
};

// State machine with an owner reference.
template <class T>
class StateMachine
{
	T* m_owner;
	State<T>* m_currentState;

public:
	StateMachine(T* owner)
	{
		m_owner = owner;
		m_currentState = nullptr;
	}

	StateMachine(T* owner, State<T>* initialState)
	{
		m_owner = owner;
		m_currentState = initialState;
		m_currentState->Enter(m_owner);
	}

	~StateMachine() {}

	void Init(State<T>* initialState)
	{
		if (m_currentState != nullptr)
		{
			m_currentState->Exit(m_owner);
		}
		m_currentState = initialState;
		m_currentState->Enter(m_owner);
	}

	void Invoke()
	{
		m_currentState->Invoke(m_owner);
	}

	void ChangeState(State<T>* targetState)
	{
		m_currentState->Exit(m_owner);
		m_currentState = targetState;
		m_currentState->Enter(m_owner);
	}
};
#endif // -ends STATEMACHINE_H

namespace GOAP
{
	template <class TPosition>
	class GOAPAIAgent;

	// This is an interface class used by AIAgents to determine the position of the action they want to perform
	template <class TPosition>
	class GOAPAIActionTarget
	{
	public:
		virtual TPosition GetActionTargetPosition() const = 0;
	};

	// This is an extension of GOAPAction to include position and movement based Logic used by AIAgents
	template <class TPosition>
	class GOAPAIAction : public GOAPAction
	{
		bool m_dynamicMovement = true;

	public:
		bool IsDynamicMovement()
		{
			return m_dynamicMovement;
		}

		virtual GOAPAIActionTarget<TPosition>* FindActionTarget(const GOAPAIAgent<TPosition>& agent) = 0;

		virtual bool IsInRange(const GOAPAIAgent<TPosition>& agent, const GOAPAIActionTarget<TPosition>* actionTarget) const = 0;
	};

	// This is an extension of GOAPBehaviour to include position and movement based Logic used by AIAgents.
	// Specifcally it does not allow using Base GOAPActions. Only AIActions are allowed.
	template <class TPosition>
	class GOAPAIBehaviour : GOAPBehaviour
	{
	protected:
		GOAPWorldState& GetBaseAgentValues()
		{
			return GOAPBehaviour::GetBaseAgentValues();
		}

		void SetBaseAgentValues(const GOAPWorldState& baseAgentValues)
		{
			GOAPBehaviour::SetBaseAgentValues(baseAgentValues);
		}

		std::vector<GOAPAction*>& GetActions()
		{
			return GOAPBehaviour::GetActions();
		}

		std::vector<GOAPGoal*>& GetGoals()
		{
			return GOAPBehaviour::GetGoals();
		}

		void AddAction(GOAPAIAction<TPosition>* action)
		{
			GOAPBehaviour::AddAction(action);
		}

		void AddGoal(GOAPGoal* goal)
		{
			GOAPBehaviour::AddGoal(goal);
		}

	public:
		GOAPAction* GetAction(int index)
		{
			return GOAPBehaviour::GetAction(index);
		}

		std::stack<GOAPAction*> GetPlanStack(const GOAPWorldState& worldState)
		{
			return GOAPBehaviour::GetPlanStack(worldState);
		}

		GOAPGoal* GetGoal(int index)
		{
			return GOAPBehaviour::GetGoal(index);
		}

		void FillAgentStateValues(GOAPWorldState& agentState)
		{
			GOAPBehaviour::FillAgentStateValues(agentState);
		}
	};

	// Singleton states used by AIAgents to determine when the agent is moving or performing an action.
	template <class TInheritedClass, class TPosition>
	class ActionSingletonState : public State<GOAPAIAgent<TPosition>>
	{
	public:
		static TInheritedClass& GetInstance()
		{
			static TInheritedClass instance;
			return instance;
		}
	};

	template <class TPosition>
	class GOAPAIMoveTarget : public GOAPAIActionTarget<TPosition>
	{
		TPosition m_moveLocation;

	public:
		TPosition GetActionTargetPosition() const override
		{
			return m_moveLocation;
		}

		void SetActionTargetPosition(TPosition pos)
		{
			m_moveLocation = pos;
		}
	};

	// This is a GOAPAgent that runs it's own logic With every Update call.
	template <class TPosition>
	class GOAPAIAgent : GOAPAgent, public GOAPAIActionTarget<TPosition>
	{
		std::stack<GOAPAction*> m_currentPlan;
		GOAPAIAction<TPosition>* m_currentAction = nullptr;

		GOAPAIActionTarget<TPosition>* m_actionTarget = nullptr;
		GOAPAIActionTarget<TPosition>* m_moveTarget = nullptr;

		// This is the container for the move location if the action is not a dynamic move type.
		GOAPAIMoveTarget<TPosition> m_moveTargetHolder;

		StateMachine<GOAPAIAgent<TPosition>> m_stateMachine;


		class GOAPAIAgentActionState : public ActionSingletonState<GOAPAIAgentActionState, TPosition>
		{
			void Enter(GOAPAIAgent<TPosition>* agent) override
			{
				agent->OnActionBegin(agent->m_actionTarget);
			}

			void Exit(GOAPAIAgent<TPosition>* agent) override
			{
				agent->OnActionStop(agent->m_actionTarget);
			}

			void Invoke(GOAPAIAgent<TPosition>* agent) override
			{
				if (!agent->IsInActionRange())
				{
					agent->ActionInterrupt();
				}
				else
				{
					agent->ProcessCurrentAction();
				}
			}
		};

		class GOAPAIAgentMoveState : public ActionSingletonState<GOAPAIAgentMoveState, TPosition>
		{
			void Enter(GOAPAIAgent<TPosition>* agent) override
			{
				agent->OnMoveBegin(agent->m_moveTarget->GetActionTargetPosition());
			}

			void Exit(GOAPAIAgent<TPosition>* agent) override
			{
				agent->OnMoveStop();
			}

			void Invoke(GOAPAIAgent<TPosition>* agent) override
			{
				agent->ProcessMove(agent->m_moveTarget->GetActionTargetPosition());
				if (agent->IsInActionRange())
				{
					agent->MoveArrive();
					agent->ActionBegin();
				}
			}
		};

		class GOAPAIAgentEmptyState : public ActionSingletonState<GOAPAIAgentEmptyState, TPosition>
		{
			void Enter(GOAPAIAgent<TPosition>* agent) override
			{

			}

			void Exit(GOAPAIAgent<TPosition>* agent) override
			{
				if (agent->m_currentPlan.empty())
				{
					agent->ProcessNextAction();
				}
			}

			void Invoke(GOAPAIAgent<TPosition>* agent) override
			{
				agent->ProcessNextAction();
			}
		};

	public:
		GOAPAIAgent() : m_stateMachine(this, &GOAPAIAgentEmptyState::GetInstance())
		{

		}

		void SetWorldState(GOAPWorldState* worldState)
		{
			GOAPAgent::SetGlobalWorldState(worldState);
		}

		void SetBehaviour(GOAPAIBehaviour<TPosition>* behaviour)
		{
			GOAPAgent::SetBehaviour((GOAPBehaviour*)behaviour);
		}

		std::stack<GOAPAction*> GetCurrentPlan()
		{
			return m_currentPlan;
		}

		GOAPAIAction<TPosition>* GetCurrentAction() const
		{
			return m_currentAction;
		}

		GOAPWorldState GetAgentWorldStateCopy()
		{
			return GOAPAgent::GetAgentWorldStateCopy();
		}

		GOAPWorldState GetMixedWorldStateCopy()
		{
			return GOAPAgent::GetMixedWorldStateCopy();
		}

		TPosition GetMoveLocation()
		{
			return m_moveTarget->GetActionTargetPosition();
		}

		void Update()
		{
			m_stateMachine.Invoke();
		}

		bool IsInActionRange() const
		{
			return m_currentAction->IsInRange(*this, m_actionTarget);
		}

	private:
		void FindPlan()
		{
			m_currentPlan = GetPlanStack();
		}

		void PopActionFromPlan()
		{
			m_currentAction = (GOAPAIAction<TPosition>*)m_currentPlan.top();
			m_currentPlan.pop();
			FindActionTarget();
		}

		void FindActionTarget()
		{
			m_actionTarget = m_currentAction->FindActionTarget(*this);
			if (m_currentAction->IsDynamicMovement())
			{
				m_moveTarget = m_actionTarget;
			}
			else
			{
				m_moveTargetHolder.SetActionTargetPosition(m_actionTarget->GetActionTargetPosition());
				m_moveTarget = &m_moveTargetHolder;
			}
		}

		void MoveBegin()
		{
			// move to action location
			m_stateMachine.ChangeState(&GOAPAIAgentMoveState::GetInstance());
		}

		void MoveArrive()
		{
			// finished moving to action location
		}

		void ProcessCurrentAction()
		{
			// Wait for Action Complete or Action End to be called

			// DEBUG: for now we'll just instantly call Action Complete
			ApplyCurrentActionEffects();
			ActionComplete();
		}

		// Actions will begin during an update step. User does not need to explictly call an action to begin
		void ActionBegin()
		{
			// start action
			// eg. animation starts playing
			m_stateMachine.ChangeState(&GOAPAIAgentActionState::GetInstance());
		}

		void ProcessNextAction()
		{
			if (m_currentPlan.empty())
			{
				FindPlan();
				if (m_currentPlan.empty())
				{
					// if the plan is still empty exit early without doing anything.
					return;
				}
			}

			PopActionFromPlan();
			if (IsInActionRange())
			{
				ActionBegin();
			}
			else
			{
				MoveBegin();
			}
		}

		void ActionEnd()
		{
			m_actionTarget = nullptr;
			ProcessNextAction();
		}

	public:
		// Should be called sometime after an action has begun but before an action has completed or interrupted.
		void ApplyCurrentActionEffects()
		{
			GOAPAgent::ApplyActionEffects(m_currentAction);
		}

		// Should be called after an action has begun and the action has fully resolved.
		// eg. Animation for chopping wood has finished playing and the wood is now a world resource.
		void ActionComplete()
		{
			// action ended successfully
			ActionEnd();
		}

		// Should be called after an action has begun and the action did not fully resolve.
		// eg. Agent was knocked away from a tree while trying to chop it. Wood may have or may not have become a world resource.
		void ActionInterrupt()
		{
			// action ended early
			// will reset plan.
			// should be safe to call before or after action effects have been applied.
			FindPlan();
			ActionEnd();
		}

		// overrides
	protected:
		virtual void ProcessMove(TPosition actionLocation) = 0;

		virtual void OnMoveBegin(TPosition actionLocation) = 0;

		virtual void OnMoveStop() = 0;

		virtual void OnActionBegin(GOAPAIActionTarget<TPosition>* actionTarget) = 0;

		virtual void OnActionStop(GOAPAIActionTarget<TPosition>* actionTarget) = 0;
	};
}

#endif // -ends GOAPAI_H