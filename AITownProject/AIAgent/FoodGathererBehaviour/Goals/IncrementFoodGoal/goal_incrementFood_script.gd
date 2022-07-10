
# returns a boolean value if the current world state meets the users requirements
# current worldstate is the world state of the current node of the planner.
# base world state is the world state that the planner started in.
# An example of iterating would be GetInt from currentWorldState > GetInt from baseWorldState
static func PerformConditions(currentWorldState, baseWorldState):
	var currentFoodCount = currentWorldState.GetInt(WorldKeys.WS_GLOBAL_FOODCOUNT)
	var baseFoodCount = baseWorldState.GetInt(WorldKeys.WS_GLOBAL_FOODCOUNT)
	return currentFoodCount > baseFoodCount
