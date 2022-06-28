#include "BehaviourResource.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
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
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "actions", PropertyHint::PROPERTY_HINT_ARRAY_TYPE, "ActionResource"), "set_action_array", "get_action_array");

	ClassDB::bind_method(D_METHOD("get_goal_array"), &BehaviourResource::get_goal_array);
	ClassDB::bind_method(D_METHOD("set_goal_array", "goals"), &BehaviourResource::set_goal_array);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "goals", PropertyHint::PROPERTY_HINT_ARRAY_TYPE, "GoalResource"), "set_goal_array", "get_goal_array");
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
	m_baseAgentsStatsRef = baseAgentStats;
	if (!m_baseAgentsStatsRef.is_null())
	{
		SetBaseAgentValues(*baseAgentStats.ptr());
	}
	else
	{
		ClearBaseAgentValues();
	}
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

// Expects Type T to Inherit from godot::Object
template<class T>
bool CheckArrayForType(const Array& value)
{
	bool allActionResource = true;
	for (int i = 0; i < value.size(); i++)
	{
		auto element = value[i];
		if (element.get_type() == 0)
		{
			// element is null
			continue;
		}
		else if (element.get_type() != Variant::OBJECT)
		{
			// type is not object
			allActionResource = false;
			break;
		}
		else
		{
			// type is object

			// Check if object reference is not empty
			if (element.booleanize())
			{
				// element not empty
				// Check if the object is actually the resource type we want
				auto target = Object::cast_to<T>(element);
				
				if (target != nullptr && !target->is_class(T::get_class_static()))
				{
					UtilityFunctions::print("Unexpected element in Array!");
					allActionResource = false;
					break;
				}
				else if (target == nullptr)
				{
					// Shouldn't get here, but just in case
					UtilityFunctions::print("Unexpected element in Array!");
					allActionResource = false;
					break;
				}
			}
		}
	}
	return allActionResource;
}

#define MESSAGE_CLASS(t_resourceClass) #t_resourceClass
#define EXPECTED_TYPE_MESSAGE_PREFIX "Expected elements of this array are: "
#define EXPECTED_TYPE_MESSAGE_SUFFIX ". Did not set the target array value."
#define EXPECTED_TYPE_MESSAGE(t_resourceClass) EXPECTED_TYPE_MESSAGE_PREFIX MESSAGE_CLASS(t_resourceClass) EXPECTED_TYPE_MESSAGE_SUFFIX

#define MESSAGE_SUCCESS_PREFIX "Success: "
#define MESSAGE_TYPE_SUCCESS(t_resourceClass) MESSAGE_SUCCESS_PREFIX MESSAGE_CLASS(t_resourceClass)

#define ADD_RESOURCE_ARRAY(targetArray, value, t_resourceClass, m_clearMethod, m_addMethod)										\
if (CheckArrayForType<t_resourceClass>(value))																					\
{																																\
	targetArray = value;																										\
																																\
	m_clearMethod();																											\
	for (int i = 0; i < value.size(); i++)																						\
	{																															\
		auto element = value[i];																								\
		if (element.get_type() != 0)																							\
		{																														\
			if (!element.booleanize())																							\
			{																													\
				UtilityFunctions::push_warning("Empty element.");																\
				continue;																										\
			}																													\
																																\
			auto target = Object::cast_to<t_resourceClass>(element);															\
			if (target != nullptr)																								\
			{																													\
				m_addMethod(target);																							\
				UtilityFunctions::print(MESSAGE_TYPE_SUCCESS(t_resourceClass));													\
			}																													\
			else																												\
			{																													\
				UtilityFunctions::push_error("Null ptr to Object.");															\
			}																													\
		}																														\
	}																															\
}																																\
else																															\
{																																\
	UtilityFunctions::push_error(EXPECTED_TYPE_MESSAGE(t_resourceClass));															\
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

GOAPGoal* BehaviourResource::FindGoal(const GOAPWorldState& worldState)
{
	m_worldStateObjectRef->SetGOAPWorldState(worldState);

	Variant worldVariant(m_worldStateObjectRef);

	Variant resultVariant = m_scriptTarget->call("FindGoal", get_goal_array(), worldVariant);
	return Object::cast_to<GoalResource>(resultVariant);
}

void BehaviourResource::Update(float delta, GOAPWorldState& agentState)
{
	m_worldStateObjectRef->SetGOAPWorldState(agentState);

	Variant worldVariant(m_worldStateObjectRef);

	Variant resultVariant = m_scriptTarget->call("Update", delta, worldVariant);
}
