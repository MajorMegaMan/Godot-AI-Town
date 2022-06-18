extends Node
# This is a bit dumb, but I've been having problems with gdExtensions running their _process in the editor.
# Therefore this script will call the Parent's Start and End functions for the MyTestClass to function at Runtime

var target

# Called when the node enters the scene tree for the first time.
func _ready():
	target = get_parent()
	# calling get_node in C++ is causing some sort of error, but it does at the moment seem harmless.
	target.Start()

# Called when the node exits the scene tree
func _exit_tree():
	target.Exit()
