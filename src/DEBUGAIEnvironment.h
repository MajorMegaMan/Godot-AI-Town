#ifndef DEBUG_AI_ENVIRONMENT_H
#define DEBUG_AI_ENVIRONMENT_H

#include "GOAPAI.h"
#include "DEBUGWorldKeys.h"
#include "SingletonTemplate.h"
#include <map>
#include <vector>

// For Vector3 class
#include <godot_cpp/core/object.hpp>

using namespace godot;
using namespace GOAP;

class TypeInterface
{
public:
	virtual int TypeID() const = 0;
};

// This is an example setup to find ActionTargets easier.
class AIActionTargetList : public PublicSingleton<AIActionTargetList>
{
	std::map<int, std::vector<GOAPAIActionTarget<Vector3>*>> m_targetsMap;

public:
	const std::vector<GOAPAIActionTarget<Vector3>*>* GetTargetList(int key)
	{
		if (m_targetsMap.count(key) > 0)
		{
			return &(m_targetsMap[key]);
		}
		else
		{
			return nullptr;
		}
	}

	void AddActionTarget(int key, GOAPAIActionTarget<Vector3>* actionTarget)
	{
		if (m_targetsMap.count(key) > 0)
		{
			if (!ListContains(m_targetsMap[key], actionTarget))
			{
				m_targetsMap[key].push_back(actionTarget);
			}
		}
		else
		{
			m_targetsMap.insert({ key, { actionTarget } });
		}
	}

	void RemoveActionTarget(int key, GOAPAIActionTarget<Vector3>* actionTarget)
	{
		if (m_targetsMap.count(key) > 0)
		{
			if (m_targetsMap[key].size() == 1)
			{
				m_targetsMap.erase(key);
			}
			else
			{
				RemoveFromList(m_targetsMap[key], actionTarget);
			}
		}
	}

private:
	bool ListContains(std::vector<GOAPAIActionTarget<Vector3>*>& list, GOAPAIActionTarget<Vector3>* target)
	{
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i] == target)
			{
				return true;
			}
		}
	}

	bool ListContains(std::vector<GOAPAIActionTarget<Vector3>*>& list, GOAPAIActionTarget<Vector3>* target, int& out_index)
	{
		out_index = -1;
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i] == target)
			{
				out_index = i;
				return true;
			}
		}
	}

	void RemoveFromList(std::vector<GOAPAIActionTarget<Vector3>*>& list, GOAPAIActionTarget<Vector3>* target)
	{
		int i = -1;
		if (!ListContains(list, target, i))
		{
			list.erase(list.begin() + i);
		}
	}

public:
	static const std::vector<GOAPAIActionTarget<Vector3>*>* get_target_list(int key)
	{
		return GetInstance().GetTargetList(key);
	}

	static void add_action_target(int key, GOAPAIActionTarget<Vector3>* actionTarget)
	{
		return GetInstance().AddActionTarget(key, actionTarget);
	}

	static void remove_action_target(int key, GOAPAIActionTarget<Vector3>* actionTarget)
	{
		return GetInstance().RemoveActionTarget(key, actionTarget);
	}
};

class AIWorld : public PublicSingleton<AIWorld>
{
	GOAPWorldState m_globalWorldState;

public:
	AIWorld()
	{
		m_globalWorldState.CreateValueType(FOODCOUNT, sizeof(int));
		m_globalWorldState.SetValue<int>(FOODCOUNT, 0);
	}

	static GOAPWorldState* GetWorldState()
	{
		return &(GetInstance().m_globalWorldState);
	}
};

#endif // ! DEBUG_AI_ENVIRONMENT_H