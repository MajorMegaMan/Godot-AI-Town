#ifndef AIACTIONTARGET_CLASS_H
#define AIACTIONTARGET_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "DEBUGAIHeader.h"
#include "NodeStarter.h"

using namespace GOAP;
using namespace godot;

class AIActionTarget : public Node3D, public GOAPAIActionTarget<Vector3>, public NodeStarter
{
	GDCLASS(AIActionTarget, Node3D);

protected:
	static void _bind_methods();

	int m_actionType = 0; // This is a ActionTargetKey enum

public:
	AIActionTarget();
	~AIActionTarget();

	// properties
	int get_action_type();
	void set_action_type(int actionTargetKey);

	// Functions.
	// Node Starter interfaces. These are here becuase godot needs to connect bindings from this class in order to
	void Start()
	{
		NodeStarter::Start();
	}

	void Exit()
	{
		NodeStarter::Exit();
	}

	void OnStart() override;
	void OnUpdate(double delta) override;
	void OnExit() override;

private:
	// internals

	// GOAPAgentOverrides
	Vector3 GetActionTargetPosition() const override;
};

#endif // ! AIACTIONTARGET_CLASS_H