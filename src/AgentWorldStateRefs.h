#pragma once
#include "GOAPcore.h"

using namespace GOAP;

class AgentWorldStateRefs : public GOAPKeyList
{
	RedirectMap m_indexMap;

	std::vector<GOAPValue> m_valueArray;

public:
	void AddWorldState(GOAPWorldState& worldState)
	{
		auto keys = worldState.GetKeys();
		for (int i = 0; i < keys.size(); i++)
		{
			if (!m_indexMap.contains(keys[i]))
			{
				AddValueRef(keys[i], worldState.GetGOAPValue(keys[i]));
			}
		}
	}

	void AddValueRef(int key, GOAPValue value)
	{
		AddKey(key);
		m_indexMap.insert(key, m_valueArray.size());
		m_valueArray.push_back(value);
	}

	GOAPValue GetValueRef(int key)
	{
		return m_valueArray[m_indexMap[key]];
	}

	int GetInt(int key)
	{
		if (!m_indexMap.contains(key))
		{
			return 0;
		}
		return GetValueRef(key).GetInt();
	}

	bool GetBool(int key)
	{
		if (!m_indexMap.contains(key))
		{
			return false;
		}
		return GetValueRef(key).GetBool();
	}

	float GetFloat(int key)
	{
		if (!m_indexMap.contains(key))
		{
			return 0.0f;
		}
		return GetValueRef(key).GetFloat();
	}
};