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

using namespace GOAP;
using namespace godot;

class AIAgent : public CharacterBody3D, public GOAPAIAgent<Vector3>, public NodeStarter
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

	// Node Starter interfaces. These are here becuase godot needs to connect bindings from this class in order to
	void Start()
	{
		NodeStarter::Start();
	}

	void Exit()
	{
		NodeStarter::Exit();
	}

	void OnStart() override;
	void OnUpdate(double delta) override;
	void OnExit() override;

	void Empty(double delta);

	// Godot overrides
	void _process(double delta) override;
	void _physics_process(double delta) override;

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
};

#endif // ! AIAGENT_CLASS_H