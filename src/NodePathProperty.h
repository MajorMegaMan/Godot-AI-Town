#ifndef NODEPATHPROPERTY_CLASS_H
#define NODEPATHPROPERTY_CLASS_H

#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/core/binder_common.hpp>

using namespace godot;

template<class T>
class NodePathProperty
{
	NodePath m_nodePath;
	T* m_node;

public:
	NodePathProperty()
	{
		m_node = nullptr;
	}

	~NodePathProperty() {}

private:
	void SetNode(T* node)
	{
		m_node = node;
	}

public:
	T* GetNode() const
	{
		return m_node;
	}

	void SetNodePath(const NodePath& nodePath)
	{
		m_nodePath = nodePath;
	}

	NodePath GetNodePath() const
	{
		return m_nodePath;
	}

	void ApplyPath(const Node& owner)
	{
		SetNode(owner.get_node<T>(m_nodePath));
	}
};

#endif // ! NODEPATHPROPERTY_CLASS_H