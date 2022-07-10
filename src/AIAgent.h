#ifndef AIAGENT_CLASS_H
#define AIAGENT_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/animation_tree.hpp>

#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "AnimationTreeGetter.h"

#include "DEBUGAIHeader.h"
#include "NodeStarter.h"
#include "ActionTimer.h"
#include "AnimationTrigger.h"
#include "AgentWorldStateRefs.h"
#include "BehaviourResource.h"
#include "ActionResource.h"

using namespace GOAP;
using namespace godot;

class AIAgent : public CharacterBody3D, public GOAPAIAgent<Vector3>
{
	GDCLASS(AIAgent, CharacterBody3D);

protected:
	static void _bind_methods();

private:
	// Node Path properties
	NodePathProperty<NavigationAgent3D> m_navAgentProp;
	NodePathProperty<AnimationTreeGetter> m_animTreeHolderProp;

	// Animation State machine
	AnimationNodeStateMachinePlayback* m_animStatePlayback;

	// behaviour resource
	Ref<BehaviourResource> m_behaviourResource;

	// Agent stats
	float m_movementSpeed = 4.0f;

	// Internal Logic
	ActionTimer m_actionTimer;

	// Internal function pointers used as a sort of state machine
	typedef void (AIAgent::*UpdateFunc)(double delta);
	UpdateFunc processFunc;
	UpdateFunc physicsFunc;

	// Self world State refs used for getters
	AgentWorldStateRefs m_worldStateRefs;

public:
	AIAgent();
	~AIAgent();

	// Functions.
	void on_velocity_computed(Vector3 safeVelocity);
	void on_target_reached();
	void MoveTo(Vector3 location);

	// Property.
	NavigationAgent3D* GetNavAgent() const;

	void set_navigation_node_path(const NodePath& navAgentNodePath);
	NodePath get_navigation_node_path() const;

	void set_move_speed(float moveSpeed);
	float get_move_speed() const;

	AnimationTree* GetAnimTree() const;

	void set_anim_tree_getter_path(const NodePath& animTreeNodePath);
	NodePath get_anim_tree_getter_path() const;

	void set_behaviour_resource(const Ref<BehaviourResource>& behaviourResource);
	Ref<BehaviourResource> get_behaviour_resource();

	// Node Starter interfaces. These are here becuase godot needs to connect bindings from this class in order to
	void Start();
	void Exit();

	void Empty(double delta);

	// Godot overrides
	void Update(double delta);
	void PhysicsUpdate(double delta);

private:
	// internals
	void SetUpNavigationAgent();
	void SetUpGOAPAgent();

	// Nav Agent
	// returns true when the agent has reached it's target
	void NavAgentUpdate(double delta);

	// Goap Process
	void PlanUpdate(double delta);
	void ActionUpdate(double delta);
	void MoveUpdate(double delta);
	void MoveArrive();

	// GOAPAgentOverrides
	Vector3 GetActionTargetPosition() const override;

	void ProcessMove(Vector3 actionLocation) override;

	void OnMoveBegin(Vector3 actionLocation) override;

	void OnMoveStop() override;

	void OnActionBegin(GOAPAIAction<Vector3>* action, GOAPAIActionTarget<Vector3>* actionTarget) override;

	void OnActionStop(GOAPAIAction<Vector3>* action, GOAPAIActionTarget<Vector3>* actionTarget) override;

	// WorldStateGetters
	int GetWorldStateInt(int key)
	{
		return m_worldStateRefs.GetInt(key);
	}

	bool GetWorldStateBool(int key)
	{
		return m_worldStateRefs.GetBool(key);
	}

	float GetWorldStateFloat(int key)
	{
		return m_worldStateRefs.GetFloat(key);
	}

	String GetKeyName(int key)
	{
		switch (key)
		{
		case WorldKeys::WS_GLOBAL_FOODCOUNT:
		{
			return "WS_GLOBAL_FOODCOUNT";
		}
		case WorldKeys::WS_AGENT_HUNGER:
		{
			return "WS_AGENT_HUNGER";
		}
		case WorldKeys::WS_AGENT_HOLDITEM:
		{
			return "WS_AGENT_HOLDITEM";
		}
		default:
			return "DEFAULT";
		}
	}

