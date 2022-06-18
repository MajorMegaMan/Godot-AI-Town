#ifndef GOAPAGENTS_H
#define GOAPAGENTS_H

#include "GOAPcore.h"

namespace GOAP
{
	class GOAPBehaviour
	{
		std::vector<GOAPAction*> m_actions;
		std::vector<GOAPGoal*> m_goals;
		GOAPWorldState m_baseAgentValues;
	protected:

		GOAPWorldState& GetBaseAgentValues()
		{
			return m_baseAgentValues;
		}

		void SetBaseAgentValues(const GOAPWorldState& baseAgentValues)
		{
			m_baseAgentValues = baseAgentValues;
		}

		std::vector<GOAPAction*>& GetActions()
		{
			return m_actions;
		}

		std::vector<GOAPGoal*>& GetGoals()
		{
			return m_goals;
		}

		void AddAction(GOAPAction* action)
		{
			m_actions.push_back(action);
		}

		void AddGoal(GOAPGoal* goal)
		{
			m_goals.push_back(goal);
		}

	public:
		GOAPAction* GetAction(int index)
		{
			return m_actions[index];
		}

		std::stack<GOAPAction*> GetPlanStack(const GOAPWorldState& worldState)
		{
			return GOAPPlanner::GetPlanStack(worldState, m_actions, *(FindGoal(worldState)));
		}

		virtual GOAPGoal* FindGoal(const GOAPWorldState& worldState) = 0;

		GOAPGoal* GetGoal(int index)
		{
			return m_goals[index];
		}

		void FillAgentStateValues(GOAPWorldState& agentState)
		{
			GOAPWorldState::FillValues(m_baseAgentValues, agentState);
		}
	};

	class GOAPAgent
	{
		GOAPWorldState* m_globalWorldState;
		GOAPBehaviour* m_behaviour;

	public:
		GOAPWorldState agentState;

	public:
		GOAPAgent()
		{
			m_globalWorldState = nullptr;
			m_behaviour = nullptr;
		}

		~GOAPAgent()
		{

		}

		GOAPAgent(const GOAPAgent& other)
		{
			m_globalWorldState = other.m_globalWorldState;
			m_behaviour = other.m_behaviour;
			agentState.CopyValues(other.agentState);
		}

		GOAPAgent& operator=(const GOAPAgent& other) = delete;

		void SetGlobalWorldState(GOAPWorldState* worldState)
		{
			m_globalWorldState = worldState;
		}

		GOAPWorldState* GetGlobalWorldState()
		{
			return m_globalWorldState;
		}

		GOAPWorldState GetAgentWorldStateCopy()
		{
			return agentState;
		}

		GOAPWorldState GetMixedWorldStateCopy()
		{
			GOAPWorldState mixed;
			FillMixedWorldState(mixed);
			return mixed;
		}

		void SetBehaviour(GOAPBehaviour* behaviour)
		{
			m_behaviour = behaviour;
			m_behaviour->FillAgentStateValues(agentState);
		}

		void ApplyActionEffects(GOAPAction* action)
		{
			// Perform the action in a copy of both states.
			GOAPWorldState mixed;
			FillMixedWorldState(mixed);
			action->PerformEffects(mixed);

			// Copy the values from the mixed state into the original worldstates.
			auto worldKeys = m_globalWorldState->GetKeys();
			for (auto key : worldKeys)
			{
				m_globalWorldState->CopyValue(key, mixed.GetConstValueRef(key));
			}

			auto agentKeys = agentState.GetKeys();
			for (auto key : agentKeys)
			{
				agentState.CopyValue(key, mixed.GetConstValueRef(key));
			}
		}

		void ApplyActionEffects(int actionIndex)
		{
			ApplyActionEffects(m_behaviour->GetAction(actionIndex));
		}

		std::stack<GOAPAction*> GetPlanStack()
		{
			GOAPWorldState mixed;
			FillMixedWorldState(mixed);
			return m_behaviour->GetPlanStack(mixed);
		}

		// Create a worldstate that contains both the global worldstate and the agent's world state.
		void FillMixedWorldState(GOAPWorldState& out)
		{
			// fill out with currents keys and values
			auto agentKeys = agentState.GetKeys();
			for (auto key : agentKeys)
			{
				if (out.CreateValueType(key, agentState.GetValueTypeSize(key)))
				{
					out.CopyValue(key, agentState.GetValueRef(key));
				}
			}

			// fill out with currents keys and values
			auto worldKeys = m_globalWorldState->GetKeys();
			for (auto key : worldKeys)
			{
				if (out.CreateValueType(key, m_globalWorldState->GetValueTypeSize(key)))
				{
					out.CopyValue(key, m_globalWorldState->GetValueRef(key));
				}
			}
		}
	};
}

#endif // -ends GOAPAGENTS_H