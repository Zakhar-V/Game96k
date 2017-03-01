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
	Reflection m_reflection; // ������� ���������
	Assets m_assets; // ��������� ����������� �������
	ResourceManager m_resourceMgr; // �������� ��������
	Device m_device; // ����, ���� � ������
	Graphics m_graphics; // ����������� �������
	//EntitySystem m_entityMgr; // �������� ���������/�����������
	SceneManager m_sceneMgr; // �������� ����
	Renderer m_renderer; // ��������
	UISystem m_ui;
	BehaviorSystem m_behavior;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
