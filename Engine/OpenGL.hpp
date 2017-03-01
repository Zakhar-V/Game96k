#pragma once

#include "Core.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

//----------------------------------------------------------------------------//
// Functions
//----------------------------------------------------------------------------//

struct OpenGL_t
{
#define _GLFUNC(R, N, ...) R (APIENTRY*N)(##__VA_ARGS__)
#define _EOL ;
#include "OpenGL.inl"
};

static union
{
	struct
	{
#define _GLFUNC(R, N, ...) R (APIENTRY*N)(##__VA_ARGS__)
#define _EOL ;
#include "OpenGL.inl"
	};

	struct
	{
		void* OpenGLFunctions[sizeof(OpenGL_t)/sizeof(void*)];
	};
};

//----------------------------------------------------------------------------//
// Loader
//----------------------------------------------------------------------------//

void LoadOpenGL(void);
bool HasOpenGLExtension(const char* _ext);

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
