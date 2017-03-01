#pragma once

#include "Core.hpp"

//----------------------------------------------------------------------------//
// Event
//----------------------------------------------------------------------------//

enum EventType : uint8
{
	ET_PreStart,
	ET_Start,
	ET_PostStart,

	ET_PreStop,
	ET_Stop,
	ET_PostStop,

	ET_BeginFrame,
	ET_PreUpdate,
	ET_Update,
	ET_PostUpdate,
	ET_PreRender,
	ET_RenderScene,
	ET_PostRenderScene,
	//ET_DebugDraw,
	ET_RenderOverlay,
	ET_PostRender,
	ET_EndFrame,
};

enum EventResult
{
	ER_Fail, // error in event chain. break
	ER_Pass,
	ER_Break, // break event chain
};

//----------------------------------------------------------------------------//
// System
//----------------------------------------------------------------------------//

class System : public NonCopyable
{
public:
	System(void);
	~System(void);

	static EventResult SendEvent(int _type, void* _data = nullptr);

protected:
	virtual EventResult _OnEvent(int _type, void* _data) { return ER_Pass; }

	System* m_prev = nullptr;
	System* m_next = nullptr;

	static System* s_first;
	static System* s_last;
};

//----------------------------------------------------------------------------//
// Subsystem
//----------------------------------------------------------------------------//

/*template <class T> class Subsystem : public System
{
public:
	Subsystem(void)
	{
		ASSERT(s_instance == nullptr);
		s_instance = static_cast<T*>(this);
	}
	~Subsystem(void)
	{
		s_instance = nullptr;
	}

	static T* Get(void) { return s_instance; }
	template<class X> static X* Get(void) { return static_cast<X*>(s_instance); }

protected:
	static T* s_instance;
};

template <class T> T* Subsystem<T>::s_instance = nullptr;*/

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
