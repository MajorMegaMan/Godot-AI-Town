
static func PerformEffects(worldState : WorldStateObject):
	worldState.SetInt(AIWorldKeys.WS_AGENT_HOLDITEM, AIWorldKeys.ITEM_NOTHING)
	var currentFoodCount = worldState.GetInt(AIWorldKeys.WS_GLOBAL_FOODCOUNT)
	worldState.SetInt(AIWorldKeys.WS_GLOBAL_FOODCOUNT, currentFoodCount + 1)
	pass

static func CheckConditions(worldState : WorldStateObject):
	var holdItem = worldState.GetInt(AIWorldKeys.WS_AGENT_HOLDITEM)
	return holdItem == AIWorldKeys.ITEM_FOOD

static func FindActionTarget(agent : AIAgent):
	return ActionTargetList.FindClosestActionTarget(ActionTargetList.AT_FOOD_STORAGE, agent.position)

static func IsInRange(agent : AIAgent, actionTarget : AIActionTarget):
	return ActionTargetList.FindDistance(agent.position, actionTarget.position) < 0.5
