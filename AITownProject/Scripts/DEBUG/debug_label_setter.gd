extends Node
@export var dictionary : PackedInt64Array
@export var labelNodePath : NodePath
var labelNode : Label
@export var agentPath : NodePath
var agent

# Define a format string
var format_string_all = "%s\n%s\n%s\n"

var format_string_foodCount = "FoodCount: {val}"
var format_string_hunger = "Hunger: {val}"
var format_string_holdItem = "HoldItem: {val}"

# Called when the node enters the scene tree for the first time.
func _ready():
	labelNode = get_node(labelNodePath)
	agent = get_node(agentPath)
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	# Using the 'format' method, replace the 'str' placeholder
	var foodCountText = FormatLabelPart(format_string_foodCount, "val", agent.GetWorldStateInt(WorldKeys.WS_GLOBAL_FOODCOUNT))
	
	var hungerText = FormatLabelPart(format_string_hunger, "val", agent.GetWorldStateFloat(WorldKeys.WS_AGENT_HUNGER))
	var holdItemText = FormatLabelPart(format_string_holdItem, "val", agent.GetWorldStateInt(WorldKeys.WS_AGENT_HOLDITEM))
	
	var actual_string = format_string_all % [hungerText, holdItemText, foodCountText]
	labelNode.text = actual_string
	pass

func FormatLabelPart(formatString, textReplace, value):
	return formatString.format({ textReplace : value})

func _input(event):
	# Mouse in viewport coordinates.
	if event is InputEventMouseButton and event.pressed:
		if event.button_index == 1:
			pass
		elif event.button_index == 2:
			pass
	elif event is InputEventKey and event.pressed:
		if event.keycode == KEY_Q:
			pass
