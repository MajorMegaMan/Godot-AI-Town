#ifndef ENABLEEXAMPLE_CLASS_H
#define ENABLEEXAMPLE_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/node3D.hpp>

#include <godot_cpp/core/binder_common.hpp>

using namespace godot;

class EnableExample : public Node3D {
	GDCLASS(EnableExample, Node3D);

protected:
	static void _bind_methods();

private:
	NodePath m_nodePath;

	typedef void (EnableExample::* UpdateFunc)(double delta);
	UpdateFunc physicsFunc;

public:
	EnableExample();
	~EnableExample();

	// Functions.
	void simple_func();

	// Property.
	void set_node_path(const NodePath& nodePath);
	NodePath get_node_path() const;

	void Start();
	void Update(double delta);
	void Exit();

	void Empty(double delta);

	// Virtual function override (no need to bind manually).

	virtual void TestVirtual();
};

#endif // ! ENABLEEXAMPLE_CLASS_H
