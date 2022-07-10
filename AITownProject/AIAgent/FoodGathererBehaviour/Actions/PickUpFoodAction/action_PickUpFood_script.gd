
static func PerformEffects(worldState : WorldStateObject):
	print("Performing effects")
	worldState.SetInt(AIWorldKeys.WS_AGENT_HOLDITEM, AIWorldKeys.ITEM_FOOD)
	pass

static func CheckConditions(worldState : WorldStateObject):
	print("Checking Conditions")
	var holdItem = worldState.GetInt(AIWorldKeys.WS_AGENT_HOLDITEM)
	return holdItem == AIWorldKeys.ITEM_NOTHING

static func FindActionTarget(agent : AIAgent):
	return ActionTargetList.FindClosestActionTarget(ActionTargetList.AT_FOOD_ITEM, agent.position)

static func IsInRange(agent : AIAgent, actionTarget : AIActionTarget):
	return ActionTargetList.FindDistance(agent.position, actionTarget.position) < 0.5
