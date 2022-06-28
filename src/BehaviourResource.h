#ifndef BEHAVIOURRESOURCE_CLASS_H
#define BEHAVIOURRESOURCE_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/gd_script.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "BaseAgentStats.h"
#include "WorldStateObject.h"

using namespace godot;
using namespace GOAP;

// This is intended to be an abstact class, but godot does not seem to support pure abstact classes.
// The user should inherit from this class and override the start function with the 
class BehaviourResource : public Resource, public GOAPAIBehaviour<Vector3>
{
	GDCLASS(BehaviourResource, Resource);

protected:
	static void _bind_methods();

private:
	String m_name;
	Ref<BaseAgentStats> m_baseAgentsStatsRef;

	Array m_actionArray;
	Array m_goalArray;

	Ref<GDScript> m_scriptTarget;

	// Internal functionality
	WorldStateObject* m_worldStateObjectRef;

public:
	BehaviourResource();
	~BehaviourResource();

	// properties
	// getter for godot resource ref
	void SetBaseAgentStatsResource(const Ref<BaseAgentStats>& baseAgentStats);
	// setter for godot resource ref
	Ref<BaseAgentStats> GetBaseAgentStatsResource();

	String GetName() const;
	void SetName(const String& value);

	void set_action_array(const Array& value);
	Array get_action_array() const;

	void set_goal_array(const Array& value);
	Array get_goal_array() const;

	void set_script(const Ref<GDScript>& gdScript);
	Ref<GDScript> get_script() const;

	// Functions.
	// Getter for BaseAgentStats
	BaseAgentStats* GetBaseAgentStats();

	// This must be here, but it's intended to be overridden in order to actually be used.
	virtual GOAPGoal* FindGoal(const GOAPWorldState& worldState) override;

	virtual void Update(float delta, GOAPWorldState& agentState) override;
};

#endif // ! BEHAVIOURRESOURCE_CLASS_H
