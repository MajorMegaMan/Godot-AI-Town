#include "enableExample.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void EnableExample::_bind_methods() {
	// Methods.
	ClassDB::bind_method(D_METHOD("Start"), &EnableExample::Start);
	ClassDB::bind_method(D_METHOD("Exit"), &EnableExample::Exit);
	ClassDB::bind_method(D_METHOD("simple_func"), &EnableExample::simple_func);
	ClassDB::bind_method(D_METHOD("TestVirtual"), &EnableExample::TestVirtual);

	// Properties.
	ClassDB::bind_method(D_METHOD("get_node_path"), &EnableExample::get_node_path);
	ClassDB::bind_method(D_METHOD("set_node_path", "nodePath"), &EnableExample::set_node_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "node_path"), "set_node_path", "get_node_path");
}

EnableExample::EnableExample() {
	UtilityFunctions::print("Constructor.");
	physicsFunc = &EnableExample::Empty;
}

EnableExample::~EnableExample() {
	UtilityFunctions::print("Destructor.");
}

// Methods.
void EnableExample::simple_func() {
	UtilityFunctions::print("  Simple func called.");
}

// Properties.
void EnableExample::set_node_path(const NodePath& nodePath)
{
	m_nodePath = nodePath;
}

NodePath EnableExample::get_node_path() const {
	return m_nodePath;
}

void EnableExample::Start()
{
	// Set up physics func
	physicsFunc = &EnableExample::Update;
	UtilityFunctions::print("Start.");

	get_node_or_null(m_nodePath);
	//get_node<Node>(m_nodePath);
}

void EnableExample::Update(double delta)
{
	global_rotate({ 0.0f, 1.0f, 0.0f }, 1);
}

void EnableExample::Exit()
{
	UtilityFunctions::print("Exit.");
}

void EnableExample::Empty(double delta)
{
	// This is intentionally empty.
}

void EnableExample::TestVirtual()
{
	UtilityFunctions::print("Virtual.");
}
