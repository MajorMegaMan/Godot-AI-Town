extends MyTester

@export var debugScript : Resource
@export var position : Vector3

@export var testWorldState : Resource

# Called when the node enters the scene tree for the first time.
func _ready():
	var actionTargetArray = ActionTargetList.GetActionTargetArray(ActionTargetList.AT_FOOD_ITEM) as Array
	var closest = ActionTargetList.FindClosestActionTarget(ActionTargetList.AT_FOOD_ITEM, position) as AIActionTarget
	print(actionTargetArray.size())
	print(closest.name)
	
	var tempWorldObject = WorldStateObject.new() as WorldStateObject
	tempWorldObject.DEBUGSetWorldState(testWorldState)
	
	var condition = debugScript.call("CheckConditions", tempWorldObject)
	print(condition)
	
	print(tempWorldObject.GetInt(WorldKeys.WS_AGENT_HOLDITEM))
	debugScript.call("PerformEffects", tempWorldObject)
	
	condition = debugScript.call("CheckConditions", tempWorldObject)
	print(condition)
	
	print(tempWorldObject.GetInt(WorldKeys.WS_AGENT_HOLDITEM))
	
	tempWorldObject.free()
	#debugScript.call(get_func_name())
	#print(call_void())
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func VirtualTest():
	return 32

func funcToCall():
	print("I want to say Something else")
	return VirtualTest()
