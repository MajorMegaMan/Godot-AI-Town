#ifndef GOALRESOURCE_CLASS_H
#define GOALRESOURCE_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/gd_script.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "GOAPAI.h"
#include "DEBUGWorldKeys.h"
#include "WorldStateObject.h"

using namespace godot;
using namespace GOAP;

// This is intended to be an abstact class, but godot does not seem to support pure abstact classes.
// The user should inherit from this class and override the start function with the 
class GoalResource : public Resource, public GOAPGoal
{
	GDCLASS(GoalResource, Resource);

protected:
	static void _bind_methods();

private:
	String m_name;
	Ref<GDScript> m_scriptTarget;

	// Internal functionality
	WorldStateObject* m_currentWorldStateObjectRef;
	WorldStateObject* m_baseWorldStateObjectRef;

public:
	GoalResource();
	~GoalResource();

	// properties
	String GetName() const;
	void SetName(const String& value);

	void set_script(const Ref<GDScript>& gdScript);
	Ref<GDScript> get_script() const;

	// Functions.

private:


public:
	virtual bool PerformConditions(const GOAPWorldState& current, const GOAPWorldState& base) override;
};

#endif // ! GOALRESOURCE_CLASS_H
