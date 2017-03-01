#pragma once

#include "Scene.hpp"
#include "Renderer.hpp"


//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gUISystem UISystem::Get()

//----------------------------------------------------------------------------//
// UIElement
//----------------------------------------------------------------------------//

class UIElement	: public Component
{
public:
	RTTI("UIElement");

	enum class ScalingMode
	{
		Fixed, // Фиксированный размер
		Relative, // Относительный размер
	};


protected:
	friend class UISystem;

	Vector2 m_minAnchor = Vector2::Zero;
	Vector2 m_maxAnchor = Vector2::One;
	Vector2 m_pivot = .5f;
};

//----------------------------------------------------------------------------//
// UISystem
//----------------------------------------------------------------------------//

class UISystem : public SceneSystem, public Singleton<UISystem>
{
public:

	UISystem(void);
	~UISystem(void);

protected:
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
