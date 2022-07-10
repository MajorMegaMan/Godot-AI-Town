extends Node

enum { AT_FOOD_ITEM, AT_FOOD_STORAGE }

var foodItemActionTargets : Array = []
var foodStorageActionTargets : Array = []

func AddActionTarget(actionTarget: AIActionTarget):
	var actionType = actionTarget.get_action_type()
	var actionTargetArray = GetActionTargetArray(actionType)
	actionTargetArray.push_back(actionTarget)
	pass

func RemoveActionTarget(actionTarget: AIActionTarget):
	var actionType = actionTarget.get_action_type()
	var actionTargetArray = GetActionTargetArray(actionType)
	actionTargetArray.erase(actionTarget)

func GetActionTargetArray(actionTargetType : int):
	if(actionTargetType == AT_FOOD_ITEM):
		var result = foodItemActionTargets
		return result
	else:
		return foodStorageActionTargets
	match(actionTargetType):
		AT_FOOD_ITEM:
			return foodItemActionTargets
		AT_FOOD_STORAGE:
			return foodStorageActionTargets

static func FindDistance(agentPos : Vector3, actionTargetPos : Vector3):
	var diff = actionTargetPos - agentPos
	return diff.length()

func FindClosestActionTarget(actionTargetType : int, agentPos : Vector3):
	var actionTargetArray : Array = GetActionTargetArray(actionTargetType)
	
	if actionTargetArray.size() == 0:
		return null
	
	var result = actionTargetArray[0] as AIActionTarget
	var lowestDist = FindDistance(agentPos, result.position) as float
	var indexRange = range(1, actionTargetArray.size())
	for i in indexRange:
		var actionTarget = actionTargetArray[i] as AIActionTarget
		var dist = FindDistance(agentPos, actionTarget.position)
		if(dist < lowestDist):
			result = actionTarget
			lowestDist = dist
	return result
