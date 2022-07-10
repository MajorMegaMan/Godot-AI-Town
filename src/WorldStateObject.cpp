#include "WorldStateObject.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void WorldStateObject::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetInt", "key"), &WorldStateObject::GetInt);
	ClassDB::bind_method(D_METHOD("SetInt", "key", "value"), &WorldStateObject::SetInt);

	ClassDB::bind_method(D_METHOD("GetBool", "key"), &WorldStateObject::GetBool);
	ClassDB::bind_method(D_METHOD("SetBool", "key", "value"), &WorldStateObject::SetBool);

	ClassDB::bind_method(D_METHOD("GetFloat", "key"), &WorldStateObject::GetFloat);
	ClassDB::bind_method(D_METHOD("SetFloat", "key", "value"), &WorldStateObject::SetFloat);

	ClassDB::bind_method(D_METHOD("DEBUGSetWorldState", "debugWorldState"), &WorldStateObject::DEBUGSetWorldState);
}

WorldStateObject::WorldStateObject()
{
	m_targetWorldState = nullptr;
}

WorldStateObject::~WorldStateObject()
{
	
}

int WorldStateObject::GetInt(int key)
{
	return m_targetWorldState->GetValue<int>(key);
}

void WorldStateObject::SetInt(int key, int value)
{
	m_targetWorldState->SetValue(key, value);
}

bool WorldStateObject::GetBool(int key)
{
	return m_targetWorldState->GetValue<bool>(key);
}

void WorldStateObject::SetBool(int key, bool value)
{
	m_targetWorldState->SetValue(key, value);
}

float WorldStateObject::GetFloat(int key)
{
	return m_targetWorldState->GetValue<float>(key);
}

void WorldStateObject::SetFloat(int key, float value)
{
	m_targetWorldState->SetValue(key, value);
}

void WorldStateObject::DEBUGSetWorldState(const Ref<BaseAgentStats>& debugWorldState)
{
	SetGOAPWorldState(*(GOAPWorldState*)debugWorldState.ptr());
}

void WorldStateObject::SetGOAPWorldState(const GOAPWorldState& worldState)
{
	m_targetWorldState = (GOAPWorldState*)&worldState;
}
