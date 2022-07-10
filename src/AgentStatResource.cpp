#include "AgentStatResource.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void AgentStatResource::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("GetDictionary"), &AgentStatResource::GetDictionary);
	ClassDB::bind_method(D_METHOD("SetDictionary", "dictionary"), &AgentStatResource::SetDictionary);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "dictionary"), "SetDictionary", "GetDictionary");
}

AgentStatResource::AgentStatResource()
{
	
}

AgentStatResource::~AgentStatResource()
{
	
}

Dictionary AgentStatResource::GetDictionary()
{
	return m_dictionary;
}

void AgentStatResource::SetDictionary(const Dictionary& value)
{
	auto keys = value.keys();
	for (int i = 0; i < keys.size(); i++)
	{
		if (keys[i].get_type() != Variant::INT && keys[i].get_type() != Variant::NIL)
		{
			UtilityFunctions::push_warning("Unexpected key Type. Did not set Dictionary. Keys are only expected to be an INT.");
			return;
		}

		if (!ValidateType(value[keys[i]]))
		{
			UtilityFunctions::push_warning("Unexpected value type. Did not set Dictionary. Currently the only supported types are INT, FLOAT and BOOL.");
			return;
		}
	}

	m_dictionary = value;
	emit_changed();
}

bool AgentStatResource::ValidateType(const Variant& value)
{
	switch (value.get_type())
	{
	case godot::Variant::NIL:
	case godot::Variant::BOOL:
	case godot::Variant::INT:
	case godot::Variant::FLOAT:
		return true;

	case godot::Variant::STRING:
	case godot::Variant::VECTOR2:
	case godot::Variant::VECTOR2I:
	case godot::Variant::RECT2:
	case godot::Variant::RECT2I:
	case godot::Variant::VECTOR3:
	case godot::Variant::VECTOR3I:
	case godot::Variant::TRANSFORM2D:
	case godot::Variant::PLANE:
	case godot::Variant::QUATERNION:
	case godot::Variant::AABB:
	case godot::Variant::BASIS:
	case godot::Variant::TRANSFORM3D:
	case godot::Variant::COLOR:
	case godot::Variant::STRING_NAME:
	case godot::Variant::NODE_PATH:
	case godot::Variant::RID:
	case godot::Variant::OBJECT:
	case godot::Variant::CALLABLE:
	case godot::Variant::SIGNAL:
	case godot::Variant::DICTIONARY:
	case godot::Variant::ARRAY:
	case godot::Variant::PACKED_BYTE_ARRAY:
	case godot::Variant::PACKED_INT32_ARRAY:
	case godot::Variant::PACKED_INT64_ARRAY:
	case godot::Variant::PACKED_FLOAT32_ARRAY:
	case godot::Variant::PACKED_FLOAT64_ARRAY:
	case godot::Variant::PACKED_STRING_ARRAY:
	case godot::Variant::PACKED_VECTOR2_ARRAY:
	case godot::Variant::PACKED_VECTOR3_ARRAY:
	case godot::Variant::PACKED_COLOR_ARRAY:
	case godot::Variant::VARIANT_MAX:
	default:
	{
		UtilityFunctions::push_warning("INT, FLOAT and BOOL are currently the only available types.");
		return false;
		break;
	}
	}
}

