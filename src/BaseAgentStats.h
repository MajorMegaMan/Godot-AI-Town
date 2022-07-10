#ifndef BASEAGENTSTATS_CLASS_H
#define BASEAGENTSTATS_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "AgentStatResource.h"
#include "GOAPAI.h"
#include "DEBUGWorldKeys.h"

using namespace godot;
using namespace GOAP;

class BehaviourResource;

class BaseAgentStats : public Resource, GOAPWorldState
{
	GDCLASS(BaseAgentStats, Resource);

protected:
	static void _bind_methods();

private:
	String m_name;

	Array m_agentStatArray;
	std::vector<Ref<AgentStatResource>> m_agentStatResourceRefs;

public:
	BaseAgentStats();
	~BaseAgentStats();

	// properties
	String GetName() const;
	void SetName(const String& value);

	void set_agent_stat_array(const Array& value);
	Array get_agent_stat_array() const;

	// Functions.
	void _process_agent_stat_change();

	GOAPValue GetBaseValue(int key);
	GOAPReadValue GetBaseReadValue(int key) const;

	void AddAgentStat(AgentStatResource* agentStat);

	void ClearAgentStats();

	friend BehaviourResource;
};

#endif // ! BASEAGENTSTATS_CLASS_H
