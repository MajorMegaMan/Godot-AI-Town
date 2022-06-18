#ifndef DEBUG_ACTIONS_H
#define DEBUG_ACTIONS_H

#include "DEBUGAIEnvironment.h"
#include <godot_cpp/variant/utility_functions.hpp>

class DEBUGAction : public GOAPAIAction<Vector3>
{
public:
	String name;

	static void SetHoldItem(GOAPWorldState& worldState, int item)
	{
		GOAPValue holdItem = worldState.GetGOAPValue(HOLDITEM);
		holdItem.SetValue(item);
	}

	static int GetHoldItem(const GOAPWorldState& worldState)
	{
		auto holdItem = worldState.GetReadValue(HOLDITEM);
		return holdItem.GetValue<int>();
	}

	static bool IsHolding(const GOAPWorldState& worldState, int item)
	{
		return item == GetHoldItem(worldState);
	}

	static void IncrementIntWithMinClamp(GOAPValue& intGoapValue, int min, int increment)
	{
		int result = intGoapValue.GetInt() + increment;
		if (result < min)
		{
			result = min;
		}
		intGoapValue.SetInt(result);
	}

	static float FindDistance(const GOAPAIActionTarget<Vector3>& origin, const GOAPAIActionTarget<Vector3>& destination)
	{
		Vector3 toDestination = destination.GetActionTargetPosition() - origin.GetActionTargetPosition();
		return toDestination.length();
	}

	static GOAPAIActionTarget<Vector3>* FindClosestActionTarget(ActionTargetKey key, const GOAPAIActionTarget<Vector3>& agent)
	{
		auto targetList = AIActionTargetList::get_target_list(key);
		GOAPAIActionTarget<Vector3>* closeTarget = nullptr;
		float lowestDistance = 0;
		if (targetList->size() > 0)
		{
			closeTarget = (*targetList)[0];
			lowestDistance = FindDistance(agent, *closeTarget);

			for (int i = 1; i < targetList->size(); i++)
			{
				auto potentialTarget = (*targetList)[i];
				float distance = FindDistance(agent, *potentialTarget);

				if (distance < lowestDistance)
				{
					closeTarget = potentialTarget;
					lowestDistance = distance;
				}
			}
		}
		return closeTarget;
	}
};

template <class TInheritedClass>
class SingletonDEBUGAction : public DEBUGAction
{
public:
	static GOAPAIAction* GetInstance()
	{
		static TInheritedClass instance;
		return &instance;
	}
};

class PickUpFoodAction : public SingletonDEBUGAction<PickUpFoodAction>
{
public:
	PickUpFoodAction()
	{
		AddKey(HOLDITEM);
		name = "Pick up Food.";
	}

	void PerformEffects(GOAPWorldState& worldState) override
	{
		DEBUGAction::SetHoldItem(worldState, 1);
	}

	bool CheckConditions(const GOAPWorldState& worldState) override
	{
		return DEBUGAction::IsHolding(worldState, 0);
	}

	GOAPAIActionTarget<Vector3>* FindActionTarget(const GOAPAIAgent<Vector3>& agent) override
	{
		auto foodItemTarget = FindClosestActionTarget(AT_FOOD_ITEM, agent);
		if (foodItemTarget != nullptr)
		{
			UtilityFunctions::print("Found Food Target.");
		}
		else
		{
			UtilityFunctions::print("Did not find Food Target.");
		}
		return foodItemTarget;
	}

	bool IsInRange(const GOAPAIAgent<Vector3>& agent, const GOAPAIActionTarget<Vector3>* actionTarget) const override
	{
		float distance = FindDistance(agent, *actionTarget);
		return distance < 0.5f;
	}
};

class StoreFoodAction : public SingletonDEBUGAction<StoreFoodAction>
{
public:
	StoreFoodAction()
	{
		AddKey(FOODCOUNT);
		AddKey(HOLDITEM);
		name = "Store Food";
	}

	void PerformEffects(GOAPWorldState& worldState) override
	{
		GOAPValue foodCount = worldState.GetGOAPValue(FOODCOUNT);
		foodCount.GetInt()++;
		DEBUGAction::SetHoldItem(worldState, 0);
	}

	bool CheckConditions(const GOAPWorldState& worldState) override
	{
		bool isHoldingFood = DEBUGAction::IsHolding(worldState, 1);

		return isHoldingFood;
	}

	GOAPAIActionTarget<Vector3>* FindActionTarget(const GOAPAIAgent<Vector3>& agent) override
	{
		return FindClosestActionTarget(AT_FOOD_STORAGE, agent);
	}

	bool IsInRange(const GOAPAIAgent<Vector3>& agent, const GOAPAIActionTarget<Vector3>* actionTarget) const override
	{
		float distance = FindDistance(agent, *actionTarget);
		return distance < 0.5f;
	}
};

class DropItemAction : public SingletonDEBUGAction<DropItemAction>
{
public:
	DropItemAction()
	{
		AddKey(HOLDITEM);
		name = "Drop Item.";
	}

	void PerformEffects(GOAPWorldState& worldState) override
	{
		DEBUGAction::SetHoldItem(worldState, 0);
	}

	bool CheckConditions(const GOAPWorldState& worldState) override
	{
		return !DEBUGAction::IsHolding(worldState, 0);
	}

	GOAPAIActionTarget<Vector3>* FindActionTarget(const GOAPAIAgent<Vector3>& agent) override
	{
		return nullptr;
	}

	bool IsInRange(const GOAPAIAgent<Vector3>& agent, const GOAPAIActionTarget<Vector3>* actionTarget) const override
	{
		return true;
	}
};

#endif // ! DEBUG_ACTIONS_H