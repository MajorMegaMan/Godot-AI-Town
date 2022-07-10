#ifndef MY_UTILITY_H
#define MY_UTILITY_H

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <vector>

using namespace godot;

// Expects Type T to Inherit from godot::Object
template<class T>
bool CheckArrayForObjectType(const Array& value)
{
	bool allActionResource = true;
	for (int i = 0; i < value.size(); i++)
	{
		auto element = value[i];
		if (element.get_type() == 0)
		{
			// element is null
			continue;
		}
		else if (element.get_type() != Variant::OBJECT)
		{
			// type is not object
			allActionResource = false;
			break;
		}
		else
		{
			// type is object

			// Check if object reference is not empty
			if (element.booleanize())
			{
				// element not empty
				// Check if the object is actually the resource type we want
				auto target = Object::cast_to<T>(element);

				if (target != nullptr && !target->is_class(T::get_class_static()))
				{
					UtilityFunctions::print("Unexpected element in Array!");
					allActionResource = false;
					break;
				}
				else if (target == nullptr)
				{
					// Shouldn't get here, but just in case
					UtilityFunctions::print("Unexpected element in Array!");
					allActionResource = false;
					break;
				}
			}
		}
	}
	return allActionResource;
}

#define MESSAGE_CLASS(t_resourceClass) #t_resourceClass
#define EXPECTED_TYPE_MESSAGE_PREFIX "Expected elements of this array are: "
#define EXPECTED_TYPE_MESSAGE_SUFFIX ". Did not set the target array value."
#define EXPECTED_TYPE_MESSAGE(t_resourceClass) EXPECTED_TYPE_MESSAGE_PREFIX MESSAGE_CLASS(t_resourceClass) EXPECTED_TYPE_MESSAGE_SUFFIX

#define MESSAGE_SUCCESS_PREFIX "Success: "
#define MESSAGE_TYPE_SUCCESS(t_resourceClass) MESSAGE_SUCCESS_PREFIX MESSAGE_CLASS(t_resourceClass)

#define ADD_RESOURCE_ARRAY(targetArray, value, t_resourceClass, m_clearMethod, m_addMethod)										\
if (CheckArrayForObjectType<t_resourceClass>(value))																			\
{																																\
	targetArray = value;																										\
																																\
	m_clearMethod();																											\
	for (int i = 0; i < value.size(); i++)																						\
	{																															\
		auto element = value[i];																								\
		if (element.get_type() != 0)																							\
		{																														\
			if (!element.booleanize())																							\
			{																													\
				UtilityFunctions::push_warning("Empty element.");																\
				continue;																										\
			}																													\
																																\
			auto target = Object::cast_to<t_resourceClass>(element);															\
			if (target != nullptr)																								\
			{																													\
				m_addMethod(target);																							\
				UtilityFunctions::print(MESSAGE_TYPE_SUCCESS(t_resourceClass));													\
			}																													\
			else																												\
			{																													\
				UtilityFunctions::push_error("Null ptr to Object.");															\
			}																													\
		}																														\
	}																															\
}																																\
else																															\
{																																\
	UtilityFunctions::push_error(EXPECTED_TYPE_MESSAGE(t_resourceClass));														\
}


#define BIND_PROPERTY_SIMPLE(p_propertyName, VariantType, t_bindingClassType, m_getter, m_setter, setterValueName)				\
ClassDB::bind_method(D_METHOD(#m_getter), &t_bindingClassType::m_getter);														\
ClassDB::bind_method(D_METHOD(#m_setter, #setterValueName), &t_bindingClassType::m_setter);										\
ADD_PROPERTY(PropertyInfo(VariantType, p_propertyName), #m_setter, #m_getter);

// This is an interface that helps target a specfic array type
// TTargetType must inherit from resource, TOwner must inherit from object
template <class TTargetType, class TOwner>
class TargetedArray
{
	Array m_array;
	std::vector<Ref<TTargetType>> m_refs;
	
	TOwner* m_owner;
	Callable m_callable;

public:
	TargetedArray(TOwner* owner, const StringName& method) : m_callable(owner, method)
	{
		m_owner = owner;
	}

	~TargetedArray() {}

	void set_array(const Array& value)
	{
		ADD_RESOURCE_ARRAY(m_array, value, TTargetType, InternalClear, InternalAddValue);
	}

	Array get_array() const
	{
		return m_array;
	}

	std::vector<Ref<TTargetType>> GetRefs()
	{
		return m_refs;
	}

	void AddValue(const Ref<TTargetType>& agentStat)
	{
		// This is a bit dumb, but I was having trouble converting ref classes. So I decided to reuse some of the utility code and simply create a variant in order to convert to a non const ptr.
		Variant variant = agentStat;

		if (!variant.booleanize())
		{
			UtilityFunctions::push_warning("Empty element.");
			return;
		}

		TTargetType* target = Object::cast_to<TTargetType>(variant);
		if (target != nullptr)
		{
			m_array.push_back(variant);
			InternalAddValue(target);
			UtilityFunctions::print(MESSAGE_TYPE_SUCCESS(TTargetType));
		}
		else
		{
			UtilityFunctions::push_error("Null ptr to Object.");
		}
	}

	void Clear()
	{
		m_array.clear();
		InternalClear();
	}

private:
	void InternalAddValue(TTargetType* agentStat)
	{
		Ref<TTargetType> targetRef = agentStat;
		m_refs.push_back(targetRef);
		if (targetRef.is_valid())
		{
			targetRef->connect("changed", m_callable);
		}

		OnAddValue(m_owner, targetRef);
	}

	void InternalClear()
	{
		for (int i = 0; i < m_refs.size(); i++)
		{
			if (m_refs[i].is_valid())
			{
				// Should be safe to disconnect here as the references are connected when they are added to this list.
				m_refs[i].ptr()->disconnect("changed", m_callable);
			}
		}
		m_refs.clear();
		OnClearTargetedArray(m_owner);
	}

public:
	virtual void OnClearTargetedArray(TOwner* owner) = 0;
	virtual void OnAddValue(TOwner* owner, Ref<TTargetType>& valueRef) = 0;

	// This function must be called during the owner's callable method that was used to bind the changed signal.
	void ProcessChange()
	{
		// There is no need to remove or change reference to resources. but the GOAPObject's internal Arrays may need to add or remove values.
		// To ensure the values are correct. Clear first.
		OnClearTargetedArray(m_owner);

		// Reassign the values from the resource refs
		for (int i = 0; i < m_refs.size(); i++)
		{
			OnAddValue(m_owner, m_refs[i]);
		}
	}
};

#endif // ! MY_UTILITY_H