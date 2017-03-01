#pragma once

#include "Assets.hpp"
#include "Resource.hpp"
#include "Device.hpp"
#include "Graphics.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"
#include "UI.hpp"
#include "Behavior.hpp"

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
	ResourceManager m_resourceMgr; // Менеджер ресурсов
	Device m_device; // Окно, ввод и таймер
	Graphics m_graphics; // Графическая система
	//EntitySystem m_entityMgr; // Менеджер сущностей/компонентов
	SceneManager m_sceneMgr; // Менеджер сцен
	Renderer m_renderer; // Рендерер
	UISystem m_ui;
	BehaviorSystem m_behavior;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
