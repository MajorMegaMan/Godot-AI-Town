extends CharacterBody3D

var m_inputKeyW : MyKeyInput = MyKeyInput.new()
var m_inputKeyS : MyKeyInput = MyKeyInput.new()
var m_inputKeyA : MyKeyInput = MyKeyInput.new()
var m_inputKeyD : MyKeyInput = MyKeyInput.new()
var m_inputKeySpace : MyKeyInput = MyKeyInput.new()

var m_inputDir : Vector2 = Vector2(0,0)

@export var m_moveSpeed = 5.0
@export var gravity = Vector3(0.0, -9.81, 0.0)

class MyKeyInput:
	var m_keyCode : int = 0
	var m_keyIsDown : bool = false
	var keyUpFunc
	var keyDownFunc
	
	func Init(keyCode, upFunc, downFunc):
		SetKeyCode(keyCode)
		Connect(upFunc, downFunc)
	
	func SetKeyCode(keyCode):
		m_keyCode = keyCode
	
	func Connect(upFunc, downFunc):
		keyUpFunc = upFunc
		keyDownFunc = downFunc
	
	func FindKeyState(event):
		if event.pressed:
			if event.keycode == m_keyCode and !m_keyIsDown:
				keyDownFunc.call()
				m_keyIsDown = true
		else:
			if m_keyIsDown:
				keyUpFunc.call()
				m_keyIsDown = false

# Called when the node enters the scene tree for the first time.
func _ready():
	m_inputKeyW.Init(KEY_W, WKeyUp, WKeyDown)
	m_inputKeyS.Init(KEY_S, SKeyUp, SKeyDown)
	m_inputKeyA.Init(KEY_A, AKeyUp, AKeyDown)
	m_inputKeyD.Init(KEY_D, DKeyUp, DKeyDown)
	m_inputKeySpace.Init(KEY_SPACE, SpaceKeyUp, SpaceKeyDown)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func _physics_process(delta):
	velocity += gravity * delta
	
	var moveDelta : Vector3 = Vector3(m_inputDir.x, 0.0, m_inputDir.y)
	moveDelta = moveDelta.normalized() * m_moveSpeed
	velocity.x = moveDelta.x
	velocity.z = moveDelta.z
	move_and_slide()

func _input(event):
	if event is InputEventKey:
		m_inputKeyW.FindKeyState(event)
		m_inputKeyS.FindKeyState(event)
		m_inputKeyA.FindKeyState(event)
		m_inputKeyD.FindKeyState(event)
		m_inputKeySpace.FindKeyState(event)

func WKeyDown():
	m_inputDir.y += 1.0

func WKeyUp():
	m_inputDir.y -= 1.0

func SKeyDown():
	m_inputDir.y -= 1.0

func SKeyUp():
	m_inputDir.y += 1.0

func AKeyDown():
	m_inputDir.x -= 1.0

func AKeyUp():
	m_inputDir.x += 1.0

func DKeyDown():
	m_inputDir.x += 1.0

func DKeyUp():
	m_inputDir.x -= 1.0

func SpaceKeyDown():
	velocity.y += 10.0

func SpaceKeyUp():
	pass
