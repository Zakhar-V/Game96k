#pragma once

#include "Device.hpp"
#include "OpenGL.hpp"

//----------------------------------------------------------------------------//
// Device
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Device::Device(void)
{
	LOG_NODE("Create Device");

	// create window
	{
		WNDCLASSA _wc;
		memset(&_wc, 0, sizeof(_wc));
		_wc.lpfnWndProc = &_WindowCallback;
		_wc.hIcon = LoadIconA(0, IDI_APPLICATION);
		_wc.hCursor = LoadCursorA(0, IDC_ARROW);
		_wc.lpszClassName = APP_NAME;

		m_atom = RegisterClassA(&_wc);
		m_window = CreateWindowA(_wc.lpszClassName, _wc.lpszClassName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP, 0, 0, nullptr);
		
		CHECK(m_window != nullptr, "Couldn't create Window");
	}

	// size
	{
		// size
		RECT _rect;
		GetClientRect(m_window, &_rect);
		m_width = _rect.right - _rect.left;
		m_height = _rect.bottom - _rect.top;
	}

	// init timer
	{
		LARGE_INTEGER _f;
		QueryPerformanceFrequency(&_f);
		m_timerFreq = 1.0 / (double)_f.QuadPart;
	}

	m_opened = true;
}
//----------------------------------------------------------------------------//
Device::~Device(void)
{
	LOG_NODE("Destroy Device");

	DestroyWindow(m_window);
	DeleteAtom(m_atom);
}
//----------------------------------------------------------------------------//
double Device::CurrentTime(void)
{
	LARGE_INTEGER _c;
	QueryPerformanceCounter(&_c);
	return m_timerFreq * (double)_c.QuadPart;
}
//----------------------------------------------------------------------------//
EventResult Device::_OnEvent(int _type, void* _data)
{
	switch (_type)
	{
	case ET_BeginFrame:
	{
		m_beginFrame = CurrentTime();
		for (MSG _msg; PeekMessageA(&_msg, 0, 0, 0, PM_REMOVE);)
		{
			TranslateMessage(&_msg);
			DispatchMessageA(&_msg);
		}
		m_endMessages = CurrentTime();

	} break;

	case ET_EndFrame:
	{
		m_endFrame = CurrentTime();

		double _dt = m_endMessages - m_beginFrame;
		if (_dt > 0.1)
			m_frameTime = (float)(m_endFrame - m_endMessages);
		else
			m_frameTime = (float)(m_endFrame - m_beginFrame);

		// temp
		/*char _buff[64];
		FloatToString(_buff, m_frameTime);
		SetWindowTextA(m_window, _buff);
		LOG("%f", m_frameTime);*/

	} break;
	}

	return ER_Pass;
}
//----------------------------------------------------------------------------//
LRESULT Device::_HandleMsg(UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_CLOSE:
		m_opened = false;
		return 0;

	case WM_SIZE:
		if (_wParam == SIZE_MAXIMIZED || _wParam == SIZE_RESTORED)
		{
			m_width = LOWORD(_lParam);
			m_height = HIWORD(_lParam);
			//SendEvent(resize, &m_width)
		}
		break;
	}

	return DefWindowProcA(m_window, _msg, _wParam, _lParam);
}
//----------------------------------------------------------------------------//
LRESULT __stdcall Device::_WindowCallback(HWND _wnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (_wnd == gDevice->m_window)
		return gDevice->_HandleMsg(_msg, _wParam, _lParam);

	return DefWindowProcA(_wnd, _msg, _wParam, _lParam);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

