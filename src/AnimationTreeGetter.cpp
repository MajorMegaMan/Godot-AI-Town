#include "AnimationTreeGetter.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void AnimationTreeGetter::_bind_methods()
{
	// Methods.
	ClassDB::bind_method(D_METHOD("Start"), &AnimationTreeGetter::Start);
	ClassDB::bind_method(D_METHOD("Exit"), &AnimationTreeGetter::Exit);

	// Funcs.
	ClassDB::bind_method(D_METHOD("GetAnimationTree"), &AnimationTreeGetter::GetAnimationTree);

	// Properties.
	ClassDB::bind_method(D_METHOD("get_anim_tree_path"), &AnimationTreeGetter::get_anim_tree_path);
	ClassDB::bind_method(D_METHOD("set_anim_tree_path", "animPath"), &AnimationTreeGetter::set_anim_tree_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "anim_prop"), "set_anim_tree_path", "get_anim_tree_path");
}

AnimationTreeGetter::AnimationTreeGetter()
{
	
}

AnimationTreeGetter::~AnimationTreeGetter()
{
	
}

NodePath AnimationTreeGetter::get_anim_tree_path() const
{
	return m_animProp.GetNodePath();
}

void AnimationTreeGetter::set_anim_tree_path(const NodePath& animPath)
{
	m_animProp.SetNodePath(animPath);
	ProcessUpdate(0);
}

AnimationTree* AnimationTreeGetter::GetAnimationTree() const
{
	return m_animProp.GetNode();
}

void AnimationTreeGetter::OnStart()
{
	m_animProp.ApplyPath(*this);
}

void AnimationTreeGetter::OnUpdate(double delta)
{
	m_animProp.ApplyPath(*this);
}

void AnimationTreeGetter::OnExit()
{
}
