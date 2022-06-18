#ifndef DEBUG_BEHAVIOURS_H
#define DEBUG_BEHAVIOURS_H

#include "DEBUGActions.h"
#include "DEBUGGoals.h"

template <class TInheritedClass>
class BehaviourSingleton : public GOAPAIBehaviour<Vector3>
{
public:
	static GOAPAIBehaviour* GetInstance()
	{
		static TInheritedClass instance;
		return &instance;
	}
};

class BehaviourFoodGatherer : public BehaviourSingleton<BehaviourFoodGatherer>
{
public:
	BehaviourFoodGatherer()
	{
		int keys[2];
		size_t typeSizes[2];
		{
			int i = 0;
			keys[i] = HUNGER;
			typeSizes[i++] = sizeof(float);
			keys[i] = HOLDITEM;
			typeSizes[i++] = sizeof(int);
		}

		GOAPWorldState baseValues;

		baseValues.CreateValueTypes(keys, typeSizes, 2);

		baseValues.SetValue(HUNGER, 100.0f);
		baseValues.SetValue(HOLDITEM, 0);

		SetBaseAgentValues(baseValues);

		AddAction(PickUpFoodAction::GetInstance());
		AddAction(StoreFoodAction::GetInstance());
		AddAction(DropItemAction::GetInstance());

		AddGoal(StoreFoodGoal::GetInstance());
	}

	~BehaviourFoodGatherer()
	{

	}

	GOAPGoal* FindGoal(const GOAPWorldState& worldState) override
	{
		return GetGoal(0);
	}
};

#endif // ! DEBUG_BEHAVIOURS_H