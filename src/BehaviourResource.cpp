#include "BehaviourResource.h"

#include <godot_cpp/core/class_db.hpp>

#include "MyUtility.h"

#include "ActionResource.h"
#include "GoalResource.h"

using namespace godot;

void BehaviourResource::_bind_methods()
{
	// Properties.
	ClassDB::bind_method(D_METHOD("GetName"), &BehaviourResource::GetName);
	ClassDB::bind_method(D_METHOD("SetName", "name"), &BehaviourResource::SetName);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "SetName", "GetName");

	ClassDB::bind_method(D_METHOD("GetBaseAgentStatsResource"), &BehaviourResource::GetBaseAgentStatsResource);
	ClassDB::bind_method(D_METHOD("SetBaseAgentStatsResource", "baseAgentStats"), &BehaviourResource::SetBaseAgentStatsResource);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "base_agent_values", PropertyHint::PROPERTY_HINT_RESOURCE_TYPE, "BaseAgentStats"), "SetBaseAgentStatsResource", "GetBaseAgentStatsResource");

	ClassDB::bind_method(D_METHOD("get_script"), &BehaviourResource::get_script);
	ClassDB::bind_method(D_METHOD("set_script", "targetScript"), &BehaviourResource::set_script);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_script", PropertyHint::PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_script", "get_script");

	ClassDB::bind_method(D_METHOD("get_action_array"), &BehaviourResource::get_action_array);
	ClassDB::bind_method(D_METHOD("set_action_array", "actions"), &BehaviourResource::set_action_array);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "actions"), "set_action_array", "get_action_array");

	ClassDB::bind_method(D_METHOD("get_goal_array"), &BehaviourResource::get_goal_array);
	ClassDB::bind_method(D_METHOD("set_goal_array", "goals"), &BehaviourResource::set_goal_array);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "goals"), "set_goal_array", "get_goal_array");

	ClassDB::bind_method(D_METHOD("_process_action_array_change"), &BehaviourResource::_process_action_array_change);
}

BehaviourResource::BehaviourResource()
{
	m_worldStateObjectRef = memnew(WorldStateObject());
}

BehaviourResource::~BehaviourResource()
{
	memdelete(m_worldStateObjectRef);
}

void BehaviourResource::SetBaseAgentStatsResource(const Ref<BaseAgentStats>& baseAgentStats)
{
	if (m_baseAgentsStatsRef.is_valid())
	{
		m_baseAgentsStatsRef->disconnect("changed", Callable(this, "_process_action_array_change"));
	}
	m_baseAgentsStatsRef = baseAgentStats;
	if (m_baseAgentsStatsRef.is_valid())
	{
		m_baseAgentsStatsRef->connect("changed", Callable(this, "_process_action_array_change"));
	}

	_process_action_array_change();
}

Ref<BaseAgentStats> BehaviourResource::GetBaseAgentStatsResource()
{
	return m_baseAgentsStatsRef;
}

String BehaviourResource::GetName() const
{
	return m_name;
}

void BehaviourResource::SetName(const String& value)
{
	m_name = value;
}

void BehaviourResource::set_action_array(const Array& value)
{
	ADD_RESOURCE_ARRAY(m_actionArray, value, ActionResource, ClearActions, AddAction);
}

Array BehaviourResource::get_action_array() const
{
	return m_actionArray;
}

void BehaviourResource::set_goal_array(const Array& value)
{
	ADD_RESOURCE_ARRAY(m_goalArray, value, GoalResource, ClearGoals, AddGoal);
}

Array BehaviourResource::get_goal_array() const
{
	return m_goalArray;
}

void BehaviourResource::set_script(const Ref<GDScript>& gdScript)
{
	m_scriptTarget = gdScript;
}

Ref<GDScript> BehaviourResource::get_script() const
{
	return m_scriptTarget;
}

BaseAgentStats* BehaviourResource::GetBaseAgentStats()
{
	return m_baseAgentsStatsRef.ptr();
}

void BehaviourResource::_process_action_array_change()
{
	UtilityFunctions::print("Updating Behaviour");
	if (!m_baseAgentsStatsRef.is_null())
	{
		SetBaseAgentValues(*m_baseAgentsStatsRef.ptr());
	}
	else
	{
		ClearBaseAgentValues();
	}
}

GOAPGoal* BehaviourResource::FindGoal(const GOAPWorldState& worldState)
{
	m_worldStateObjectRef->SetGOAPWorldState(worldState);

	Variant worldVariant(m_worldStateObjectRef);

	//Variant selfScript = Object::get_script();
	//if (selfScript.booleanize())
	//{
	//	Variant result = call("FindGoal", worldVariant);
	//	return Object::cast_to<GoalResource>(result);
	//}
	//else
	//{
	//	return nullptr;
	//}

	Variant resultVariant = m_scriptTarget->call("FindGoal", get_goal_array(), worldVariant);

	if (resultVariant.booleanize())
	{
		auto goalResource = Object::cast_to<GoalResource>(resultVariant);
		UtilityFunctions::print(goalResource->get_class());
	}
	else
	{
		UtilityFunctions::print("Variant got busted.");
		UtilityFunctions::print(Variant::get_type_name(resultVariant.get_type()));
		return nullptr;
	}

	return Object::cast_to<GoalResource>(resultVariant);
}

void BehaviourResource::Update(float delta, GOAPWorldState& agentState)
{
	m_worldStateObjectRef->SetGOAPWorldState(agentState);

	Variant worldVariant(m_worldStateObjectRef);

	Variant resultVariant = m_scriptTarget->call("Update", delta, worldVariant);
}
