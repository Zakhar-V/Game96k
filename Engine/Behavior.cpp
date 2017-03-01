#pragma once

#include "Behavior.hpp"

//----------------------------------------------------------------------------//
// Behavior
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void Behavior::_Register(void){	gBehaviorSystem->_Register(this);}//----------------------------------------------------------------------------//
void Behavior::_Unregister(void){	gBehaviorSystem->_Unregister(this);}//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// BehaviorSystem 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
BehaviorSystem::BehaviorSystem(void)
{
	LOG_NODE("Create BehaviorSystem");

	gReflection->GetOrCreateTypeInfo<Behavior>()->SetFactory([]() { return ObjectPtr(new Behavior); });
}
//----------------------------------------------------------------------------//
BehaviorSystem::~BehaviorSystem(void)
{
	LOG_NODE("Destroy BehaviorSystem");
}
//----------------------------------------------------------------------------//
void BehaviorSystem::_Register(Behavior* _behavior)
{
	Link(m_activeBehaviors, _behavior, _behavior->m_prevBehavior);
	_behavior->_Start();
}
//----------------------------------------------------------------------------//
void BehaviorSystem::_Unregister(Behavior* _behavior)
{
	//TODO: active/inactive ?
	Unlink(m_activeBehaviors, _behavior, _behavior->m_prevBehavior);
	_behavior->_OnDestroy();
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
