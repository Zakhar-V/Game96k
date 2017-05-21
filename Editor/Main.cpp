#include "../Engine/Engine.hpp"

#include "EditorApp.hpp"

void main(void)
{
	{
		LOG(APP_NAME " build v0.1.0 [WIP] " __DATE__ " " __TIME__);

		EditorApp _app;
		_app.Run();

	}

#ifdef _DEBUG
	system("timeout 5");
#endif

	ExitProcess(0);
}
