extends Node3D

var animTree : AnimationTree
var animStatePlayback : AnimationNodeStateMachinePlayback

@export var blend_amount : float = 0.0

# Called when the node enters the scene tree for the first time.
func _ready():
	animTree = $Dude.get_node("AnimationTree")
	animStatePlayback = animTree.get("parameters/playback")
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	animTree.advance(delta)
	animTree.set("parameters/MovementBlend/Forward/blend_amount", blend_amount)
	pass

func _input(event):
	# Mouse in viewport coordinates.
	if event is InputEventMouseButton and event.pressed:
		if event.button_index == 1:
			blend_amount = 1.0
			print(animTree.get("parameters/MovementBlend/Forward/blend_amount"))
			pass
		elif event.button_index == 2:
			blend_amount = 0.0
			pass
	elif event is InputEventKey and event.pressed:
		if event.keycode == KEY_Q:
			animStatePlayback.travel("ActionStates")
			animTree.set("parameters/ActionStates/blend_position", 0)
		if event.keycode == KEY_W:
			animStatePlayback.travel("ActionStates")
			animTree.set("parameters/ActionStates/blend_position", 1)
		if event.keycode == KEY_E:
			animStatePlayback.travel("ActionStates")
			animTree.set("parameters/ActionStates/blend_position", 2)
		if event.keycode == KEY_D:
			animStatePlayback.travel("MovementBlend")
		if event.keycode == KEY_S:
			animStatePlayback.travel("ActionStates")
