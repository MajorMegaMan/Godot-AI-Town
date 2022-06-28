#ifndef DEBUG_WORLDKEYS_H
#define DEBUG_WORLDKEYS_H

#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/core/binder_common.hpp>

using namespace godot;

class WorldKeys : public godot::Object
{
	GDCLASS(WorldKeys, Object);

protected:
	static void _bind_methods();

public:
	// WS is short for WorldState
	enum WorldStateKey
	{
		// world var
		WS_GLOBAL_FOODCOUNT,

		// selfish var
		WS_AGENT_HUNGER,
		WS_AGENT_HOLDITEM
	};

	// AT is short for Action Target
	enum ActionTargetKeys
	{
		AT_FOOD_ITEM,
		AT_FOOD_STORAGE
	};
};

VARIANT_ENUM_CAST(WorldKeys, WorldStateKey);
VARIANT_ENUM_CAST(WorldKeys, ActionTargetKeys);

enum GOAPKey
{
	// world var
	FOODCOUNT,

	// selfish var
	HUNGER,
	HOLDITEM
};

// AT is short for Action Target
enum ActionTargetKey
{
	AT_FOOD_ITEM,
	AT_FOOD_STORAGE
};

#endif // ! DEBUG_WORLDKEYS_H