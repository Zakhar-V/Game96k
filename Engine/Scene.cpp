#pragma once

#include "Scene.hpp"

//----------------------------------------------------------------------------//
// Component
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
// Entity
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Entity::Entity(void)
{
	
}
//----------------------------------------------------------------------------//
Entity::~Entity(void)
{
	ASSERT(m_scene == nullptr);

	while (m_components)
	{
		Component* _component = m_components;
		Unlink(m_components, _component, _component->m_prev);
		_component->Release();
	}

	while (m_child)
		m_child->SetParent(nullptr, false);
}
//----------------------------------------------------------------------------//
bool Entity::IsInParentHierarchy(Entity* _parent)
{
	if (_parent != nullptr)
	{
		for (Entity* _test = m_parent; _test; _test = _test->m_parent)
		{
			if (_test == _parent)
				return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------//
bool Entity::IsInChildHierarchy(Entity* _child)
{
	return _child && _child->IsInParentHierarchy(this);
}
//----------------------------------------------------------------------------//
bool Entity::SetParent(Entity* _parent, bool _keepWorldTransform)
{
	if (m_parent == _parent)
		return true;

	if (_parent == this || IsInChildHierarchy(_parent))
		return false;


	if (!m_parent && !m_scene)
		AddRef();

	Matrix44 _worldTM;
	if(_keepWorldTransform)
		_worldTM = GetWorldTransform();

	if (m_parent)
	{
		Unlink(m_parent->m_child, this, m_prev);
	}
	else if (m_scene)
	{
		m_scene->_RemoveRootEntity(this);
	}

	m_parent = _parent;

	if (m_parent)
	{
		Link(m_parent->m_child, this, m_prev);
		if(m_parent->m_scene != m_scene)
			_SetScene(m_parent->m_scene);
	}
	else if (m_scene)
	{
		m_scene->_AddRootEntity(this);
	}

	if(_keepWorldTransform)
		SetWorldTransform(_worldTM);

	if (!m_parent && !m_scene)
		Release();

	return true;
}
//----------------------------------------------------------------------------//
void Entity::_SetScene(Scene* _scene)
{
	ASSERT(m_scene != _scene);
	ASSERT(!m_parent || m_parent->m_scene == _scene);

	if (!m_parent && !m_scene)
		AddRef();

	if (m_scene)
	{
		if (!m_parent)
			m_scene->_RemoveRootEntity(this);
	}

	Scene* _oldScene = m_scene;
	m_scene = _scene;

	if (m_scene)
	{
		if(!m_parent)
			m_scene->_AddRootEntity(this);
	}

	bool _isManaged = _oldScene && _oldScene->IsManaged();
	if (m_scene && m_scene->IsManaged())
	{
		if (!_isManaged)
			_RegisterComponents();
	}
	else if (_isManaged)
	{
		_UnregisterComponents();
	}

	for (Entity* i = m_child; i; i = i->m_next)
	{
		i->_SetScene(m_scene);
	}

	if (!m_parent && !m_scene)
		Release();
}
//----------------------------------------------------------------------------//
Component* Entity::GetComponent(uint _type)
{
	for (Component* i = m_components; i; i = i->m_next)
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

		_component->AddRef();
		_component->m_entity = this;
		Link<Component>(m_components, _component, _component->m_prev);

		if (m_scene && m_scene->IsManaged())
			_RegisterComponent(_component);

		return _component;
	}
	return nullptr;
}
//----------------------------------------------------------------------------//
void Entity::_RegisterComponent(Component* _component)
{
	ASSERT(_component && _component->m_entity == this);

	_component->_Register();
}
//----------------------------------------------------------------------------//
void Entity::_RegisterComponents(void)
{
	for (Component* i = m_components; i; i = i->m_next)
		_RegisterComponent(i);
}
//----------------------------------------------------------------------------//
void Entity::_UnregisterComponent(Component* _component)
{
	ASSERT(_component && _component->m_entity == this);

	_component->_Unregister();
}
//----------------------------------------------------------------------------//
void Entity::_UnregisterComponents(void)
{
	for (Component* i = m_components; i; i = i->m_next)
		_UnregisterComponent(i);
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
	if (m_inheritPosition && m_parent)
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
	if (m_inheritRotation && m_parent)
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
	if (m_inheritScale && m_parent)
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
void Entity::SetInheritPosition(bool _enabled)
{
	if (m_inheritPosition != _enabled)
	{
		Vector3 _pos = GetWorldPosition();
		m_inheritPosition = _enabled;
		SetWorldPosition(_pos);
	}
}
//----------------------------------------------------------------------------//
void Entity::SetInheritRotation(bool _enabled)
{
	if (m_inheritRotation != _enabled)
	{
		Quaternion _rot = GetWorldRotation();
		m_inheritRotation = _enabled;
		SetWorldRotation(_rot);
	}
}
//----------------------------------------------------------------------------//
void Entity::SetInheritScale(bool _enabled)
{
	if (m_inheritScale != _enabled)
	{
		Vector3 _scl = GetWorldScale();
		m_inheritScale = _enabled;
		SetWorldScale(_scl);
	}
}
//----------------------------------------------------------------------------//
void Entity::_InvalidateTransform(void)
{
	if (m_transformUpdated)
	{
		//WakeUp();
		m_transformUpdated = false;
		m_transformChanged = true;
		//_InvalidateWorldBBox();
		for (Entity* i = m_child; i; i = i->m_next) // invalidate and activate all children
			i->_InvalidateTransform();
	}
}
//----------------------------------------------------------------------------//
void Entity::_UpdateTransform(void)
{
	if (!m_transformUpdated)
	{
		Vector3 _pos = m_localPosition;
		Quaternion _rot = m_localRotation;
		Vector3 _scl = m_localScale;

		if (m_parent)
		{
			if (m_inheritPosition)
				_pos = m_parent->GetWorldTransform().Transform(_pos);
			if (m_inheritRotation)
				_rot = m_parent->GetWorldRotation() * _rot;
			if (m_inheritScale)
				_scl *= m_parent->GetWorldScale();
		}

		m_worldTransform.CreateTransform(_pos, _rot, _scl);
		m_worldRotation = m_worldTransform.GetRotation();
		m_transformUpdated = true;
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Entity* Scene::AddEntity(void)
{
	EntityPtr _entity = new Entity;
	_entity->_SetScene(this);
	return _entity;
}
//----------------------------------------------------------------------------//
bool Scene::AddEntity(Entity* _entity)
{
	if (!_entity)
			return false;
	if (_entity->m_parent)
	{
		LOG("Unable to add non-root entity");
		return false;
	}
	if (_entity->m_scene == this) // already attached
		return true;

	_entity->_SetScene(this);

	return true;
}
//----------------------------------------------------------------------------//
bool Scene::RemoveEntity(Entity* _entity)
{
	if (!_entity || _entity->m_scene != this)
		return false;

	_entity->SetParent(nullptr);
	_entity->_SetScene(nullptr);

	return true;
}
//----------------------------------------------------------------------------//
void Scene::Enable(bool _enable)
{

}
//----------------------------------------------------------------------------//
void Scene::_AddRootEntity(Entity* _entity)
{
	Link(m_rootEntities, _entity, _entity->m_prev);
}
//----------------------------------------------------------------------------//
void Scene::_RemoveRootEntity(Entity* _entity)
{
	Unlink(m_rootEntities, _entity, _entity->m_prev);
}
//----------------------------------------------------------------------------//
void Scene::_Register(void)
{
	ASSERT(m_inSystem == false);

	m_inSystem = true;
	for (Entity* i = m_rootEntities; i; i = i->m_next)
	{
		i->_RegisterComponents();
	}
}
//----------------------------------------------------------------------------//
void Scene::_Unregister(void)
{
	for (Entity* i = m_rootEntities; i; i = i->m_next)
	{
		i->_UnregisterComponents();
	}
	m_inSystem = false;
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
	Link(s_firstSceneSystem, s_lastSceneSystem, this, m_prevSceneSystem);
}
//----------------------------------------------------------------------------//
SceneSystem::~SceneSystem(void)
{
	Unlink(s_firstSceneSystem, s_lastSceneSystem, this, m_prevSceneSystem);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// SceneManager
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
SceneManager::SceneManager(void)
{
	LOG_NODE("Create SceneManager");
}
//----------------------------------------------------------------------------//
SceneManager::~SceneManager(void)
{
	LOG_NODE("Destroy SceneManager");
}
//----------------------------------------------------------------------------//
void SceneManager::AddScene(Scene* _scene)
{
	if (!_scene || _scene->m_inSystem)
		return;

	_scene->AddRef();
	Link(m_scenes, _scene, _scene->m_prev);
	_scene->_Register();
}
//----------------------------------------------------------------------------//
EventResult SceneManager::_OnEvent(int _type, void* _data)
{
	switch (_type)
	{
	case ET_BeginFrame:
	{

	} break;

	case ET_EndFrame:
	{
		/*gEntitySystem->_RegisterComponents();
		gEntitySystem->_UnregisterComponents();
		gEntitySystem->_DestroyEntities();*/

	} break;
	}

	return ER_Pass;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
