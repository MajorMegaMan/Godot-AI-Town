#include "GameManager.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/world3D.hpp>

void GameManager::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("Start"), &GameManager::Start);
	ClassDB::bind_method(D_METHOD("Exit"), &GameManager::Exit);

	ClassDB::bind_method(D_METHOD("GetBehaviourResourceRef"), &GameManager::GetBehaviourResourceRef);
	ClassDB::bind_method(D_METHOD("SetBehaviourResourceRef", "behaviourResourceRef"), &GameManager::SetBehaviourResourceRef);
	
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "behaviour_resource_ref", PropertyHint::PROPERTY_HINT_RESOURCE_TYPE, "BehaviourResource"), "SetBehaviourResourceRef", "GetBehaviourResourceRef");
}

GameManager::GameManager()
{
	
}

GameManager::~GameManager()
{

}

void GameManager::SetBehaviourResourceRef(const Ref<BehaviourResource>& behaviourResourceRef)
{
	m_behaviourResourceRef = behaviourResourceRef;
}

Ref<BehaviourResource> GameManager::GetBehaviourResourceRef()
{
	return m_behaviourResourceRef;
}

BehaviourResource* GameManager::GetBehaviourResource()
{
	return m_behaviourResourceRef.ptr();
}

void GameManager::OnStart()
{
	UtilityFunctions::print("GameManager Start");
	if (!m_behaviourResourceRef.is_null())
	{
		String format_hunger = "Hunger: %s";
		String result_hunger = format_hunger % (String::num(GetBehaviourResource()->GetBaseAgentStatsResource()->GetBaseHunger()));
		String format_holdItem = "HoldItem: %s";
		String result_holdItem = format_holdItem % (String::num(GetBehaviourResource()->GetBaseAgentStatsResource()->GetBaseHoldItem()));

		UtilityFunctions::print(result_hunger);
		UtilityFunctions::print(result_holdItem);
	}
	else
	{
		UtilityFunctions::print("No Behaviour was loaded.");
	}
}

void GameManager::OnUpdate(double delta)
{

}

void GameManager::OnExit()
{

}
