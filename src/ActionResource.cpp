#include "ActionResource.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include "AIAgent.h"
#include "AIActionTarget.h"

using namespace godot;

void ActionResource::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetName"), &ActionResource::GetName);
	ClassDB::bind_method(D_METHOD("SetName", "name"), &ActionResource::SetName);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "SetName", "GetName");

	ClassDB::bind_method(D_METHOD("GetActionCompleteTime"), &ActionResource::GetActionCompleteTime);
	ClassDB::bind_method(D_METHOD("SetActionCompleteTime", "actionCompleteTime"), &ActionResource::SetActionCompleteTime);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "action_complete_time"), "SetActionCompleteTime", "GetActionCompleteTime");

	ClassDB::bind_method(D_METHOD("GetAnimationTriggerValue"), &ActionResource::GetAnimationTriggerValue);
	ClassDB::bind_method(D_METHOD("SetAnimationTriggerValue", "animationTriggerValue"), &ActionResource::SetAnimationTriggerValue);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "animation_trigger_value"), "SetAnimationTriggerValue", "GetAnimationTriggerValue");

	ClassDB::bind_method(D_METHOD("get_script"), &ActionResource::get_script);
	ClassDB::bind_method(D_METHOD("set_script", "targetScript"), &ActionResource::set_script);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_script", PropertyHint::PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_script", "get_script");
}

ActionResource::ActionResource()
{
	m_worldStateObjectRef = memnew(WorldStateObject);
}

ActionResource::~ActionResource()
{
	memdelete(m_worldStateObjectRef);
}

float ActionResource::GetActionCompleteTime() const
{
	return m_actionCompleteTime;
}

void ActionResource::SetActionCompleteTime(float value)
{
	m_actionCompleteTime = value;
}

float ActionResource::GetAnimationTriggerValue() const
{
	return m_animationTriggerValue;
}

void ActionResource::SetAnimationTriggerValue(float value)
{
	m_animationTriggerValue = value;
}

String ActionResource::GetName() const
{
	return m_name;
}

void ActionResource::SetName(const String& value)
{
	m_name = value;
}

void ActionResource::set_script(const Ref<GDScript>& gdScript)
{
	m_scriptTarget = gdScript;
}

Ref<GDScript> ActionResource::get_script() const
{
	return m_scriptTarget;
}

void ActionResource::PerformEffects(GOAPWorldState& worldState)
{
	m_worldStateObjectRef->SetGOAPWorldState(worldState);

	Variant testVariant(m_worldStateObjectRef);

	m_scriptTarget->call("PerformEffects", testVariant);
}

bool ActionResource::CheckConditions(const GOAPWorldState& worldState)
{
	m_worldStateObjectRef->SetGOAPWorldState(worldState);

	Variant testVariant(m_worldStateObjectRef);

	Variant resultVariant = m_scriptTarget->call("CheckConditions", testVariant);	
	return resultVariant.booleanize();
}

GOAPAIActionTarget<Vector3>* ActionResource::FindActionTarget(const GOAPAIAgent<Vector3>& agent)
{
	AIAgent* aiAgent = (AIAgent*)&agent;
	Variant aiVariant(aiAgent);

	Variant resultVariant = m_scriptTarget->call("FindActionTarget", aiVariant);
	return Object::cast_to<AIActionTarget>(resultVariant);
}

bool ActionResource::IsInRange(const GOAPAIAgent<Vector3>& agent, const GOAPAIActionTarget<Vector3>* actionTarget)
{
	AIAgent* aiAgent = (AIAgent*)&agent;
	AIActionTarget* aiActionTarget = (AIActionTarget*)actionTarget;

	Variant aiVariant(aiAgent);
	Variant aiActionTargetVariant(aiActionTarget);

	Variant resultVariant = m_scriptTarget->call("IsInRange", aiVariant, aiActionTargetVariant);
	return resultVariant.booleanize();
}
