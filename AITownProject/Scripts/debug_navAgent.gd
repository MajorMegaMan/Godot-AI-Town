extends CharacterBody3D

@export var target : NodePath

@export var m_movementSpeed : float = 4.0
@onready var m_navAgent : NavigationAgent3D = $NavigationAgent3D

# Called when the node enters the scene tree for the first time.
func _ready():
	var agent_rid : RID = m_navAgent.get_rid()
	
	var map_rid : RID = m_navAgent.get_tree().get_root().get_world_3d().get_navigation_map()    
	NavigationServer3D.agent_set_map(agent_rid, map_rid)
	NavigationServer3D.agent_set_callback(agent_rid, m_navAgent, "_avoidance_done")
	
	SetTargetLocation(get_node(target).global_transform.origin)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func _physics_process(_delta):
	if m_navAgent.is_navigation_finished():
		return
	var nextPos : Vector3 = m_navAgent.get_next_location()
	var currentAgentPos : Vector3 = global_transform.origin
	var newVelocity : Vector3 = (nextPos - currentAgentPos).normalized() * m_movementSpeed
	m_navAgent.set_velocity(newVelocity)

func _on_navigation_agent_3d_velocity_computed(safe_velocity):
	velocity = safe_velocity
	move_and_slide()

func _on_navigation_agent_3d_target_reached():
	velocity = Vector3.ZERO

func SetTargetLocation(position : Vector3):
	m_navAgent.set_target_location(position)


func _on_mouse_ray_click_event(worldPos):
	SetTargetLocation(worldPos)
