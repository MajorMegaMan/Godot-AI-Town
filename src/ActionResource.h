#ifndef ACTIONRESOURCE_CLASS_H
#define ACTIONRESOURCE_CLASS_H

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
class ActionResource : public Resource, public GOAPAIAction<Vector3>
{
	GDCLASS(ActionResource, Resource);

protected:
	static void _bind_methods();

private:
	String m_name;
	float m_actionCompleteTime = 0.0f;
	float m_animationTriggerValue = 0.0f;

	Ref<GDScript> m_scriptTarget;

	// Internal functionality
	WorldStateObject* m_worldStateObjectRef;

public:
	ActionResource();
	~ActionResource();

	// properties
	float GetActionCompleteTime() const;
	void SetActionCompleteTime(float value);

	float GetAnimationTriggerValue() const;
	void SetAnimationTriggerValue(float value);

	String GetName() const;
	void SetName(const String& value);

	void set_script(const Ref<GDScript>& gdScript);
	Ref<GDScript> get_script() const;

	// Functions.

private:


public:
	virtual void PerformEffects(GOAPWorldState& worldState) override;

	virtual bool CheckConditions(const GOAPWorldState& worldState) override;

	// agent should be expected to be a type that inherits godot object
	virtual GOAPAIActionTarget<Vector3>* FindActionTarget(const GOAPAIAgent<Vector3>& agent) override;

	// agent  and action target should be expected to be a type that inherits godot object
	virtual bool IsInRange(const GOAPAIAgent<Vector3>& agent, const GOAPAIActionTarget<Vector3>* actionTarget) override;
};

#endif // ! ACTIONRESOURCE_CLASS_H
