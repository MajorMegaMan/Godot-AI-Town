#ifndef WORLDSTATEOBJECT_CLASS_H
#define WORLDSTATEOBJECT_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "GOAPAI.h"
#include "DEBUGWorldKeys.h"

using namespace godot;
using namespace GOAP;

// This is a class that is an interface for gdScript bindings to get values out of the target worldstate.
// Currently only handles simple types.
class WorldStateObject : public Object
{
	GDCLASS(WorldStateObject, Object);

protected:
	static void _bind_methods();

private:
	GOAPWorldState* m_targetWorldState;

public:
	WorldStateObject();
	~WorldStateObject();

	// properties

	// Functions.
	int GetInt(int key);
	void SetInt(int key, int value);

	bool GetBool(int key);
	void SetBool(int key, bool value);

	float GetFloat(int key);
	void SetFloat(int key, float value);

	// Non Godot Functions
	void SetGOAPWorldState(const GOAPWorldState& worldState);
};

#endif // ! WORLDSTATEOBJECT_CLASS_H
