#include "GoalResource.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void GoalResource::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetName"), &GoalResource::GetName);
	ClassDB::bind_method(D_METHOD("SetName", "name"), &GoalResource::SetName);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "SetName", "GetName");

	ClassDB::bind_method(D_METHOD("get_script"), &GoalResource::get_script);
	ClassDB::bind_method(D_METHOD("set_script", "targetScript"), &GoalResource::set_script);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_script", PropertyHint::PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_script", "get_script");
}

GoalResource::GoalResource()
{
	m_currentWorldStateObjectRef = memnew(WorldStateObject);
	m_baseWorldStateObjectRef = memnew(WorldStateObject);
}

GoalResource::~GoalResource()
{
	memdelete(m_currentWorldStateObjectRef);
	memdelete(m_baseWorldStateObjectRef);
}

String GoalResource::GetName() const
{
	return m_name;
}

void GoalResource::SetName(const String& value)
{
	m_name = value;
}

void GoalResource::set_script(const Ref<GDScript>& gdScript)
{
	m_scriptTarget = gdScript;
}

Ref<GDScript> GoalResource::get_script() const
{
	return m_scriptTarget;
}

bool GoalResource::PerformConditions(const GOAPWorldState& current, const GOAPWorldState& base)
{
	m_currentWorldStateObjectRef->SetGOAPWorldState(current);
	m_baseWorldStateObjectRef->SetGOAPWorldState(base);

	Variant currentVariant(m_currentWorldStateObjectRef);
	Variant baseVariant(m_baseWorldStateObjectRef);

	Variant resultVariant = m_scriptTarget->call("PerformConditions", currentVariant, baseVariant);
	return resultVariant.booleanize();
}
