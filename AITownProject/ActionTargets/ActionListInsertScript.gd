extends AIActionTarget

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func _enter_tree():
	ActionTargetList.AddActionTarget(self)

func _exit_tree():
	ActionTargetList.RemoveActionTarget(self)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
