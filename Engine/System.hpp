#pragma once

#include "Core.hpp"

//----------------------------------------------------------------------------//
// Event
//----------------------------------------------------------------------------//

// system: 0-100
// device: 101-200
// renderer: 201 - 300
struct SystemEvent
{
	enum Enum
	{
		PreStart,
		Start,
		PostStart,

		PreStop,
		Stop,
		PostStop,

		BeginFrame,
		PreUpdate,
		Update,
		PostUpdate,
		Render,
		EndFrame,
	};
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
	template <class T> static EventResult SendEvent(int _type, T _data) { return SendEvent(_type, reinterpret_cast<void*>(&_data)); }
	template <class T> static EventResult SendEvent(int _type, T* _data) { return SendEvent(_type, reinterpret_cast<void*>(_data)); }

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
