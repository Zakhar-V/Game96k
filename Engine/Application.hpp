#pragma once

#include "Assets.hpp"
#include "Timer.hpp"
#include "Thread.hpp"
#include "Device.hpp"
#include "Graphics.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"
#include "UI.hpp"
#include "Physics.hpp"
#include "Pathfinding.hpp"
#include "Behavior.hpp"

#include "Image.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gApplication Application::Get()

//----------------------------------------------------------------------------//
// Application
//----------------------------------------------------------------------------//

class Application : public System, public Singleton<Application>
{
public:
	Application(void);
	~Application(void);

	void Run(void);

protected:
	EventResult _SendEvent(int _type, void* _data = nullptr);

	virtual EventResult _PreEvent(int _type, void* _data = nullptr) { return ER_Pass; };
	virtual EventResult _PostEvent(int _type, void* _data = nullptr) { return ER_Pass; };

	bool m_quit = false;
	Reflection m_reflection; // Система рефлексии
	Assets m_assets; // Загрузчик статических ассетов
	Timer m_timer; // Главный таймер
	ThreadPool m_threads; // Пул потоков
	Device m_device; // Окно, ввод и таймер
	Graphics m_graphics; // Графическая система

	ImageManager m_imagehMgr;
	MaterialManager m_materialhMgr;
	MeshManager m_meshMgr;

	Scene m_scene;
	Renderer m_renderer; // Рендерер
	UISystem m_ui;
	PhysicsSystem m_physics;
	Pathfinding m_pathfinding;
	BehaviorSystem m_behavior;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
