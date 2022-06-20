#ifndef ANIMATIONTREEGETTER_CLASS_H
#define ANIMATIONTREEGETTER_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/animation_tree.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "NodePathProperty.h"
#include "NodeStarter.h"

class AnimationTreeGetter : public Node3D, public NodeStarter
{
	GDCLASS(AnimationTreeGetter, Node3D);

protected:
	static void _bind_methods();

private:
	NodePathProperty<AnimationTree> m_animProp;

public:
	AnimationTreeGetter();
	~AnimationTreeGetter();

	// properties
	NodePath get_anim_tree_path() const;
	void set_anim_tree_path(const NodePath& animPath);

	// Functions.
	AnimationTree* GetAnimationTree() const;

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

#endif // ! ANIMATIONTREEGETTER_CLASS_H