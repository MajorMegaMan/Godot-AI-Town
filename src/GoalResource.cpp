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

	BIND_PROPERTY_SIMPLE("goap_keys", Variant::ARRAY, GoalResource, get_goap_key_array, set_goap_key_array, "goap_keys");
	ClassDB::bind_method(D_METHOD("_process_agent_stat_change"), &GoalResource::_process_agent_stat_change);
	ClassDB::bind_method(D_METHOD("AddGOAPKey", "goapKeys"), &GoalResource::AddGOAPKey);
	ClassDB::bind_method(D_METHOD("ClearGOAPKeys"), &GoalResource::ClearGOAPKeys);
}

GoalResource::GoalResource() : m_GOAPKeys(this, "_process_agent_stat_change")
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

void GoalResource::set_goap_key_array(const Array& value)
{
	m_GOAPKeys.set_array(value);
	emit_changed();
}

Array GoalResource::get_goap_key_array() const
{
	return m_GOAPKeys.get_array();
}

void GoalResource::AddGOAPKey(const Ref<AgentStatResource>& goapKeys)
{
	m_GOAPKeys.AddValue(goapKeys);
	emit_changed();
}

void GoalResource::ClearGOAPKeys()
{
	m_GOAPKeys.Clear();
	emit_changed();
}

void GoalResource::_process_agent_stat_change()
{
	m_GOAPKeys.ProcessChange();
	emit_changed();
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
