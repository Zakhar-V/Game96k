#pragma once

#include "OpenGL.hpp"

//----------------------------------------------------------------------------//
// OpenGL loader
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void LoadOpenGL(void)
{
	static const char* _names[] = 
	{
#define _GLFUNC(R, N, ...) #N
#define _EOL ,
#include "OpenGL.inl"
	};

#ifdef _DEBUG
	int _loaded = 0;
#endif
	for (size_t i = 0; i < sizeof(OpenGL_t) / sizeof(void*); ++i)
	{
		OpenGLFunctions[i] = wglGetProcAddress(_names[i]);
#ifdef _DEBUG
		if (OpenGLFunctions[i])
			++_loaded;
#endif
	}

#ifdef _LOG
	LOG("Load OpenGL: %d/%d", _loaded, sizeof(OpenGL_t) / sizeof(void*));
#endif
}
//----------------------------------------------------------------------------//
bool HasOpenGLExtension(const char* _ext)
{
	const char* e = (const char*)glGetStringi(GL_EXTENSIONS, 0);
	for (int i = 0; e; e = (const char*)glGetStringi(GL_EXTENSIONS, ++i))
	{
		if (!String::Compare(e, _ext))
			return true;
	}
	return false;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
