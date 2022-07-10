#ifndef AGENTSTATRESOURCE_CLASS_H
#define AGENTSTATRESOURCE_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/gd_script.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "GOAPagents.h"

using namespace godot;
using namespace GOAP;

// This is intended to be an abstact class, but godot does not seem to support pure abstact classes.
// The user should inherit from this class and override the start function with the 
class AgentStatResource : public Resource
{
	GDCLASS(AgentStatResource, Resource);

protected:
	static void _bind_methods();

private:
	Dictionary m_dictionary;

public:
	AgentStatResource();
	~AgentStatResource();

	// properties
	Dictionary GetDictionary();
	void SetDictionary(const Dictionary& value);

	// Functions.

private:

public:
	static bool ValidateType(const Variant& value);

	void AddToGOAPWorldState(GOAPWorldState& worldState);

	template <class T>
	void TryCreateInGOAPWorldState(GOAPWorldState& worldState, int key, Variant& value)
	{
		if (worldState.CreateValueType(key, sizeof(T)))
		{
			worldState.SetValue<T>(key, value);
		}
	}

	std::vector<int> GetGOAPKeys();
};

#endif // ! AGENTSTATRESOURCE_CLASS_H
