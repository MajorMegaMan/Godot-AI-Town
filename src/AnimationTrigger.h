#pragma once

#include <godot_cpp/classes/animation_tree.hpp>
#include <vector>

class AnimationCondition
{
public:
	AnimationCondition() {}

	AnimationCondition(const godot::String& propertyPath)
	{
		this->propertyPath = propertyPath;
	}

	~AnimationCondition() {}

	godot::String propertyPath;

	void SetBool(godot::AnimationTree* animTree, bool value)
	{
		animTree->set(propertyPath, value);
	}
};

class AnimationTriggerList
{
	godot::AnimationTree* m_animTree;

	std::vector<AnimationCondition> m_triggers;
	std::vector<AnimationCondition*> m_toBeReset;

public:
	void SetAnimTree(godot::AnimationTree* animTree)
	{
		m_animTree = animTree;
	}

	void Reset()
	{
		for (int i = 0; i < m_toBeReset.size(); i++)
		{
			m_toBeReset[i]->SetBool(m_animTree, false);
		}
		m_toBeReset.clear();
	}

	void Trigger(int index)
	{
		m_triggers[index].SetBool(m_animTree, true);
		m_toBeReset.push_back(&m_triggers[index]);
	}

	size_t Size()
	{
		return m_triggers.size();
	}

	void AddCondition(const AnimationCondition& condition)
	{
		m_triggers.push_back(condition);
	}

	void AddCondition(const godot::String& propertyPath)
	{
		m_triggers.push_back(AnimationCondition(propertyPath));
	}
};