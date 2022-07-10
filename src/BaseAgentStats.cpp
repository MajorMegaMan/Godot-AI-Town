#include "BaseAgentStats.h"

#include <godot_cpp/core/class_db.hpp>

#include "MyUtility.h"

using namespace godot;

void BaseAgentStats::_bind_methods()
{
	// Properties.
	ADD_GROUP("Base Agent Values", "baseAgentGroup_");

	ClassDB::bind_method(D_METHOD("GetName"), &BaseAgentStats::GetName);
	ClassDB::bind_method(D_METHOD("SetName", "name"), &BaseAgentStats::SetName);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "SetName", "GetName");

	BIND_PROPERTY_SIMPLE("agent_stats", Variant::ARRAY, BaseAgentStats, get_agent_stat_array, set_agent_stat_array, "agent_stats");

	ClassDB::bind_method(D_METHOD("_process_agent_stat_change"), &BaseAgentStats::_process_agent_stat_change);
}

BaseAgentStats::BaseAgentStats()
{
	
}

BaseAgentStats::~BaseAgentStats()
{
	
}

String BaseAgentStats::GetName() const
{
	return m_name;
}

void BaseAgentStats::SetName(const String& value)
{
	m_name = value;
}

void BaseAgentStats::set_agent_stat_array(const Array& value)
{
	// Iterate through array. connect/disconnect "changed" signal
	ADD_RESOURCE_ARRAY(m_agentStatArray, value, AgentStatResource, ClearAgentStats, AddAgentStat);
	emit_changed();
}

Array BaseAgentStats::get_agent_stat_array() const
{
	return m_agentStatArray;
}

// This function should be called when changes to the targeted resource arrays are changed.
// The user shouldn't need to call it as signals are connected to the resources when they are added.
void BaseAgentStats::_process_agent_stat_change()
{
	// There is no need to remove or change reference to resources. but the GOAPWorldState's internal Arrays may need to add or remove values.
	// To ensure the values are correct. Clear first.
	Clear();

	// Reassign the values from the resource refs
	for (int i = 0; i < m_agentStatResourceRefs.size(); i++)
	{
		m_agentStatResourceRefs[i]->AddToGOAPWorldState(*this);
	}
}

GOAPValue BaseAgentStats::GetBaseValue(int key)
{
	return GetGOAPValue(key);
}

GOAPReadValue BaseAgentStats::GetBaseReadValue(int key) const
{
	return GetReadValue(key);
}

void BaseAgentStats::AddAgentStat(AgentStatResource* agentStat)
{
	Ref<AgentStatResource> targetRef = agentStat;
	m_agentStatResourceRefs.push_back(targetRef);
	targetRef->connect("changed", Callable(this, "_process_agent_stat_change"));
	agentStat->AddToGOAPWorldState(*this);
}

void BaseAgentStats::ClearAgentStats()
{
	for (int i = 0; i < m_agentStatResourceRefs.size(); i++)
	{
		// Should be safe to disconnect here as the references are connected when they are added to this list.
		m_agentStatResourceRefs[i].ptr()->disconnect("changed", Callable(this, "_process_agent_stat_change"));
	}
	m_agentStatResourceRefs.clear();
	Clear();
}
