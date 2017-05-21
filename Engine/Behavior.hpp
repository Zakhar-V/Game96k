#pragma once

#include "Scene.hpp"
//----------------------------------------------------------------------------//
// Defs 
//----------------------------------------------------------------------------//

#define gBehaviorSystem BehaviorSystem::Get()

//----------------------------------------------------------------------------//
// Behavior 
//----------------------------------------------------------------------------//

class Behavior abstract: public Component
{
public:
	RTTI("Behavior"); //!>\note it's abstract base class: typeinfo is not registered 

	Behavior(void);
	~Behavior(void);

protected:
	void _Register(bool _newState) override;

	virtual void _Start(void) { }
	virtual void _Update(void) { }
	virtual void _Stop(void) { }
};

//----------------------------------------------------------------------------//
// BehaviorSystem 
//----------------------------------------------------------------------------//

class BehaviorSystem : public SceneSystem, public Singleton<BehaviorSystem>
{
public:
	BehaviorSystem(void);
	~BehaviorSystem(void);

protected:

	EventResult _OnEvent(int _type, void* _data) override;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
