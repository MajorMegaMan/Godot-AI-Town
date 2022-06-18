#include "AIActionTarget.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/world3D.hpp>
#include <godot_cpp/classes/navigation_server3d.hpp>

using namespace godot;

void AIActionTarget::_bind_methods()
{
	// Methods.
	ClassDB::bind_method(D_METHOD("Start"), &AIActionTarget::Start);
	ClassDB::bind_method(D_METHOD("Exit"), &AIActionTarget::Exit);
	// Properties.

	ClassDB::bind_method(D_METHOD("get_action_type"), &AIActionTarget::get_action_type);
	ClassDB::bind_method(D_METHOD("set_action_type", "actionType"), &AIActionTarget::set_action_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "action_target_type", PropertyHint::PROPERTY_HINT_ENUM, "AT_FOOD_ITEM, AT_FOOD_STORAGE"), "set_action_type", "get_action_type");
}

AIActionTarget::AIActionTarget()
{
	
}

AIActionTarget::~AIActionTarget()
{
	
}

int AIActionTarget::get_action_type()
{
	return m_actionType;
}

void AIActionTarget::set_action_type(int actionTargetKey)
{
	m_actionType = actionTargetKey;
}

void AIActionTarget::OnStart()
{
	AIActionTargetList::add_action_target(m_actionType, this);
}

void AIActionTarget::OnUpdate(double delta)
{

}

void AIActionTarget::OnExit()
{
	AIActionTargetList::remove_action_target(m_actionType, this);
}

// Methods.
Vector3 AIActionTarget::GetActionTargetPosition() const
{
	return get_global_transform().get_origin();
}
