#pragma once

#include "Resource.hpp"


//----------------------------------------------------------------------------//
// ResourceCache
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
ResourceCache::ResourceCache(uint _type, Object::Factory _factory) :
	m_type(_type),
	m_factory(_factory)
{
}
//----------------------------------------------------------------------------//
Resource* ResourceCache::GetResource(uint _id)
{
	for (Resource* i = m_resources; i; i = i->GetNext())
	{
		if (i->GetID() == _id)
			return i;
	}
	return nullptr;
}
//----------------------------------------------------------------------------//
Resource* ResourceCache::AddResource(uint _id, const char* _name)
{
	LOG("Add %s '%s' 0x % 04x", m_typename, _name, _id);
	ResourcePtr _res = GetResource(_id);
	if (_res)
	{
		LOG("%s '%s' 0x % 04x already exists", m_typename, _name, _id);
		return _res;
	}

	_res = m_factory().Cast<Resource>();
	_res->SetID(_id);
	_res->AddRef();
	Link<Resource>(m_resources, _res, _res->m_prev);
	++m_numResources;

	return _res;
}
//----------------------------------------------------------------------------//
void ResourceCache::UnloadAll(void)
{
	while (m_resources)
	{
		Resource* _res = m_resources;
		Unlink(m_resources, _res, _res->m_prev);
		_res->Release();
		--m_numResources;
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// ResourceManager 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
ResourceManager::ResourceManager(void)
{
	LOG_NODE("Create ResourceManager");
}
//----------------------------------------------------------------------------//
ResourceManager::~ResourceManager(void)
{
	LOG_NODE("Destroy ResourceManager");
}
//----------------------------------------------------------------------------//
ResourceCache* ResourceManager::GetCache(uint _type)
{
	for (ResourceCache* i = m_caches; i; i = i->GetNext())
	{
		if (i->GetType() == _type)
			return i;
	}
	return nullptr;
}
//----------------------------------------------------------------------------//
Resource* ResourceManager::GetResource(uint _type, uint _id)
{
	ResourceCache* _cache = GetCache(_type);
	ASSERT(_cache != nullptr);
	return _cache->GetResource(_id);
}
//----------------------------------------------------------------------------//
Resource* ResourceManager::AddResource(uint _type, uint _id, const char* _name)
{
	ResourceCache* _cache = GetCache(_type);
	ASSERT(_cache != nullptr);
	return _cache->AddResource(_id, _name);
}
//----------------------------------------------------------------------------//
EventResult ResourceManager::_OnEvent(int _type, void* _data)
{
	switch (_type)
	{
	case ET_Stop:
		for (ResourceCache* i = m_caches; i; i = i->GetNext())
		{
			LOG("Delete %s cache", i->m_typename);
			i->UnloadAll();
		}
	break;
	}

	return ER_Pass;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
