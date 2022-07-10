extends AIAgent

var deltaTime : float = 0.0

# Called when the node enters the scene tree for the first time.
func _ready():
	self.Start()
	pass # Replace with function body.

func _exit_tree():
	self.Exit()

func _input(event):
	# Mouse in viewport coordinates.
	if event is InputEventMouseButton and event.pressed:
		if event.button_index == 1:
			#self.Update(deltaTime)
			pass
	if event is InputEventKey and event.pressed:
		var keyEvent = event as InputEventKey
		if keyEvent.keycode == KEY_1:
			self.DEBUGFindPlan()
		elif keyEvent.keycode == KEY_2:
			self.DEBUGPopNext()
		if keyEvent.keycode == KEY_3:
			self.DEBUGTestCurrentActionRange()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	deltaTime = delta
	#self.Update(delta)
	pass

func _physics_process(delta):
	self.PhysicsUpdate(delta)
	pass
