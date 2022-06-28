#include "AIAgent.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/world3D.hpp>
#include <godot_cpp/classes/navigation_server3d.hpp>

using namespace godot;

void AIAgent::_bind_methods() {
	// Methods.
	ClassDB::bind_method(D_METHOD("Start"), &AIAgent::Start);
	ClassDB::bind_method(D_METHOD("Exit"), &AIAgent::Exit);
	ClassDB::bind_method(D_METHOD("on_velocity_computed"), &AIAgent::on_velocity_computed);
	ClassDB::bind_method(D_METHOD("on_target_reached"), &AIAgent::on_target_reached);

	// Properties.
	ClassDB::bind_method(D_METHOD("get_navigation_node_path"), &AIAgent::get_navigation_node_path);
	ClassDB::bind_method(D_METHOD("set_navigation_node_path", "navNodePath"), &AIAgent::set_navigation_node_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "navigation_node_path"), "set_navigation_node_path", "get_navigation_node_path");

	ClassDB::bind_method(D_METHOD("get_anim_tree_getter_path"), &AIAgent::get_anim_tree_getter_path);
	ClassDB::bind_method(D_METHOD("set_anim_tree_getter_path", "animTreeGetterPath"), &AIAgent::set_anim_tree_getter_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "anim_tree_getter_path"), "set_anim_tree_getter_path", "get_anim_tree_getter_path");

	ClassDB::bind_method(D_METHOD("get_move_speed"), &AIAgent::get_move_speed);
	ClassDB::bind_method(D_METHOD("set_move_speed", "moveSpeed"), &AIAgent::set_move_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "movementSpeed"), "set_move_speed", "get_move_speed");


	ClassDB::bind_method(D_METHOD("GetWorldStateInt", "key"), &AIAgent::GetWorldStateInt);
	ClassDB::bind_method(D_METHOD("GetWorldStateBool", "key"), &AIAgent::GetWorldStateBool);
	ClassDB::bind_method(D_METHOD("GetWorldStateFloat", "key"), &AIAgent::GetWorldStateFloat);
}

AIAgent::AIAgent()
{
	processFunc = &AIAgent::Empty;
	physicsFunc = &AIAgent::Empty;
}

AIAgent::~AIAgent()
{
	
}

// Methods.
void AIAgent::on_velocity_computed(Vector3 safeVelocity)
{
	set_velocity(safeVelocity);
	look_at(get_global_transform().get_origin() + safeVelocity);

	GetAnimTree()->set("parameters/MovementBlend/Forward/blend_amount", safeVelocity.length() / m_movementSpeed);
	move_and_slide();
}

void AIAgent::on_target_reached()
{
	MoveArrive();
}

void AIAgent::MoveTo(Vector3 location)
{
	GetNavAgent()->set_target_location(location);
}

// Properties.
NavigationAgent3D* AIAgent::GetNavAgent() const
{
	return m_navAgentProp.GetNode();
}

void AIAgent::set_navigation_node_path(const NodePath& navAgentNodePath)
{
	m_navAgentProp.SetNodePath(navAgentNodePath);
}

NodePath AIAgent::get_navigation_node_path() const
{
	return m_navAgentProp.GetNodePath();
}

void AIAgent::set_move_speed(float moveSpeed)
{
	m_movementSpeed = moveSpeed;
}

float AIAgent::get_move_speed() const
{
	return m_movementSpeed;
}

AnimationTree* AIAgent::GetAnimTree() const
{
	return m_animTreeHolderProp.GetNode()->GetAnimationTree();
}

void AIAgent::set_anim_tree_getter_path(const NodePath& animTreeNodePath)
{
	m_animTreeHolderProp.SetNodePath(animTreeNodePath);
}

NodePath AIAgent::get_anim_tree_getter_path() const
{
	return m_animTreeHolderProp.GetNodePath();
}

void AIAgent::OnStart()
{
	SetUpNavigationAgent();
	SetUpGOAPAgent();
}

void AIAgent::OnUpdate(double delta)
{
	(this->*physicsFunc)(delta);
}

void AIAgent::OnExit()
{

}

void AIAgent::Empty(double delta)
{

}

void AIAgent::_process(double delta)
{
	(this->*processFunc)(delta);
}

void AIAgent::_physics_process(double delta)
{
	NodeStarter::ProcessUpdate(delta);
}

