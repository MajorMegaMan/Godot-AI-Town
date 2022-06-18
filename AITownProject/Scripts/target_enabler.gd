extends Node
# This is a bit dumb, but I've been having problems with gdExtensions running their _process in the editor.
# Therefore this script will call the Parent's Start and End functions for the MyTestClass to function at Runtime

@export var targetPath : NodePath
var target

# Called when the node enters the scene tree for the first time.
func _ready():
	pass

# Called when the node exits the scene tree
func _exit_tree():
	target.Exit()


func _on_mouse_ray_click_event(_worldPos):
	target = get_node(targetPath)
	target.Start()
