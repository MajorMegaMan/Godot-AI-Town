#ifndef GAMEMANAGER_CLASS_H
#define GAMEMANAGER_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "BehaviourResource.h"
#include "NodeStarter.h"

using namespace godot;

class GameManager : public Node3D, public NodeStarter
{
	GDCLASS(GameManager, Node3D);

protected:
	static void _bind_methods();

	Ref<BehaviourResource> m_behaviourResourceRef;

public:
	GameManager();
	~GameManager();

	// getter for godot resource ref
	void SetBehaviourResourceRef(const Ref<BehaviourResource>& behaviourResourceRef);
	// setter for godot resource ref
	Ref<BehaviourResource> GetBehaviourResourceRef();

	// Getter for BaseAgentStats
	BehaviourResource* GetBehaviourResource();

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
};

#endif // ! GAMEMANAGER_CLASS_H