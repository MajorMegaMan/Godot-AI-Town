#include "MyTester.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "ActionResource.h"

using namespace godot;

void Thing(GDExtensionClassInstancePtr p_instance, const GDNativeTypePtr* p_args, GDNativeTypePtr r_ret)
{
	(*((int*)r_ret)) = ((MyTester*)p_instance)->VirtualTest();
}

void MyTester::_bind_methods() {
	// Methods.

	// Properties.
	ClassDB::bind_method(D_METHOD("get_func_name"), &MyTester::get_func_name);
	ClassDB::bind_method(D_METHOD("set_func_name", "funcName"), &MyTester::set_func_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "func_name"), "set_func_name", "get_func_name");

	ClassDB::bind_method(D_METHOD("get_script"), &MyTester::get_script);
	ClassDB::bind_method(D_METHOD("set_script", "targetScript"), &MyTester::set_script);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_script", PropertyHint::PROPERTY_HINT_RESOURCE_TYPE, "GDScript"), "set_script", "get_script");

	ClassDB::bind_method(D_METHOD("call_void"), &MyTester::call_void);

	ClassDB::bind_method(D_METHOD("get_array"), &MyTester::get_array);
	ClassDB::bind_method(D_METHOD("set_array", "values"), &MyTester::set_array);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "actions", PropertyHint::PROPERTY_HINT_ARRAY_TYPE, "ActionResource"), "set_array", "get_array");

	ClassDB::bind_virtual_method("MyTester", "VirtualTest", &Thing);
}

MyTester::MyTester()
{

}

MyTester::~MyTester()
{

}

void MyTester::set_func_name(const String& funcName)
{
	m_funcName = funcName;
}

String MyTester::get_func_name() const
{
	return m_funcName;
}

void MyTester::set_script(const Ref<GDScript>& gdScript)
{
	m_scriptTarget = gdScript;
}

Ref<GDScript> MyTester::get_script() const
{
	return m_scriptTarget;
}

void MyTester::set_array(const Array& value)
{
	m_testArray = value;
}

Array MyTester::get_array() const
{
	return m_testArray;
}

Variant MyTester::call_void()
{
	UtilityFunctions::print("My Virtual number is");
	UtilityFunctions::print(VirtualTest());
	UtilityFunctions::print("Trying to call...");

	Variant selfScriptVariant = Node::get_script();
	if (selfScriptVariant.booleanize())
	{
		UtilityFunctions::print("Found a script attached.");
		UtilityFunctions::print(Variant::get_type_name(selfScriptVariant.get_type()));
		auto selfScript = Object::cast_to<GDScript>(selfScriptVariant);
		UtilityFunctions::print(selfScript->get_class());

		if (selfScript != nullptr)
		{
			return call(m_funcName);
		}
		else
		{
			return m_scriptTarget->call(m_funcName);
		}
	}
	else
	{
		return m_scriptTarget->call(m_funcName);
	}
}

