#include "BaseAgentStats.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void BaseAgentStats::_bind_methods()
{
	// Properties.
	ADD_GROUP("Base Agent Values", "baseAgentGroup_");

	ClassDB::bind_method(D_METHOD("GetName"), &BaseAgentStats::GetName);
	ClassDB::bind_method(D_METHOD("SetName", "name"), &BaseAgentStats::SetName);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "SetName", "GetName");

	ClassDB::bind_method(D_METHOD("GetBaseHunger"), &BaseAgentStats::GetBaseHunger);
	ClassDB::bind_method(D_METHOD("SetBaseHunger", "hungerValue"), &BaseAgentStats::SetBaseHunger);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "baseAgentGroup_hunger"), "SetBaseHunger", "GetBaseHunger");

	ClassDB::bind_method(D_METHOD("GetBaseHoldItem"), &BaseAgentStats::GetBaseHoldItem);
	ClassDB::bind_method(D_METHOD("SetBaseHoldItem", "holdItemKey"), &BaseAgentStats::SetBaseHoldItem);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "baseAgentGroup_hold_item", PropertyHint::PROPERTY_HINT_ENUM, "HOLDITEM_NONE, HOLDITEM_FOOD"), "SetBaseHoldItem", "GetBaseHoldItem");
}

BaseAgentStats::BaseAgentStats()
{
	CreateValueType(WorldKeys::WS_AGENT_HUNGER, sizeof(float));
	SetBaseHunger(100.0f);
	CreateValueType(WorldKeys::WS_AGENT_HOLDITEM, sizeof(int));
	SetBaseHoldItem(0);
}

BaseAgentStats::~BaseAgentStats()
{
	
}

float BaseAgentStats::GetBaseHunger() const
{
	return GetBaseReadValue(WorldKeys::WS_AGENT_HUNGER).GetFloat();
}

void BaseAgentStats::SetBaseHunger(float value)
{
	GetBaseValue(WorldKeys::WS_AGENT_HUNGER).SetFloat(value);
}

int BaseAgentStats::GetBaseHoldItem() const
{
	return GetBaseReadValue(WorldKeys::WS_AGENT_HOLDITEM).GetInt();
}

void BaseAgentStats::SetBaseHoldItem(int value)
{
	GetBaseValue(WorldKeys::WS_AGENT_HOLDITEM).SetInt(value);
}

String BaseAgentStats::GetName() const
{
	return m_name;
}

void BaseAgentStats::SetName(const String& value)
{
	m_name = value;
}

GOAPValue BaseAgentStats::GetBaseValue(int key)
{
	return GetGOAPValue(key);
}

GOAPReadValue BaseAgentStats::GetBaseReadValue(int key) const
{
	return GetReadValue(key);
}
