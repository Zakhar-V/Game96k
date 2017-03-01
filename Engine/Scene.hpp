#pragma once

#include "System.hpp"
#include "Math.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

typedef SharedPtr<class Component> ComponentPtr;
typedef SharedPtr<class Entity> EntityPtr;
typedef SharedPtr<class Scene> ScenePtr;

//----------------------------------------------------------------------------//
// Component
//----------------------------------------------------------------------------//

class Component : public Object
{
public:
	RTTI("Component");

	enum Flags
	{
		Single = 0x1,
		//NonRemovable = 0x2,
	};

	virtual bool IsRemovable(void) { return true; }

protected:
	friend class Entity;
	friend class EntitySystem;

	//virtual void _OnAddComponent(Component* _component) { }
	//virtual void _OnRemoveComponent(Component* _component) { }

	virtual void _Register(void) {  }
	virtual void _Unregister(void) { }

	Entity*	m_entity = nullptr;
	Component* m_prev = nullptr;
	Component* m_next = nullptr;
};

//----------------------------------------------------------------------------//
// Entity
//----------------------------------------------------------------------------//

class Entity : public Object
{
public:
	RTTI("Entity");

	Entity(void);
	~Entity(void);
	
	// [hierarchy]

	bool IsInParentHierarchy(Entity* _parent);
	bool IsInChildHierarchy(Entity* _child);
	bool SetParent(Entity* _parent, bool _keepWorldTransform = true);
	Entity* GetParent(void) { return m_parent; }
  
	// [component]

	Component* GetComponent(uint _type);
	Component* AddComponent(uint _type);
	template <class T> T* AddComponent(void) 
	{
		T* _component = static_cast<T*>(AddComponent(T::Type));
		DEBUG_CODE(if (!_component) LOG("Unknown %s component", T::TypeName));
		return _component;
	}

	// [transform]

	Vector3 LocalToWorld(const Vector3& _v) { return GetWorldTransform().Transform(_v); }
	Vector3 WorldToLocal(const Vector3& _v) { return GetWorldTransform().Copy().Inverse().Transform(_v); }

	const Matrix44& GetWorldTransform(void);
	void SetWorldTransform(const Matrix44& _tm);
	void SetLocalPosition(const Vector3& _position);
	const Vector3& GetLocalPosition(void);
	void SetLocalRotation(const Quaternion& _rotation);
	const Quaternion& GetLocalRotation(void);
	void SetLocalScale(const Vector3& _scale);
	const Vector3& GetLocalScale(void);
	void SetWorldPosition(const Vector3& _position);
	Vector3 GetWorldPosition(void);
	void SetWorldRotation(const Quaternion& _rotation);
	const Quaternion& GetWorldRotation(void);
	void SetWorldScale(const Vector3& _scale);
	Vector3 GetWorldScale(void);

	Vector3 GetWorldDirection(void) { return -Vector3::UnitZ * GetWorldRotation(); }
	void SetWorldDirection(const Vector3& _dir) { SetWorldRotation(Quaternion().FromRotationTo(-Vector3::UnitZ, _dir)); }

	void SetInheritPosition(bool _enabled = true);
	bool GetInheritPosition(void) { return m_inheritPosition; }
	void SetInheritRotation(bool _enabled = true);
	bool GetInheritRotation(void) { return m_inheritRotation; }
	void SetInheritScale(bool _enabled = true);
	bool GetInheritScale(void) { return m_inheritScale; }

protected:
	friend class Scene;

	void _SetScene(Scene* _scene);

	void _RegisterComponent(Component* _component);
	void _RegisterComponents(void);
	void _UnregisterComponent(Component* _component);
	void _UnregisterComponents(void);

	void _InvalidateTransform(void);
	void _UpdateTransform(void);


	Scene* m_scene = nullptr;
	Entity* m_parent = nullptr;
	Entity* m_child = nullptr;
	Entity* m_prev = nullptr;
	Entity* m_next = nullptr;

	Component* m_components = nullptr;

	Vector3 m_localPosition = Vector3::Zero;
	Quaternion m_localRotation = Quaternion::Identity;
	Vector3 m_localScale = Vector3::One;
	Matrix44 m_worldTransform = Matrix44::Identity;
	Quaternion m_worldRotation = Quaternion::Identity;

	bool m_transformUpdated = true;
	bool m_transformChanged = false;

	bool m_inheritPosition = true;
	bool m_inheritRotation = true;
	bool m_inheritScale = true;
};

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

class Scene	: public Object
{
public:

	bool IsManaged(void) { return m_inSystem; }

	Entity* AddEntity(void);
	bool AddEntity(Entity* _entity);
	bool RemoveEntity(Entity* _entity);

	void Enable(bool _enable);

protected:
	friend class Entity;
	friend class SceneManager;

	void _Register(void);
	void _Unregister(void);

	void _AddRootEntity(Entity* _entity);
	void _RemoveRootEntity(Entity* _entity);


	bool m_inSystem = false;
	Entity* m_rootEntities = nullptr;

	bool m_enabled = true;

	Scene* m_prev = nullptr;
	Scene* m_next = nullptr;
};

//----------------------------------------------------------------------------//
// SceneSystem
//----------------------------------------------------------------------------//

class SceneSystem : public System
{
public:
	SceneSystem(void);
	~SceneSystem(void);

	static SceneSystem* GetFirst(void) { return s_firstSceneSystem; }
	SceneSystem* GetNext(void) { return m_nextSceneSystem; }

protected:

	SceneSystem* m_prevSceneSystem = nullptr;
	SceneSystem* m_nextSceneSystem = nullptr;


	static SceneSystem* s_firstSceneSystem;
	static SceneSystem* s_lastSceneSystem;
};

//----------------------------------------------------------------------------//
// SceneManager
//----------------------------------------------------------------------------//

#define gSceneManager SceneManager::Get()

class SceneManager : public System, public Singleton<SceneManager>
{
public:
	SceneManager(void);
	~SceneManager(void);

	void AddScene(Scene* _scene);

protected:
	friend class Entity;

	EventResult _OnEvent(int _type, void* _data) override;

	Scene* m_scenes = nullptr;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
