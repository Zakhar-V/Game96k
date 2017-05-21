#pragma once

#include "Scene.hpp"
#include "Timer.hpp"

//----------------------------------------------------------------------------//
// Component
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void Component::Enable(bool _newState)
{
	if (_newState)
		m_enabled = true;
	_Enable(_newState);
	if (!_newState)
		m_enabled = false;
}
//----------------------------------------------------------------------------//
bool Component::IsEnabled(void)
{
	return m_enabled && m_entity->IsEnabled();
}
//----------------------------------------------------------------------------//
void Component::Destroy(void)
{
	ASSERT(m_entity != nullptr);
	m_entity->RemoveComponent(this);
}
//----------------------------------------------------------------------------//
uint Component::GetTag(void)
{
	return m_entity->GetTag();
}
//----------------------------------------------------------------------------//
bool Component::HasTag(uint _mask)
{
	return m_entity->HasTag(_mask);
}
//----------------------------------------------------------------------------//
bool Component::MatchTag(uint _mask)
{
	return m_entity->MatchTag(_mask);
}
//----------------------------------------------------------------------------//
uint Component::GetLayer(void)
{
	return m_entity->GetLayer();
}
//----------------------------------------------------------------------------//
bool Component::HasLayer(uint _mask)
{
	return m_entity->HasLayer(_mask);
}
//----------------------------------------------------------------------------//
bool Component::MatchLayer(uint _mask)
{
	return m_entity->MatchLayer(_mask);
}
//----------------------------------------------------------------------------//
Component* Component::GetNextComponent(uint _type)
{
	for (Component* i = m_nextComponent; i; i = i->m_nextComponent)
	{
		if (i->IsTypeOf(_type))
			return i;
	}
	return nullptr;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Entity
//----------------------------------------------------------------------------//

Entity* Entity::s_root = nullptr;

//----------------------------------------------------------------------------//
Entity::Entity(void)
{
}
//----------------------------------------------------------------------------//
Entity::~Entity(void)
{
	ASSERT(m_inScene == false);

	while (m_components)
		RemoveComponent(m_components);

	while (m_child)
		m_child->SetParent(nullptr, false);
}
//----------------------------------------------------------------------------//
void Entity::Instantiate(void)
{
	if (!m_inScene && !m_parent)
	{
		AddRef();
		_AddToScene(true);
	}
	DEBUG_CODE(else if (m_parent) LOG("Non-root entity cannot be instantiated"));
}
//----------------------------------------------------------------------------//
void Entity::Destroy(void)
{
	SetParent(nullptr, false);
	if (m_inScene)
	{
		_AddToScene(false);
		Release();
	}
}
//----------------------------------------------------------------------------//
void Entity::_AddToScene(bool _newState)
{
	ASSERT(m_inScene != _newState);
	ASSERT(!m_parent || m_parent->m_inScene == _newState);

	if (!m_parent)
	{
		if (_newState)
			Link(s_root, this, m_prev);
		else
			Unlink(s_root, this, m_prev);
	}

	if (_newState)
		m_inScene = true;
	_Register(_newState);

	for (Component* i = m_components; i; i = i->m_nextComponent)
		i->_Register(_newState);

	if(!_newState)
		m_inScene = false;

	for (Entity* i = m_child; i; i = i->m_next)
		i->_AddToScene(_newState);
}
//----------------------------------------------------------------------------//
void Entity::SetLayer(uint _layer, bool _recursive)
{
	m_layerMask = _layer;
	for (Entity* i = m_child; i; i = i->m_next)
		i->SetLayer(_layer, true);
}
//----------------------------------------------------------------------------//
void Entity::AddLayer(uint _layer, bool _recursive)
{
	m_layerMask |= _layer;
	for (Entity* i = m_child; i; i = i->m_next)
		i->SetLayer(_layer, true);
}
//----------------------------------------------------------------------------//
void Entity::RemoveLayer(uint _layer, bool _recursive)
{
	m_layerMask &= ~_layer;
	for (Entity* i = m_child; i; i = i->m_next)
		i->SetLayer(_layer, true);
}
//----------------------------------------------------------------------------//
void Entity::SetParent(Entity* _parent, bool _keepWorldTransform)
{
	if (m_parent == _parent)
		return;
	for (Entity* i = _parent; i; i = i->m_parent)
	{
		if (i == this)
			return;	// child entity cannot be parent
	}


	if (!m_parent && !m_inScene)
		AddRef();

	Matrix44 _worldTM;
	if(_keepWorldTransform)
		_worldTM = GetWorldTransform();

	if (m_parent)
		Unlink(m_parent->m_child, this, m_prev);
	else if (m_inScene)
		Unlink(s_root, this, m_prev);

	_OnChangeParent(_parent);
	m_parent = _parent;

	if (m_parent)
	{
		Link(m_parent->m_child, this, m_prev);

		if(m_parent->m_inScene != m_inScene)
			_AddToScene(m_parent->m_inScene);
	}
	else if (m_inScene)
	{
		Link(s_root, this, m_prev);
	}

	if(_keepWorldTransform)
		SetWorldTransform(_worldTM);

	_OnParentEnable(m_parent ? m_parent->IsEnabled() : m_enabledSelf);

	if (!m_parent && !m_inScene)
		Release();
}
//----------------------------------------------------------------------------//
Entity* Entity::GetParent(uint _type)
{
	for (Entity* i = m_parent; i; i = i->m_parent)
	{
		if (i->IsTypeOf(_type))
			return i;
	}
	return nullptr;
}
//----------------------------------------------------------------------------//
Entity* Entity::AddChild(uint _type)
{
	TypeInfo* _typeInfo = gReflection->GetTypeInfo(_type);
	if (_typeInfo)
	{
		EntityPtr _child = _typeInfo->Factory().Cast<Entity>();
		_child->SetParent(this, false);
		return _child;
	}
	return nullptr;
}
//----------------------------------------------------------------------------//
void Entity::_OnParentEnable(bool _enabled)
{
	if (m_parentEnabled != _enabled)
	{
		bool _current = IsEnabled();
		_enabled = _enabled && m_enabledSelf;
		m_parentEnabled = _enabled;
		if (_enabled != _current)
		{
			for (Entity* i = m_child; i; i = i->m_next)
				i->_OnParentEnable(_enabled);
		}
	}
}
//----------------------------------------------------------------------------//
void Entity::_EnableSelf(bool _enabled)
{
	if (m_enabledSelf != _enabled)
	{
		bool _current = IsEnabled();
		_enabled = _enabled && m_parentEnabled;
		m_enabledSelf = _enabled;
		if (_enabled != _current)
		{
			for (Entity* i = m_child; i; i = i->m_next)
				i->_OnParentEnable(_enabled);
		}
	}
}
//----------------------------------------------------------------------------//
const Matrix44& Entity::GetWorldTransform(void)
{
	_UpdateTransform();
	return m_worldTransform;
}
//----------------------------------------------------------------------------//
void Entity::SetWorldTransform(const Matrix44& _tm)
{
	SetWorldPosition(_tm.GetTranslation());
	SetWorldRotation(_tm.GetRotation());
	SetWorldScale(_tm.GetScale());
}
//----------------------------------------------------------------------------//
void Entity::SetLocalPosition(const Vector3& _position)
{
	m_localPosition = _position;
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
const Vector3& Entity::GetLocalPosition(void)
{
	return m_localPosition;
}
//----------------------------------------------------------------------------//
void Entity::SetLocalRotation(const Quaternion& _rotation)
{
	m_localRotation = _rotation;
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
const Quaternion& Entity::GetLocalRotation(void)
{
	return m_localRotation;
}
//----------------------------------------------------------------------------//
void Entity::SetLocalScale(const Vector3& _scale)
{
	m_localScale = _scale;
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
const Vector3& Entity::GetLocalScale(void)
{
	return m_localScale;
}
//----------------------------------------------------------------------------//
void Entity::SetWorldPosition(const Vector3& _position)
{
	m_localPosition = _position;
	if (m_inheritTransform && m_parent)
		m_localPosition = m_parent->GetWorldTransform().Copy().Inverse().Transform(m_localPosition);
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
Vector3 Entity::GetWorldPosition(void)
{
	_UpdateTransform();
	return m_worldTransform.GetTranslation();
}
//----------------------------------------------------------------------------//
void Entity::SetWorldRotation(const Quaternion& _rotation)
{
	m_localRotation = _rotation;
	if (m_inheritTransform && m_parent)
		m_localRotation = m_parent->GetWorldRotation().Copy().Inverse() * m_localRotation;
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
const Quaternion& Entity::GetWorldRotation(void)
{
	_UpdateTransform();
	return m_worldRotation;
}
//----------------------------------------------------------------------------//
void Entity::SetWorldScale(const Vector3& _scale)
{
	m_localScale = _scale;
	if (m_inheritTransform && m_parent)
		m_localScale /= m_parent->GetWorldTransform().GetScale();
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
Vector3 Entity::GetWorldScale(void)
{
	_UpdateTransform();
	return m_worldTransform.GetScale();
}
//----------------------------------------------------------------------------//
void Entity::SetInheritTransform(bool _enabled)
{
	if (m_inheritTransform != _enabled)
	{
		Matrix44 _tm = GetWorldTransform();
		m_inheritTransform = _enabled;
		SetWorldTransform(_tm);
	}
}
//----------------------------------------------------------------------------//
void Entity::_InvalidateTransform(void)
{
	if (m_transformUpdated)
	{
		m_transformUpdated = false;

		_OnTransformChanged();

		if (m_inScene)
		{
			for (Component* i = m_components; i; i = i->m_nextComponent)
			{
				if(i->m_eventMask & Component::Transforms)
					i->_OnTransformChanged();
			}
		}

		for (Entity* i = m_child; i; i = i->m_next)
			i->_InvalidateTransform();
	}
}
//----------------------------------------------------------------------------//
void Entity::_UpdateTransform(void)
{
	if (!m_transformUpdated)
	{
		_OnTransformUpdate();

		m_worldTransform.CreateTransform(m_localPosition, m_localRotation, m_localScale);

		if (m_parent && m_inheritTransform)
			m_worldTransform = m_parent->GetWorldTransform() * m_worldTransform;

		m_worldRotation = m_worldTransform.GetRotation();
		m_transformUpdated = true;
		_OnTransformUpdated();
	}
}
//----------------------------------------------------------------------------//
Component* Entity::GetComponent(uint _type)
{
	for (Component* i = m_components; i; i = i->m_nextComponent)
	{
		if (i->IsTypeOf(_type))
			return i;
	}
	return nullptr;
}
//----------------------------------------------------------------------------//
Component* Entity::AddComponent(uint _type)
{
	TypeInfo* _typeInfo = gReflection->GetTypeInfo(_type);
	if (_typeInfo)
	{
		if (_typeInfo->HasAnyOfFlags(Component::Single))
		{
			Component* _component = GetComponent(_type);
			if (_component)
				return _component;
		}

		ASSERT(_typeInfo->Factory != nullptr);
		ComponentPtr _component = _typeInfo->Factory().Cast<Component>();
		ASSERT(((Object*)_component)->IsTypeOf<Component>());

		_component->AddRef();
		_component->m_entity = this;
		Link<Component>(m_components, _component, _component->m_prevComponent);

		if (m_inScene)
			_component->_Register(true);

		return _component;
	}
	return nullptr;
}
//----------------------------------------------------------------------------//
void Entity::RemoveComponent(Component* _component)
{
	if(_component && _component->m_entity == this)
	{
		if (m_inScene)
			_component->_Register(false);

		Unlink(m_components, _component, _component->m_prevComponent);
		_component->m_entity = nullptr;
		_component->Release();
	}
}
//----------------------------------------------------------------------------//
void Entity::_UpdateRecursive(void)
{
	for (Component* i = m_components; i;)
	{
		Component* _next = i->m_nextComponent;
		if (i->m_eventMask & Component::Updates)
			i->_Update();
		i = _next;
	}

	_Update();

	for (Entity* i = m_child; i;)
	{
		Entity* _next = i->m_next;
		i->_UpdateRecursive();
		i = _next;
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Scene::Scene(void)
{
	LOG_NODE("Create Scene manager");

	gReflection->GetOrCreateTypeInfo<Entity>()->SetFactory([]() { return ObjectPtr(new Entity); });
}
//----------------------------------------------------------------------------//
Scene::~Scene(void)
{
	LOG_NODE("Destroy Scene manager");
}
//----------------------------------------------------------------------------//
EventResult Scene::_OnEvent(int _type, void* _data)
{
	switch (_type)
	{
	case SystemEvent::PreUpdate:
	{
	}
	break;

	case SystemEvent::Update:
	{
		for (Entity* _entity = Entity::s_root; _entity;)
		{
			Entity* _next = _entity->m_next;
			_entity->_UpdateRecursive();
			_entity = _next;
		}

	} break;

	case SystemEvent::Stop: // temp
	{
		while (Entity::s_root)
		{
			Entity::s_root->Destroy();
		}

	} break;
	}

	return ER_Pass;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// SceneSystem
//----------------------------------------------------------------------------//

SceneSystem* SceneSystem::s_firstSceneSystem = nullptr;
SceneSystem* SceneSystem::s_lastSceneSystem = nullptr;

//----------------------------------------------------------------------------//
SceneSystem::SceneSystem(void)
{
	LinkLast(s_firstSceneSystem, s_lastSceneSystem, this, m_prevSceneSystem);
}
//----------------------------------------------------------------------------//
SceneSystem::~SceneSystem(void)
{
	Unlink(s_firstSceneSystem, s_lastSceneSystem, this, m_prevSceneSystem);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
