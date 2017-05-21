#pragma once

#include "OpenGL.hpp"

//----------------------------------------------------------------------------//
// Functions
//----------------------------------------------------------------------------//

#define _GLFUNC(R, N, ...) R (APIENTRY*N)(##__VA_ARGS__) = nullptr
#define _EOL ;
#include "OpenGL.inl"

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

	static void** _funcs[] =
	{
#define _GLFUNC(R, N, ...) (void**)&N
#define _EOL ,
#include "OpenGL.inl"
	};


#ifdef _LOG
	int _loaded = 0;
#endif
	for (size_t i = 0; i < sizeof(_funcs) / sizeof(void*); ++i)
	{
		*_funcs[i] = wglGetProcAddress(_names[i]);
#ifdef _LOG
		if (*_funcs[i])
			++_loaded;
#endif
	}

#ifdef _LOG
	LOG("Load OpenGL: %d/%d", _loaded, sizeof(_funcs) / sizeof(void*));
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
