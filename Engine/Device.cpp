#pragma once

#include "Device.hpp"

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
		m_size.Set((float)(_rect.right - _rect.left), (float)(_rect.bottom - _rect.top));
	}

	m_opened = true;
	m_active = true;
}
//----------------------------------------------------------------------------//
Device::~Device(void)
{
	LOG_NODE("Destroy Device");

	DestroyWindow(m_window);
	DeleteAtom(m_atom);
}
//----------------------------------------------------------------------------//
EventResult Device::_OnEvent(int _type, void* _data)
{
	switch (_type)
	{
	case SystemEvent::BeginFrame:
	{
		for (MSG _msg; PeekMessageA(&_msg, 0, 0, 0, PM_REMOVE);)
		{
			TranslateMessage(&_msg);
			DispatchMessageA(&_msg);
		}

		if (m_cursorMode == CursorMode::Camera && m_active)
			_CursorToCenter();

	} break;
	}

	return ER_Pass;
}
//----------------------------------------------------------------------------//
LRESULT Device::_HandleMsg(UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_SETFOCUS:
		m_active = true;

		m_resetCameraDelta = true;

		if (m_cursorMode != CursorMode::Default)
			ShowCursor(false);
		break;

	case WM_KILLFOCUS:
		m_active = false;
		m_resetCameraDelta = true;

		ClipCursor(nullptr);
		if (m_cursorMode != CursorMode::Default)
			ShowCursor(true);
		break;

	case WM_CLOSE:
		m_opened = false;
		return 0;

	case WM_SIZE:
	{
		if (_wParam == SIZE_MAXIMIZED || _wParam == SIZE_RESTORED)
		{
			m_size.Set(LOWORD(_lParam), HIWORD(_lParam));
		}
		m_resetCameraDelta = true;

		Vector2 _size = m_size;
		SendEvent(DeviceEvent::Resize, &_size);
	}
	break;

	case WM_MOUSEMOVE:
	{
		if ((m_cursorMode == CursorMode::Camera || m_fullscreen) && m_active)
		{
			RECT _rc, _crc;
			POINT _pt;
			GetClientRect(m_window, &_rc);
			_pt.x = _rc.left;
			_pt.y = _rc.top;
			ClientToScreen(m_window, &_pt);
			_crc.left = _pt.x;
			_crc.top = _pt.y;
			_crc.right = _pt.x + _rc.right;
			_crc.bottom = _pt.y + _rc.bottom;
			ClipCursor(&_crc);
		}
		else
			ClipCursor(nullptr);

		//m_cursorPrevPos = m_cursorPos;
		m_cursorPos.Set(LOWORD(_lParam), HIWORD(_lParam));
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
void Device::SetCursorMode(CursorMode _mode)
{
	if (m_cursorMode != _mode)
	{
		if (m_cursorMode == CursorMode::Default)
			ShowCursor(false);
		else //if(m_active)
			ShowCursor(true);

		m_cursorMode = _mode;

		if (_mode == CursorMode::Camera)
		{
			m_resetCameraDelta = true;
			_CursorToCenter();
		}
		else
			m_cameraDelta = Vector2::Zero;
	}
}
//----------------------------------------------------------------------------//
void Device::_CursorToCenter(void)
{
	IntVector2 _center = m_size * .5f;
	m_cameraDelta = m_resetCameraDelta ? 0 : (m_cursorPos - _center);
	m_resetCameraDelta = false;
	POINT _pt;
	_pt.x = _center.x;
	_pt.y = _center.y;
	ClientToScreen(m_window, &_pt);
	SetCursorPos(_pt.x, _pt.y);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