void AIAgent::SetUpNavigationAgent()
{
	m_navAgentProp.ApplyPath(*this);

	// Set up navigation agent and map
	RID agent_rid = GetNavAgent()->get_rid();
	RID map_rid = GetNavAgent()->get_tree()->get_root()->get_world_3d()->get_navigation_map();

	auto navigationServer = NavigationServer3D::get_singleton();
	navigationServer->agent_set_map(agent_rid, map_rid);
	navigationServer->agent_set_callback(agent_rid, GetNavAgent(), "_avoidance_done");

	// connect siganls
	GetNavAgent()->connect("velocity_computed", Callable(this, "on_velocity_computed"));
	GetNavAgent()->connect("target_reached", Callable(this, "on_target_reached"));

	// Set inital target location to self
	//MoveTo(get_global_transform().get_origin());
}

void AIAgent::SetUpGOAPAgent()
{
	UtilityFunctions::print("Begin set up GOAPAgent.");
	SetWorldState(AIWorld::GetWorldState());
	SetBehaviour(BehaviourFoodGatherer::GetInstance());
	processFunc = &AIAgent::PlanUpdate;

	//m_animTreeHolderProp.ApplyPath(*this);
	AnimationTree* animTree = nullptr;
	m_animTreeHolderProp.ApplyPath(*this);
	animTree = GetAnimTree();
	if (animTree != nullptr)
	{
		UtilityFunctions::print("Yay! Found the animTree.");
		m_animStatePlayback = Object::cast_to<AnimationNodeStateMachinePlayback>(GetAnimTree()->get("parameters/playback"));
	}
	else
	{
		UtilityFunctions::print("boo! did not find animTree.");
	}

	// Add worldStateRefs
	m_worldStateRefs.AddWorldState(*AIWorld::GetWorldState());
	m_worldStateRefs.AddWorldState(GetAgentWorldState());
}

void AIAgent::NavAgentUpdate(double delta)
{
	auto navAgent = GetNavAgent();
	if (navAgent->is_navigation_finished())
	{
		return;
	}

	Vector3 nextPos = navAgent->get_next_location();
	Vector3 currentAgentPos = get_global_transform().get_origin();
	Vector3 newVelocity = (nextPos - currentAgentPos).normalized() * m_movementSpeed;
	navAgent->set_velocity(newVelocity);
}

void AIAgent::PlanUpdate(double delta)
{
	GOAPAIAgent::Update(delta);
	GetAnimTree()->advance(delta);
}

void AIAgent::ActionUpdate(double delta)
{
	PlanUpdate(delta);
	m_actionTimer.Tick(delta);
	if (m_actionTimer.Evaluate())
	{
		ApplyCurrentActionEffects();
		ActionComplete();
	}
}

void AIAgent::MoveUpdate(double delta)
{
	NavAgentUpdate(delta);
}

void AIAgent::MoveArrive()
{

}

Vector3 AIAgent::GetActionTargetPosition() const
{
	return get_global_transform().get_origin();
}

void AIAgent::ProcessMove(Vector3 actionLocation)
{
	if (GetNavAgent()->is_navigation_finished())
	{
		// Redirect to new location. This is kinda shit but will do for now.
		MoveTo(actionLocation);
	}
}

void AIAgent::OnMoveBegin(Vector3 actionLocation)
{
	UtilityFunctions::print("Begin Move.");
	MoveTo(actionLocation);
	if (GetCurrentAction() != nullptr)
	{
		UtilityFunctions::print("Found an action.");
	}
	else
	{
		UtilityFunctions::print("nullptr");
	}
	physicsFunc = &AIAgent::MoveUpdate;

	m_animStatePlayback->travel("MovementBlend");
}

void AIAgent::OnMoveStop()
{
	physicsFunc = &AIAgent::Empty;
}

void AIAgent::OnActionBegin(GOAPAIAction<Vector3>* action, GOAPAIActionTarget<Vector3>* actionTarget)
{
	UtilityFunctions::print("Begin action.");
	DEBUGAction* debugAction = (DEBUGAction*)action;
	UtilityFunctions::print(debugAction->name);

	m_actionTimer.Reset();
	m_actionTimer.SetTargetTime(debugAction->actionCompleteTime);

	processFunc = &AIAgent::ActionUpdate;

	m_animStatePlayback->travel("ActionStates");
	GetAnimTree()->set("parameters/ActionStates/blend_position", debugAction->animationTriggerValue);
}

void AIAgent::OnActionStop(GOAPAIAction<Vector3>* action, GOAPAIActionTarget<Vector3>* actionTarget)
{
	UtilityFunctions::print("Stop action.");
	DEBUGAction* debugAction = (DEBUGAction*)action;
	UtilityFunctions::print(debugAction->name);

	processFunc = &AIAgent::PlanUpdate;
	m_animStatePlayback->travel("MovementBlend");
}
