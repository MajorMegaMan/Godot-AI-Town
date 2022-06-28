extends Node3D

var camera : Camera3D
@export var camPath : NodePath

signal click_event(worldPos)

@export var mouseLocation : NodePath
var mouseLocNode : Node3D

var mouseScreenPos : Vector2 = Vector2.ZERO

# Called when the node enters the scene tree for the first time.
func _ready():
	set_process_input(true)
	camera = get_node(camPath)
	mouseLocNode = get_node(mouseLocation)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	var hitInfo = CamRaycast(mouseScreenPos)
	if !hitInfo.is_empty():
		mouseLocNode.set_position(hitInfo.position)


func CamRaycast(screenPosition):
	# get data for positions for ray
	var from = camera.project_ray_origin(screenPosition)
	var to = from + camera.project_ray_normal(screenPosition) * 1000
	
	# get worldspace for ray to runction
	var space_state = get_world_3d().direct_space_state
	
	# create params
	var params : PhysicsRayQueryParameters3D
	params = PhysicsRayQueryParameters3D.new()
	params.set_from(from)
	params.set_to(to)
	
	# use space to cast ray with params
	var hitInfo = space_state.intersect_ray(params)
	return hitInfo

func _input(event):
	# Mouse in viewport coordinates.
	if event is InputEventMouseButton and event.pressed:
		if event.button_index == 1:
			var hitInfo = CamRaycast(event.position)
			if !hitInfo.is_empty():
				emit_signal("click_event", hitInfo.position)
		elif event.button_index == 2:
			var hitInfo = CamRaycast(event.position)
			if !hitInfo.is_empty():
				pass
	elif event is InputEventMouseMotion:
		mouseScreenPos = event.position
