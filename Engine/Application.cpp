#pragma once

#include "Application.hpp"

//----------------------------------------------------------------------------//
// Application
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Application::Application(void)
{
}
//----------------------------------------------------------------------------//
Application::~Application(void)
{
}
//----------------------------------------------------------------------------//
void Application::Run(void)
{
	LOG("Start app ...");
	for (int i = ET_PreStart; i <= ET_PostStart; ++i)
	{
		if (_SendEvent(i) != ER_Pass)
		{
			CHECK(false, "Error on start of app");
			return;
		}
	}

	LOG("Main loop ...");
	while (!m_quit)
	{
		for (int i = ET_BeginFrame; i <= ET_EndFrame; ++i)
		{
			EventResult _r = _SendEvent(i);
			if (_r == ER_Break)
				continue;
			else if (_r == ER_Fail)
			{
				LOG("End main loop");
				break;
			}
		}
	}

	LOG("Stop app ...");
	for (int i = ET_PreStop; i <= ET_PostStop; ++i)
	{
		if (_SendEvent(i) != ER_Pass)
		{
			CHECK(false, "Error on stop of app");
			return;
		}
	}

	LOG("Exit ...");
}
//----------------------------------------------------------------------------//
EventResult Application::_SendEvent(int _type, void* _data)
{
	EventResult _r = _PreEvent(_type, _data);
	if (_r != ER_Pass)
		return _r;
	_r = System::SendEvent(_type, _data);
	if (_r != ER_Pass)
		return _r;
	return _PostEvent(_type, _data);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
