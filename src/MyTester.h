#ifndef MYTESTER_CLASS_H
#define MYTESTER_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/node3D.hpp>
#include <godot_cpp/classes/gd_script.hpp>

#include <godot_cpp/core/binder_common.hpp>

using namespace godot;

class MyTester : public Node {
	GDCLASS(MyTester, Node);

protected:
	static void _bind_methods();

private:
	Ref<GDScript> m_scriptTarget;
	String m_funcName;

	Array m_testArray;

public:
	MyTester();
	~MyTester();

	// Property.
	void set_func_name(const String& funcName);
	String get_func_name() const;

	void set_script(const Ref<GDScript>& gdScript);
	Ref<GDScript> get_script() const;

	void set_array(const Array& value);
	Array get_array() const;

	// Functions.
	Variant call_void();

	virtual int VirtualTest() { return 8; }
};

#endif // ! MYTESTER_CLASS_H
