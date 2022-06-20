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

#include <godot_cpp/core/binder_common.hpp>

#include "AnimationTreeGetter.h"

#include "DEBUGAIHeader.h"
#include "NodeStarter.h"

using namespace GOAP;
using namespace godot;

class AIAgent : public CharacterBody3D, public GOAPAIAgent<Vector3>, public NodeStarter
{
	GDCLASS(AIAgent, CharacterBody3D);

protected:
	static void _bind_methods();

private:
	NodePathProperty<NavigationAgent3D> m_navAgentProp;
	
	NodePathProperty<AnimationTreeGetter> m_animTreeHolderProp;
	//NodePath m_animTreeHolderPath; // This is the node that has an animation tree as a child.
	//AnimationTree* m_animTree;

	float m_movementSpeed = 4.0f;

	typedef void (AIAgent::*UpdateFunc)(double delta);
	UpdateFunc processFunc;
	UpdateFunc physicsFunc;

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
	void MoveUpdate(double delta);
	void MoveArrive();

	// GOAPAgentOverrides
	Vector3 GetActionTargetPosition() const override;

	void ProcessMove(Vector3 actionLocation) override;

	void OnMoveBegin(Vector3 actionLocation) override;

	void OnMoveStop() override;

	void OnActionBegin(GOAPAIActionTarget<Vector3>* actionTarget) override;

	void OnActionStop(GOAPAIActionTarget<Vector3>* actionTarget) override;
};

#endif // ! AIAGENT_CLASS_H

/*
extends CharacterBody3D

@export var target : NodePath

@export var m_movementSpeed : float = 4.0
@onready var m_navAgent : NavigationAgent3D = $NavigationAgent3D

# Called when the node enters the scene tree for the first time.
func _ready():
	var agent_rid : RID = m_navAgent.get_rid()

	var map_rid : RID = m_navAgent.get_tree().get_root().get_world_3d().get_navigation_map()
	NavigationServer3D.agent_set_map(agent_rid, map_rid)
	NavigationServer3D.agent_set_callback(agent_rid, m_navAgent, "_avoidance_done")

	SetTargetLocation(get_node(target).global_transform.origin)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func _physics_process(_delta):
	if m_navAgent.is_navigation_finished():
		return
	var nextPos : Vector3 = m_navAgent.get_next_location()
	var currentAgentPos : Vector3 = global_transform.origin
	var newVelocity : Vector3 = (nextPos - currentAgentPos).normalized() * m_movementSpeed
	m_navAgent.set_velocity(newVelocity)

func _on_navigation_agent_3d_velocity_computed(safe_velocity):
	velocity = safe_velocity
	move_and_slide()

func _on_navigation_agent_3d_target_reached():
	velocity = Vector3.ZERO

func SetTargetLocation(position : Vector3):
	m_navAgent.set_target_location(position)


func _on_mouse_ray_click_event(worldPos):
	SetTargetLocation(worldPos)
*/