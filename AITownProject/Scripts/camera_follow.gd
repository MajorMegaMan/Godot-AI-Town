extends Camera3D

@export var targetNodePath : NodePath
@export var lookOffset : Vector3
@export var camLength : float = 3.0
var targetFollow

# Called when the node enters the scene tree for the first time.
func _ready():
	targetFollow = get_node(targetNodePath)
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	var toTarget : Vector3 = targetFollow.position - position
	toTarget.y = 0
	var length = toTarget.length()
	if(length > 0):
		var nextPos = targetFollow.position
		nextPos.y = position.y
		position = nextPos - ((toTarget / length) * camLength)
	var thing = targetFollow as Node3D
	look_at(thing.position + lookOffset)
	pass
