#include "ResourceExample.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ResourceExample::_bind_methods() {
	// Methods.
	ClassDB::bind_method(D_METHOD("simple_func"), &ResourceExample::simple_func);

	// Properties.
	ADD_GROUP("Test group", "group_");
	ADD_SUBGROUP("Test subgroup", "group_subgroup_");

	ClassDB::bind_method(D_METHOD("get_custom_position"), &ResourceExample::get_custom_position);
	ClassDB::bind_method(D_METHOD("set_custom_position", "position"), &ResourceExample::set_custom_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "group_subgroup_custom_position"), "set_custom_position", "get_custom_position");
}

ResourceExample::ResourceExample() {
	UtilityFunctions::print("Constructor.");
}

ResourceExample::~ResourceExample() {
	UtilityFunctions::print("Destructor.");
}

// Methods.
void ResourceExample::simple_func() {
	UtilityFunctions::print("  Simple func called.");
}

// Properties.
void ResourceExample::set_custom_position(const Vector2 &pos) {
	custom_position = pos;
	emit_changed();
}

Vector2 ResourceExample::get_custom_position() const {
	return custom_position;
}