void AgentStatResource::AddToGOAPWorldState(GOAPWorldState& worldState)
{
	auto keys =	m_dictionary.keys();

	for (int i = 0; i < keys.size(); i++)
	{
		if (keys[i].get_type() == Variant::NIL)
		{
			continue;
		}

		Variant value = m_dictionary[keys[i]];

		switch (value.get_type())
		{
		case godot::Variant::NIL:
			continue;
		case godot::Variant::BOOL:
			TryCreateInGOAPWorldState<bool>(worldState, keys[i], value);
			continue;
		case godot::Variant::INT:
			TryCreateInGOAPWorldState<int>(worldState, keys[i], value);
			continue;
		case godot::Variant::FLOAT:
			TryCreateInGOAPWorldState<float>(worldState, keys[i], value);
			continue;
		case godot::Variant::STRING:
			TryCreateInGOAPWorldState<String>(worldState, keys[i], value);
			continue;
		case godot::Variant::VECTOR2:
			TryCreateInGOAPWorldState<Vector2>(worldState, keys[i], value);
			continue;
		case godot::Variant::VECTOR2I:
			TryCreateInGOAPWorldState<Vector2i>(worldState, keys[i], value);
			continue;
		case godot::Variant::RECT2:
			TryCreateInGOAPWorldState<Rect2>(worldState, keys[i], value);
			continue;
		case godot::Variant::RECT2I:
			TryCreateInGOAPWorldState<Rect2i>(worldState, keys[i], value);
			continue;
		case godot::Variant::VECTOR3:
			TryCreateInGOAPWorldState<Vector3>(worldState, keys[i], value);
			continue;
		case godot::Variant::VECTOR3I:
			TryCreateInGOAPWorldState<Vector3i>(worldState, keys[i], value);
			continue;
		case godot::Variant::TRANSFORM2D:
			TryCreateInGOAPWorldState<Transform2D>(worldState, keys[i], value);
			continue;
		case godot::Variant::PLANE:
			TryCreateInGOAPWorldState<Plane>(worldState, keys[i], value);
			continue;
		case godot::Variant::QUATERNION:
			TryCreateInGOAPWorldState<Quaternion>(worldState, keys[i], value);
			continue;
		case godot::Variant::AABB:
			TryCreateInGOAPWorldState<AABB>(worldState, keys[i], value);
			continue;
		case godot::Variant::BASIS:
			TryCreateInGOAPWorldState<Basis>(worldState, keys[i], value);
			continue;
		case godot::Variant::TRANSFORM3D:
			TryCreateInGOAPWorldState<Transform3D>(worldState, keys[i], value);
			continue;
		case godot::Variant::COLOR:
			TryCreateInGOAPWorldState<Color>(worldState, keys[i], value);
			continue;
		case godot::Variant::STRING_NAME:
			TryCreateInGOAPWorldState<StringName>(worldState, keys[i], value);
			continue;
		case godot::Variant::NODE_PATH:
			TryCreateInGOAPWorldState<NodePath>(worldState, keys[i], value);
			continue;
		case godot::Variant::RID:
			TryCreateInGOAPWorldState<RID>(worldState, keys[i], value);
			continue;
		case godot::Variant::OBJECT:
			// Objects seem to be the only type that is a ptr rather than a class
			TryCreateInGOAPWorldState<Object*>(worldState, keys[i], value);
			continue;
		case godot::Variant::CALLABLE:
			TryCreateInGOAPWorldState<Callable>(worldState, keys[i], value);
			continue;
		case godot::Variant::SIGNAL:
			TryCreateInGOAPWorldState<Signal>(worldState, keys[i], value);
			continue;
		case godot::Variant::DICTIONARY:
			TryCreateInGOAPWorldState<Dictionary>(worldState, keys[i], value);
			continue;
		case godot::Variant::ARRAY:
			TryCreateInGOAPWorldState<Array>(worldState, keys[i], value);
			continue;
		case godot::Variant::PACKED_BYTE_ARRAY:
			TryCreateInGOAPWorldState<PackedByteArray>(worldState, keys[i], value);
			continue;
		case godot::Variant::PACKED_INT32_ARRAY:
			TryCreateInGOAPWorldState<PackedInt32Array>(worldState, keys[i], value);
			continue;
		case godot::Variant::PACKED_INT64_ARRAY:
			TryCreateInGOAPWorldState<PackedInt64Array>(worldState, keys[i], value);
			continue;
		case godot::Variant::PACKED_FLOAT32_ARRAY:
			TryCreateInGOAPWorldState<PackedFloat32Array>(worldState, keys[i], value);
			continue;
		case godot::Variant::PACKED_FLOAT64_ARRAY:
			TryCreateInGOAPWorldState<PackedFloat64Array>(worldState, keys[i], value);
			continue;
		case godot::Variant::PACKED_STRING_ARRAY:
			TryCreateInGOAPWorldState<PackedStringArray>(worldState, keys[i], value);
			continue;
		case godot::Variant::PACKED_VECTOR2_ARRAY:
			TryCreateInGOAPWorldState<PackedVector2Array>(worldState, keys[i], value);
			continue;
		case godot::Variant::PACKED_VECTOR3_ARRAY:
			TryCreateInGOAPWorldState<PackedVector3Array>(worldState, keys[i], value);
			continue;
		case godot::Variant::PACKED_COLOR_ARRAY:
			TryCreateInGOAPWorldState<PackedColorArray>(worldState, keys[i], value);
			continue;
		case godot::Variant::VARIANT_MAX:
		default:
		{
			UtilityFunctions::push_warning("INT, FLOAT and BOOL are currently the only available types.");
			continue;
		}
		}
	}
}

std::vector<int> AgentStatResource::GetGOAPKeys()
{
	Array keyArray = m_dictionary.keys();
	std::vector<int> keysResult;

	for (int i = 0; i < keyArray.size(); i++)
	{
		// The expected values of keys should be ints. So it should be fine to directly send variant values.
		keysResult.push_back(keyArray[i]);
	}

	return keysResult;
}
