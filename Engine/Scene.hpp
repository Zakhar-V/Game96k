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

	enum EventMask : uint16
	{
		Updates = 0x1,
		Transforms = 0x2,
		//Components = 0x4,
	};

	void Enable(bool _newState);
	bool IsEnabledSelf(void) { return m_enabled; }
	bool IsEnabled(void);

	void Destroy(void);

	uint GetTag(void);
	bool HasTag(uint _mask);
	bool MatchTag(uint _mask);

	uint GetLayer(void);
	bool HasLayer(uint _mask);
	bool MatchLayer(uint _mask);

	Entity* GetEntity(void) { return m_entity; }
	Component* GetNextComponent(void) { return m_nextComponent; }
	Component* GetNextComponent(uint _type);
	template <class T> T* GetNextComponent(void) { return static_cast<T*>(GetNextComponent(T::TypeID)); }

protected:
	friend class Entity;

	virtual void _Register(bool _newState) { }
	virtual void _Enable(bool _newState) { }
	virtual void _Update(void) { }
	virtual void _OnTransformChanged(void) { }

	Entity* m_entity = nullptr;
	Component* m_prevComponent = nullptr;
	Component* m_nextComponent = nullptr;
	bool m_enabled = true;
	uint16 m_eventMask = 0;	//!< do set combination of EventMask for events handling
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

	// [lifetime]

	bool IsInScene(void) { return m_inScene; }
	void Instantiate(void); //!< add to scene \note has dependency from state of parent entity
	void Destroy(void); //!< remove from scene and parent

	// [tags]

	uint GetTag(void) { return m_tags; }
	bool HasTag(uint _mask) { return (m_tags & _mask) != 0; }
	bool MatchTag(uint _mask) { return (m_tags & _mask) == _mask; }
	void SetTag(uint _tag) { m_tags = _tag; }
	void AddTag(uint _tag) { m_tags |= _tag; }
	void RemoveTag(uint _tag) { m_tags &= ~_tag; }

	// [layers]
	uint GetLayer(void) { return m_layerMask; }
	bool HasLayer(uint _mask) { return (m_layerMask & _mask) != 0; }
	bool MatchLayer(uint _mask) { return (m_layerMask & _mask) == _mask; }
	void SetLayer(uint _layer, bool _recursive = true);
	void AddLayer(uint _layer, bool _recursive = true);
	void RemoveLayer(uint _layer, bool _recursive = true);

	// [hierarchy]

	static Entity* GetRoot(void) { return s_root; }
	void SetParent(Entity* _parent, bool _keepWorldTransform = true);
	Entity* GetParent(void) { return m_parent; }
	Entity* GetParent(uint _type = Entity::TypeID);
	template <class T> T* GetParent(void) { return static_cast<T*>(GetParent(T::TypeID)); }
	Entity* GetNext(void) { return m_next; }
	Entity* GetChild(void) { return m_child; }
	Entity* AddChild(uint _type = Entity::TypeID);
	template <class T> T* AddChild(void)
	{
		T* _child = static_cast<T*>(AddChild(T::TypeID));
		DEBUG_CODE(if (!_child) LOG("Unknown %s entity", T::TypeName));
		return _child;
	}

	// [enable]

	void Enable(bool _enabled = true) { _EnableSelf(_enabled); }
	bool IsEnabled(void) { return m_enabledSelf && m_parentEnabled; }
	bool IsEnabledSelf(void) { return m_enabledSelf; }

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

	void SetInheritTransform(bool _enabled = true);
	bool GetInheritTransform(void) { return m_inheritTransform; }

	// [component]

	Component* GetComponent(uint _type);
	template <class T> T* GetComponent(void) { return static_cast<T*>(GetComponent(T::TypeID)); }

	Component* AddComponent(uint _type);
	template <class T> T* AddComponent(void)
	{
		T* _component = static_cast<T*>(AddComponent(T::TypeID));
		DEBUG_CODE(if (!_component) LOG("Unknown %s component", T::TypeName));
		return _component;
	}

	void RemoveComponent(Component* _component);
	template <class T> bool RemoveComponent(void)
	{
		T* _component = GetComponent<T>();
		if (_component)
		{
			RemoveComponent(_component);
			return true;
		}
		return false;
	}

protected:
	friend class Scene;

	void _AddToScene(bool _newState);

	void _OnParentEnable(bool _enabled);
	void _EnableSelf(bool _enabled);

	void _InvalidateTransform(void);
	void _UpdateTransform(void);

	void _UpdateRecursive(void);

	virtual void _Register(bool _newState) { }
	virtual void _OnChangeParent(Entity* _newParent) { }
	virtual void _OnTransformChanged(void) { }
	virtual void _OnTransformUpdate(void) { }
	virtual void _OnTransformUpdated(void) { }
	virtual void _Update(void) { }

	bool m_inScene = false;

	bool m_enabledSelf = true;
	bool m_parentEnabled = true;

	uint m_tags = 0x1;
	uint m_layerMask = 0x1;

	Entity* m_parent = nullptr;
	Entity* m_prev = nullptr;
	Entity* m_next = nullptr;
	Entity* m_child = nullptr;

	Vector3 m_localPosition = Vector3::Zero;
	Quaternion m_localRotation = Quaternion::Identity;
	Vector3 m_localScale = Vector3::One;
	Matrix44 m_worldTransform = Matrix44::Identity;
	Quaternion m_worldRotation = Quaternion::Identity;

	bool m_transformUpdated = true;
	bool m_inheritTransform = true;

	Component* m_components = nullptr;

	static Entity* s_root;
};

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

#define gScene Scene::Get()

//TODO: Rename to SceneManager
class Scene	: public System, public Singleton<Scene>
{
public:

	Scene(void);
	~Scene(void);

	//TODO: AddEntity()

protected:
	friend class Entity;

	EventResult _OnEvent(int _type, void* _data) override;
};

//----------------------------------------------------------------------------//
// SceneSystem
//----------------------------------------------------------------------------//

//TODO: Remove it
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
//
//----------------------------------------------------------------------------//
