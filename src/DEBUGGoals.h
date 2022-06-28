#ifndef DEBUG_GOALS_H
#define DEBUG_GOALS_H

#include "DEBUGAIEnvironment.h"

using namespace GOAP;

template <class TInheritedClass>
class GoalSingleton : public GOAPGoal
{
public:
	static GOAPGoal* GetInstance()
	{
		static TInheritedClass instance;
		return &instance;
	}
};

// Goals
class StoreFoodGoal : public GoalSingleton<StoreFoodGoal>
{
public:
	StoreFoodGoal()
	{
		AddKey(FOODCOUNT);
	}

	bool PerformConditions(const GOAPWorldState& current, const GOAPWorldState& base) override
	{
		GOAPReadValue currentfoodCount = current.GetReadValue(FOODCOUNT);
		GOAPReadValue basefoodCount = base.GetReadValue(FOODCOUNT);
		int diff = currentfoodCount.GetInt() - basefoodCount.GetInt();
		return diff > 0;
	}
};

class FillHungerGoal : public GoalSingleton<FillHungerGoal>
{
public:
	FillHungerGoal()
	{
		AddKey(HUNGER);
	}

	bool PerformConditions(const GOAPWorldState& current, const GOAPWorldState& base) override
	{
		GOAPReadValue currentHunger = current.GetReadValue(HUNGER);
		return currentHunger.GetFloat() > 80.0f;
	}
};

#endif // ! DEBUG_GOALS_H