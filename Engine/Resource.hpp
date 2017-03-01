#pragma once

#include "System.hpp"

//----------------------------------------------------------------------------//
// Resource
//----------------------------------------------------------------------------//

struct ResourceName
{
	uint id;
#ifdef _EDITOR
	String name;
	void SetName(const String& _name) { name = _name; id = String::Hash(_name); }
#endif
};

//----------------------------------------------------------------------------//
// Resource
//----------------------------------------------------------------------------//

typedef SharedPtr<class Resource> ResourcePtr;

class Resource : public Object
{
public:
	RTTI("Resource");

	Resource* GetNext(void) { return m_next; }

	void SetID(uint _id) { m_name.id = _id; }
	uint GetID(void) { return m_name.id; }

#ifdef _EDITOR
	void SetName(const String& _name) { m_name.SetName(_name); }
	const String& GetName(void) { return m_name.name; }
#endif

protected:
	friend class ResourceCache;

	Resource* m_prev = nullptr;
	Resource* m_next = nullptr;

	ResourceName m_name;
};

//----------------------------------------------------------------------------//
// ResourceCache
//----------------------------------------------------------------------------//

class ResourceCache
{
public:
	uint GetType(void) { return m_type; }
	ResourceCache* GetNext(void) { return m_next; }

	Resource* GetResource(uint _id);
	Resource* AddResource(uint _id, const char* _name = nullptr);
	void UnloadAll(void);

protected:
	friend class ResourceManager;

	ResourceCache(uint _type, Object::Factory _factory);

	ResourceCache* m_prev = nullptr;;
	ResourceCache* m_next = nullptr;;
	uint m_type = 0;
#ifdef _HAVE_TYPE_NAMES
	const char* m_typename = "";
#endif
	Object::Factory m_factory = nullptr;
	Resource* m_resources = nullptr;
	uint m_numResources = 0;
};

//----------------------------------------------------------------------------//
// ResourceManager 
//----------------------------------------------------------------------------//

#define gResources ResourceManager::Get()

class ResourceManager : public System, public Singleton<ResourceManager>
{
public:
	ResourceManager(void);
	~ResourceManager(void);

	template <class T> void RegisterType()
	{
		LOG("Create %s cache", T::TypeName);
		ASSERT(GetCache(T::Type) == nullptr);
		ResourceCache* _cache = new ResourceCache(T::Type, Object::Create<T>);
		Link(m_caches, _cache, _cache->m_prev);
#ifdef _HAVE_TYPE_NAMES
		_cache->m_typename = T::TypeName;
#endif
	}

	ResourceCache* GetCache(uint _type);

	Resource* GetResource(uint _type, uint _id);
	Resource* AddResource(uint _type, uint _id, const char* _name = nullptr);
	template <class T> T* GetResource(uint _id) { return static_cast<T*>(GetResource(T::Type, _id)); }
	template <class T> T* AddResource(uint _id, const char* _name = nullptr) { return static_cast<T*>(AddResource(T::Type, _id, _name)); }

protected:
	EventResult _OnEvent(int _type, void* _data) override;

	ResourceCache* m_caches = nullptr;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
