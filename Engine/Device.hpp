#pragma once

#include "System.hpp"
#include "Math.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gDevice Device::Get()

//----------------------------------------------------------------------------//
// Device
//----------------------------------------------------------------------------//

class Device : public System, public Singleton<Device>
{
public:
	Device(void);
	~Device(void);

	HWND _WindowHandle(void) { return m_window; }
	uint Width(void) { return m_width; }
	uint Height(void) { return m_height; }

	bool IsOpened(void) { return m_opened; }

	// [timer]
	double CurrentTime(void);
	float GetFrameTime(void) { return m_frameTime; }

protected:
	EventResult _OnEvent(int _type, void* _data) override;

	LRESULT _HandleMsg(UINT _msg, WPARAM _wParam, LPARAM _lParam);
	static LRESULT __stdcall _WindowCallback(HWND _wnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

	bool m_opened = false;
	ATOM m_atom = 0;
	HWND m_window = 0;

	uint m_width = 0;
	uint m_height = 0;

	double m_timerFreq = 0;	// in seconds
	double m_beginFrame;
	double m_endMessages;
	double m_endFrame;
	float m_frameTime = 0;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