	// DEBUG calls
	void DEBUGFindPlan()
	{
		//FindPlan();

		// Find Plan
		std::stack<GOAPAction*> resultPlan;
		GOAPWorldState mixed;
		FillMixedWorldState(mixed);
		//resultPlan = GetBehaviour()->GetPlanStack(mixed);

		// Get Plan Stack
		auto goalPtr = GetBehaviour()->FindGoal(mixed);

		if (goalPtr == nullptr)
		{
			UtilityFunctions::print("FAILED:: Goal is not valid.");
			resultPlan = std::stack<GOAPAction*>();
		}
		else
		{
			if (mixed.Contains(WorldKeys::WS_GLOBAL_FOODCOUNT))
			{
				UtilityFunctions::print("WS contains Food Count.");
			}
			else
			{
				UtilityFunctions::print("WS missing Food Count.");
			}

			if (mixed.Contains(WorldKeys::WS_AGENT_HUNGER))
			{
				UtilityFunctions::print("WS contains hunger.");
			}
			else
			{
				UtilityFunctions::print("WS missing hunger.");
			}

			if (mixed.Contains(WorldKeys::WS_AGENT_HOLDITEM))
			{
				UtilityFunctions::print("WS contains holditem.");
			}
			else
			{
				UtilityFunctions::print("WS missing holditem.");
			}

			UtilityFunctions::print("Goal Should be valid.");
			UtilityFunctions::print("Goal Keys.");
			auto goalKeys = goalPtr->GetKeys();
			for (int i = 0; i < goalKeys.size(); i++)
			{
				UtilityFunctions::print(GetKeyName(goalKeys[i]));
			}

			auto actions = &(GetBehaviour()->GetActions());
			if (actions->empty())
			{
				UtilityFunctions::print("No Actions Present.");
			}
			else
			{
				UtilityFunctions::print("No. of Actions: ");
				UtilityFunctions::print(actions->size());
				
				// This is temporary
				GOAPWorldState copy;
				copy.CopyValues(mixed);
				for (int i = 0; i < actions->size(); i++)
				{
					ActionResource* actResource = (ActionResource*)((*actions)[i]);
					UtilityFunctions::print(actResource->GetName());
					auto actionKeys = actResource->GetKeys();
					for (int i = 0; i < actionKeys.size(); i++)
					{
						UtilityFunctions::print(GetKeyName(actionKeys[i]));
					}

					if (actResource->CheckConditions(copy))
					{
						UtilityFunctions::print("-Conditions met");
						actResource->PerformEffects(copy);
					}
				}
			}
			return;
			UtilityFunctions::print("Starting Plan.");
			//resultPlan = GOAPPlanner::GetPlanStack(mixed, GetBehaviour()->GetActions(), *goalPtr);

			
			//GOAPNode* goalNode = BuildTree(worldState, m_actionList, goalPtr);
			
			std::vector<GOAPAction*> m_actionList = GetBehaviour()->GetActions();

			std::vector<GOAPNode> nodeTree;
			std::vector<int> goalIndexList;

			auto _nodeTree = &nodeTree;
			auto _goalIndexList = &goalIndexList;

			// start
			_nodeTree->clear();
			_goalIndexList->clear();

			// create initial node with worldState that contains at least the goal keys
			GOAPWorldState initialState;
			GOAPWorldState::FillValues(goalPtr->GetKeys(), mixed, initialState);
			GOAPNode initial(initialState);
			_nodeTree->push_back(initial);

			size_t currentNodeIndex = 0;
			size_t minDepth = 4294967295; // uINT_MAX

			size_t minDepthNodeIndex = 0;

			// loop all nodes in the current nodeTree
			while (currentNodeIndex != _nodeTree->size())
			{
				for (GOAPAction* action : m_actionList)
				{
					GOAPNode node(currentNodeIndex, (*_nodeTree)[currentNodeIndex].GetDepth() + 1, action);
					if (node.GetDepth() <= minDepth && node.GetDepth() <= MAX_DEPTH)
					{
						// process node
						if (node.ProcessAction(*_nodeTree, mixed))
						{
							// conditions were met and effects are applied. add this node as a new leaf
							_nodeTree->push_back(node);

							// check if the node is results in the goal conditions
							if (node.MeetsGoal(*goalPtr, mixed))
							{
								size_t goalIndex = _nodeTree->size() - 1;
								_goalIndexList->push_back(goalIndex);
								size_t depth = node.GetDepth();
								if (depth < minDepth)
								{
									minDepthNodeIndex = goalIndex;
									minDepth = node.GetDepth();
								}
							}
						}
					}
				}

				currentNodeIndex++;
			}

			return;
			UtilityFunctions::print("Creating Plan Stack.");
			GOAPNode* goalNode = &(*_nodeTree)[minDepthNodeIndex];

			// Build stack from Tree
			std::stack<GOAPAction*> actionStack;
			if (goalNode != nullptr)
			{
				GOAPNode* currentNode = goalNode;
				while (currentNode->GetDepth() != 0)
				{
					actionStack.push(currentNode->GetAction());
					currentNode = &(nodeTree[currentNode->GetParentIndex()]);
				}
			}
			resultPlan = actionStack;
		}

		m_currentPlan = resultPlan;
	}

	void DEBUGPopNext()
	{
		if (m_currentPlan.empty())
		{
			UtilityFunctions::print("Plan is empty.");
			return;
		}

		PopActionFromPlan();
		if (m_actionTarget != nullptr)
		{
			UtilityFunctions::print("Popped Next Action.");
		}
		else
		{
			UtilityFunctions::print("Failed to find Action Target.");
		}
	}

	void DEBUGTestCurrentActionRange()
	{
		if (m_actionTarget != nullptr)
		{
			if (IsInActionRange())
			{
				UtilityFunctions::print("In Action Range.");
			}
			else
			{
				UtilityFunctions::print("Not In Action Range.");
			}
		}
		else
		{
			UtilityFunctions::print("Failed to find Action Target.");
		}
	}
};

#endif // ! AIAGENT_CLASS_H