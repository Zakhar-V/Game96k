#pragma once

#include "System.hpp"
#include "Math.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gDevice Device::Get()

//----------------------------------------------------------------------------//
// Events
//----------------------------------------------------------------------------//

struct DeviceEvent
{
	enum Enum
	{
		Resize = String::ConstHash("DeviceEvent::Resize"), // arg = const Vector2*
	};
};

//----------------------------------------------------------------------------//
// CursorMode
//----------------------------------------------------------------------------//

enum class CursorMode
{
	Default = 0x0,
	Hidden = 0x1,
	Camera = 0x2 | Hidden,
};

//----------------------------------------------------------------------------//
// Device
//----------------------------------------------------------------------------//

class Device : public System, public Singleton<Device>
{
public:
	Device(void);
	~Device(void);

	HWND _WindowHandle(void) { return m_window; }
	uint Width(void) { return (uint)m_size.x; }
	uint Height(void) { return (uint)m_size.y; }
	const Vector2& Size(void) { return m_size; }

	bool IsOpened(void) { return m_opened; }

	void SetCursorMode(CursorMode _mode);
	CursorMode GetCursorMode(void) { return m_cursorMode; }
	const Vector2& CursorPos(void) { return m_cursorPos; }
	const Vector2& CameraDelta(void) { return m_cameraDelta; }

protected:
	EventResult _OnEvent(int _type, void* _data) override;

	void _CursorToCenter(void);

	LRESULT _HandleMsg(UINT _msg, WPARAM _wParam, LPARAM _lParam);
	static LRESULT __stdcall _WindowCallback(HWND _wnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

	ATOM m_atom = 0;
	HWND m_window = 0;

	bool m_opened = false;
	bool m_active = false;
	bool m_fullscreen = false;

	Vector2 m_size = Vector2::Zero;

	CursorMode m_cursorMode = CursorMode::Default;
	Vector2 m_cursorPos = Vector2::Zero;
	//Vector2 m_cursorPrevPos = Vector2::Zero;
	Vector2 m_cameraDelta = Vector2::Zero;
	bool m_resetCameraDelta = true;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
