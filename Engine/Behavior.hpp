#pragma once

#include "Scene.hpp"
//----------------------------------------------------------------------------//
// Defs 
//----------------------------------------------------------------------------//

#define gBehaviorSystem BehaviorSystem::Get()

//----------------------------------------------------------------------------//
// Behavior 
//----------------------------------------------------------------------------//

class Behavior : public Component
{
public:
	RTTI("Behavior");


protected:
	friend class BehaviorSystem;

	void _Register(void) override;	void _Unregister(void) override;

	virtual void _Start(void)
	{ 
		LOG("start");
	}
	virtual void _Update(void) { }
	virtual void _OnDestroy(void) { }

	Behavior* m_prevBehavior = nullptr;
	Behavior* m_nextBehavior = nullptr;
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
	friend class Behavior;

	void _Register(Behavior* _behavior);
	void _Unregister(Behavior* _behavior);

	Behavior* m_activeBehaviors = nullptr;
	Behavior* m_inactiveBehaviors = nullptr;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
