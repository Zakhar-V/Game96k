/*
#ifndef _GLFUNC
#	define _GLFUNC(R, N, ...) R (APIENTRY*N)(##__VA_ARGS__)
#endif
#ifndef _EOL
#	define _EOL ;
#endif
*/

// 1.2
//_GLFUNC(void, glDrawRangeElements, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices)_EOL
//_GLFUNC(void, glTexImage3D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glCopyTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)_EOL

// 1.3
//_GLFUNC(void, glActiveTexture, GLenum texture)_EOL
//_GLFUNC(void, glSampleCoverage, GLfloat value, GLboolean invert)_EOL
//_GLFUNC(void, glCompressedTexImage3D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data)_EOL
//_GLFUNC(void, glCompressedTexImage2D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)_EOL
//_GLFUNC(void, glCompressedTexImage1D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data)_EOL
//_GLFUNC(void, glCompressedTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data)_EOL
//_GLFUNC(void, glCompressedTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)_EOL
//_GLFUNC(void, glCompressedTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data)_EOL
//_GLFUNC(void, glGetCompressedTexImage, GLenum target, GLint level, void *img)_EOL

// 1.4
//_GLFUNC(void, glBlendFuncSeparate, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)_EOL
//_GLFUNC(void, glMultiDrawArrays, GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount)_EOL
//_GLFUNC(void, glMultiDrawElements, GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount)_EOL
//_GLFUNC(void, glPointParameterf, GLenum pname, GLfloat param)_EOL
//_GLFUNC(void, glPointParameterfv, GLenum pname, const GLfloat *params)_EOL
//_GLFUNC(void, glPointParameteri, GLenum pname, GLint param)_EOL
//_GLFUNC(void, glPointParameteriv, GLenum pname, const GLint *params)_EOL
//_GLFUNC(void, glBlendColor, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)_EOL
_GLFUNC(void, glBlendEquation, GLenum mode)_EOL

// 1.5
//_GLFUNC(void, glGenQueries, GLsizei n, GLuint *ids)_EOL
//_GLFUNC(void, glDeleteQueries, GLsizei n, const GLuint *ids)_EOL
//_GLFUNC(GLboolean, glIsQuery, GLuint id)_EOL
//_GLFUNC(void, glBeginQuery, GLenum target, GLuint id)_EOL
//_GLFUNC(void, glEndQuery, GLenum target)_EOL
//_GLFUNC(void, glGetQueryiv, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetQueryObjectiv, GLuint id, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetQueryObjectuiv, GLuint id, GLenum pname, GLuint *params)_EOL
_GLFUNC(void, glBindBuffer, GLenum target, GLuint buffer)_EOL
_GLFUNC(void, glDeleteBuffers, GLsizei n, const GLuint *buffers)_EOL
_GLFUNC(void, glGenBuffers, GLsizei n, GLuint *buffers)_EOL
//_GLFUNC(GLboolean, glIsBuffer, GLuint buffer)_EOL
//_GLFUNC(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage)_EOL
//_GLFUNC(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data)_EOL
//_GLFUNC(void, glGetBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, void *data)_EOL
//_GLFUNC(void *, glMapBuffer, GLenum target, GLenum access)_EOL
//_GLFUNC(GLboolean, glUnmapBuffer, GLenum target)_EOL
//_GLFUNC(void, glGetBufferParameteriv, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetBufferPointerv, GLenum target, GLenum pname, void **params)_EOL

// 2.0
//_GLFUNC(void, glBlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha)_EOL
//_GLFUNC(void, glDrawBuffers, GLsizei n, const GLenum *bufs)_EOL
//_GLFUNC(void, glStencilOpSeparate, GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)_EOL
//_GLFUNC(void, glStencilFuncSeparate, GLenum face, GLenum func, GLint ref, GLuint mask)_EOL
//_GLFUNC(void, glStencilMaskSeparate, GLenum face, GLuint mask)_EOL
//_GLFUNC(void, glAttachShader, GLuint program, GLuint shader)_EOL
//_GLFUNC(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar *name)_EOL
//_GLFUNC(void, glCompileShader, GLuint shader)_EOL
//_GLFUNC(GLuint, glCreateProgram, void)_EOL
//_GLFUNC(GLuint, glCreateShader, GLenum type)_EOL
//_GLFUNC(void, glDeleteProgram, GLuint program)_EOL
//_GLFUNC(void, glDeleteShader, GLuint shader)_EOL
//_GLFUNC(void, glDetachShader, GLuint program, GLuint shader)_EOL
_GLFUNC(void, glDisableVertexAttribArray, GLuint index)_EOL
_GLFUNC(void, glEnableVertexAttribArray, GLuint index)_EOL
//_GLFUNC(void, glGetActiveAttrib, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)_EOL
//_GLFUNC(void, glGetActiveUniform, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)_EOL
//_GLFUNC(void, glGetAttachedShaders, GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders)_EOL
//_GLFUNC(GLint, glGetAttribLocation, GLuint program, const GLchar *name)_EOL
_GLFUNC(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params)_EOL
//#ifdef _DEBUG_GRAPHICS
_GLFUNC(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)_EOL
//#endif
//_GLFUNC(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)_EOL
//_GLFUNC(void, glGetShaderSource, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)_EOL
_GLFUNC(GLint, glGetUniformLocation, GLuint program, const GLchar *name)_EOL
//_GLFUNC(void, glGetUniformfv, GLuint program, GLint location, GLfloat *params)_EOL
//_GLFUNC(void, glGetUniformiv, GLuint program, GLint location, GLint *params)_EOL
//_GLFUNC(void, glGetVertexAttribdv, GLuint index, GLenum pname, GLdouble *params)_EOL
//_GLFUNC(void, glGetVertexAttribfv, GLuint index, GLenum pname, GLfloat *params)_EOL
//_GLFUNC(void, glGetVertexAttribiv, GLuint index, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetVertexAttribPointerv, GLuint index, GLenum pname, void **pointer)_EOL
//_GLFUNC(GLboolean, glIsProgram, GLuint program)_EOL
//_GLFUNC(GLboolean, glIsShader, GLuint shader)_EOL
//_GLFUNC(void, glLinkProgram, GLuint program)_EOL
//_GLFUNC(void, glShaderSource, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)_EOL
//_GLFUNC(void, glUseProgram, GLuint program)_EOL
//_GLFUNC(void, glUniform1f, GLint location, GLfloat v0)_EOL
//_GLFUNC(void, glUniform2f, GLint location, GLfloat v0, GLfloat v1)_EOL
//_GLFUNC(void, glUniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)_EOL
//_GLFUNC(void, glUniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)_EOL
//_GLFUNC(void, glUniform1i, GLint location, GLint v0)_EOL
//_GLFUNC(void, glUniform2i, GLint location, GLint v0, GLint v1)_EOL
//_GLFUNC(void, glUniform3i, GLint location, GLint v0, GLint v1, GLint v2)_EOL
//_GLFUNC(void, glUniform4i, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)_EOL
//_GLFUNC(void, glUniform1fv, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glUniform2fv, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glUniform3fv, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glUniform4fv, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glUniform1iv, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glUniform2iv, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glUniform3iv, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glUniform4iv, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glValidateProgram, GLuint program)_EOL
//_GLFUNC(void, glVertexAttrib1d, GLuint index, GLdouble x)_EOL
//_GLFUNC(void, glVertexAttrib1dv, GLuint index, const GLdouble *v)_EOL
//_GLFUNC(void, glVertexAttrib1f, GLuint index, GLfloat x)_EOL
//_GLFUNC(void, glVertexAttrib1fv, GLuint index, const GLfloat *v)_EOL
//_GLFUNC(void, glVertexAttrib1s, GLuint index, GLshort x)_EOL
//_GLFUNC(void, glVertexAttrib1sv, GLuint index, const GLshort *v)_EOL
//_GLFUNC(void, glVertexAttrib2d, GLuint index, GLdouble x, GLdouble y)_EOL
//_GLFUNC(void, glVertexAttrib2dv, GLuint index, const GLdouble *v)_EOL
//_GLFUNC(void, glVertexAttrib2f, GLuint index, GLfloat x, GLfloat y)_EOL
//_GLFUNC(void, glVertexAttrib2fv, GLuint index, const GLfloat *v)_EOL
//_GLFUNC(void, glVertexAttrib2s, GLuint index, GLshort x, GLshort y)_EOL
//_GLFUNC(void, glVertexAttrib2sv, GLuint index, const GLshort *v)_EOL
//_GLFUNC(void, glVertexAttrib3d, GLuint index, GLdouble x, GLdouble y, GLdouble z)_EOL
//_GLFUNC(void, glVertexAttrib3dv, GLuint index, const GLdouble *v)_EOL
//_GLFUNC(void, glVertexAttrib3f, GLuint index, GLfloat x, GLfloat y, GLfloat z)_EOL
//_GLFUNC(void, glVertexAttrib3fv, GLuint index, const GLfloat *v)_EOL
//_GLFUNC(void, glVertexAttrib3s, GLuint index, GLshort x, GLshort y, GLshort z)_EOL
//_GLFUNC(void, glVertexAttrib3sv, GLuint index, const GLshort *v)_EOL
//_GLFUNC(void, glVertexAttrib4Nbv, GLuint index, const GLbyte *v)_EOL
//_GLFUNC(void, glVertexAttrib4Niv, GLuint index, const GLint *v)_EOL
//_GLFUNC(void, glVertexAttrib4Nsv, GLuint index, const GLshort *v)_EOL
//_GLFUNC(void, glVertexAttrib4Nub, GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w)_EOL
//_GLFUNC(void, glVertexAttrib4Nubv, GLuint index, const GLubyte *v)_EOL
//_GLFUNC(void, glVertexAttrib4Nuiv, GLuint index, const GLuint *v)_EOL
//_GLFUNC(void, glVertexAttrib4Nusv, GLuint index, const GLushort *v)_EOL
//_GLFUNC(void, glVertexAttrib4bv, GLuint index, const GLbyte *v)_EOL
//_GLFUNC(void, glVertexAttrib4d, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)_EOL
//_GLFUNC(void, glVertexAttrib4dv, GLuint index, const GLdouble *v)_EOL
//_GLFUNC(void, glVertexAttrib4f, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)_EOL
_GLFUNC(void, glVertexAttrib4fv, GLuint index, const GLfloat *v)_EOL
_GLFUNC(void, glVertexAttrib4iv, GLuint index, const GLint *v)_EOL
//_GLFUNC(void, glVertexAttrib4s, GLuint index, GLshort x, GLshort y, GLshort z, GLshort w)_EOL
//_GLFUNC(void, glVertexAttrib4sv, GLuint index, const GLshort *v)_EOL
//_GLFUNC(void, glVertexAttrib4ubv, GLuint index, const GLubyte *v)_EOL
//_GLFUNC(void, glVertexAttrib4uiv, GLuint index, const GLuint *v)_EOL
//_GLFUNC(void, glVertexAttrib4usv, GLuint index, const GLushort *v)_EOL
//_GLFUNC(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)_EOL

// 2.1
//_GLFUNC(void, glUniformMatrix2x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glUniformMatrix3x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glUniformMatrix2x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glUniformMatrix4x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glUniformMatrix3x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glUniformMatrix4x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL

// 3.0
//_GLFUNC(void, glColorMaski, GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)_EOL
//_GLFUNC(void, glGetBooleani_v, GLenum target, GLuint index, GLboolean *data)_EOL
//_GLFUNC(void, glGetIntegeri_v, GLenum target, GLuint index, GLint *data)_EOL
//_GLFUNC(void, glEnablei, GLenum target, GLuint index)_EOL
//_GLFUNC(void, glDisablei, GLenum target, GLuint index)_EOL
//_GLFUNC(GLboolean, glIsEnabledi, GLenum target, GLuint index)_EOL
//_GLFUNC(void, glBeginTransformFeedback, GLenum primitiveMode)_EOL
//_GLFUNC(void, glEndTransformFeedback, void)_EOL
//_GLFUNC(void, glBindBufferRange, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)_EOL
_GLFUNC(void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer)_EOL
//_GLFUNC(void, glTransformFeedbackVaryings, GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode)_EOL
//_GLFUNC(void, glGetTransformFeedbackVarying, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)_EOL
//_GLFUNC(void, glClampColor, GLenum target, GLenum clamp)_EOL
//_GLFUNC(void, glBeginConditionalRender, GLuint id, GLenum mode)_EOL
//_GLFUNC(void, glEndConditionalRender, void)_EOL
//_GLFUNC(void, glVertexAttribIPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer)_EOL
//_GLFUNC(void, glGetVertexAttribIiv, GLuint index, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetVertexAttribIuiv, GLuint index, GLenum pname, GLuint *params)_EOL
//_GLFUNC(void, glVertexAttribI1i, GLuint index, GLint x)_EOL
//_GLFUNC(void, glVertexAttribI2i, GLuint index, GLint x, GLint y)_EOL
//_GLFUNC(void, glVertexAttribI3i, GLuint index, GLint x, GLint y, GLint z)_EOL
//_GLFUNC(void, glVertexAttribI4i, GLuint index, GLint x, GLint y, GLint z, GLint w)_EOL
//_GLFUNC(void, glVertexAttribI1ui, GLuint index, GLuint x)_EOL
//_GLFUNC(void, glVertexAttribI2ui, GLuint index, GLuint x, GLuint y)_EOL
//_GLFUNC(void, glVertexAttribI3ui, GLuint index, GLuint x, GLuint y, GLuint z)_EOL
//_GLFUNC(void, glVertexAttribI4ui, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)_EOL
//_GLFUNC(void, glVertexAttribI1iv, GLuint index, const GLint *v)_EOL
//_GLFUNC(void, glVertexAttribI2iv, GLuint index, const GLint *v)_EOL
//_GLFUNC(void, glVertexAttribI3iv, GLuint index, const GLint *v)_EOL
//_GLFUNC(void, glVertexAttribI4iv, GLuint index, const GLint *v)_EOL
//_GLFUNC(void, glVertexAttribI1uiv, GLuint index, const GLuint *v)_EOL
//_GLFUNC(void, glVertexAttribI2uiv, GLuint index, const GLuint *v)_EOL
//_GLFUNC(void, glVertexAttribI3uiv, GLuint index, const GLuint *v)_EOL
//_GLFUNC(void, glVertexAttribI4uiv, GLuint index, const GLuint *v)_EOL
//_GLFUNC(void, glVertexAttribI4bv, GLuint index, const GLbyte *v)_EOL
//_GLFUNC(void, glVertexAttribI4sv, GLuint index, const GLshort *v)_EOL
//_GLFUNC(void, glVertexAttribI4ubv, GLuint index, const GLubyte *v)_EOL
//_GLFUNC(void, glVertexAttribI4usv, GLuint index, const GLushort *v)_EOL
//_GLFUNC(void, glGetUniformuiv, GLuint program, GLint location, GLuint *params)_EOL
//_GLFUNC(void, glBindFragDataLocation, GLuint program, GLuint color, const GLchar *name)_EOL
//_GLFUNC(GLint, glGetFragDataLocation, GLuint program, const GLchar *name)_EOL
//_GLFUNC(void, glUniform1ui, GLint location, GLuint v0)_EOL
//_GLFUNC(void, glUniform2ui, GLint location, GLuint v0, GLuint v1)_EOL
//_GLFUNC(void, glUniform3ui, GLint location, GLuint v0, GLuint v1, GLuint v2)_EOL
//_GLFUNC(void, glUniform4ui, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)_EOL
//_GLFUNC(void, glUniform1uiv, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glUniform2uiv, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glUniform3uiv, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glUniform4uiv, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glTexParameterIiv, GLenum target, GLenum pname, const GLint *params)_EOL
//_GLFUNC(void, glTexParameterIuiv, GLenum target, GLenum pname, const GLuint *params)_EOL
//_GLFUNC(void, glGetTexParameterIiv, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetTexParameterIuiv, GLenum target, GLenum pname, GLuint *params)_EOL
_GLFUNC(void, glClearBufferiv, GLenum buffer, GLint drawbuffer, const GLint *value)_EOL
//_GLFUNC(void, glClearBufferuiv, GLenum buffer, GLint drawbuffer, const GLuint *value)_EOL
_GLFUNC(void, glClearBufferfv, GLenum buffer, GLint drawbuffer, const GLfloat *value)_EOL
//_GLFUNC(void, glClearBufferfi, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)_EOL
_GLFUNC(const GLubyte *, glGetStringi, GLenum name, GLuint index)_EOL
//_GLFUNC(GLboolean, glIsRenderbuffer, GLuint renderbuffer)_EOL
_GLFUNC(void, glBindRenderbuffer, GLenum target, GLuint renderbuffer)_EOL
//_GLFUNC(void, glDeleteRenderbuffers, GLsizei n, const GLuint *renderbuffers)_EOL
_GLFUNC(void, glGenRenderbuffers, GLsizei n, GLuint *renderbuffers)_EOL
//_GLFUNC(void, glRenderbufferStorage, GLenum target, GLenum internalformat, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glGetRenderbufferParameteriv, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(GLboolean, glIsFramebuffer, GLuint framebuffer)_EOL
_GLFUNC(void, glBindFramebuffer, GLenum target, GLuint framebuffer)_EOL
//_GLFUNC(void, glDeleteFramebuffers, GLsizei n, const GLuint *framebuffers)_EOL
_GLFUNC(void, glGenFramebuffers, GLsizei n, GLuint *framebuffers)_EOL
#ifdef _DEBUG_GRAPHICS
//_GLFUNC(GLenum, glCheckFramebufferStatus, GLenum target)_EOL
#endif
//_GLFUNC(void, glFramebufferTexture1D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)_EOL
//_GLFUNC(void, glFramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)_EOL
//_GLFUNC(void, glFramebufferTexture3D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)_EOL
//_GLFUNC(void, glFramebufferRenderbuffer, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)_EOL
//_GLFUNC(void, glGetFramebufferAttachmentParameteriv, GLenum target, GLenum attachment, GLenum pname, GLint *params)_EOL
_GLFUNC(void, glGenerateMipmap, GLenum target)_EOL
_GLFUNC(void, glBlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)_EOL
//_GLFUNC(void, glRenderbufferStorageMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glFramebufferTextureLayer, GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)_EOL
//_GLFUNC(void*, glMapBufferRange, GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)_EOL
//_GLFUNC(void, glFlushMappedBufferRange, GLenum target, GLintptr offset, GLsizeiptr length)_EOL
_GLFUNC(void, glBindVertexArray, GLuint array)_EOL
//_GLFUNC(void, glDeleteVertexArrays, GLsizei n, const GLuint *arrays)_EOL
_GLFUNC(void, glGenVertexArrays, GLsizei n, GLuint *arrays)_EOL
//_GLFUNC(GLboolean, glIsVertexArray, GLuint array)_EOL

// 3.1
_GLFUNC(void, glDrawArraysInstanced, GLenum mode, GLint first, GLsizei count, GLsizei instancecount)_EOL
//_GLFUNC(void, glDrawElementsInstanced, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount)_EOL
//_GLFUNC(void, glTexBuffer, GLenum target, GLenum internalformat, GLuint buffer)_EOL
//_GLFUNC(void, glPrimitiveRestartIndex, GLuint index)_EOL
//_GLFUNC(void, glCopyBufferSubData, GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)_EOL
//_GLFUNC(void, glGetUniformIndices, GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices)_EOL
//_GLFUNC(void, glGetActiveUniformsiv, GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetActiveUniformName, GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName)_EOL
//_GLFUNC(GLuint, glGetUniformBlockIndex, GLuint program, const GLchar *uniformBlockName)_EOL
//_GLFUNC(void, glGetActiveUniformBlockiv, GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetActiveUniformBlockName, GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)_EOL
//_GLFUNC(void, glUniformBlockBinding, GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)_EOL

// 3.2
//_GLFUNC(void, glDrawElementsBaseVertex, GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex)_EOL
//_GLFUNC(void, glDrawRangeElementsBaseVertex, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex)_EOL
_GLFUNC(void, glDrawElementsInstancedBaseVertex, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex)_EOL
//_GLFUNC(void, glMultiDrawElementsBaseVertex, GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex)_EOL
//_GLFUNC(void, glProvokingVertex, GLenum mode)_EOL
//_GLFUNC(GLsync, glFenceSync, GLenum condition, GLbitfield flags)_EOL
//_GLFUNC(GLboolean, glIsSync, GLsync sync)_EOL
//_GLFUNC(void, glDeleteSync, GLsync sync)_EOL
//_GLFUNC(GLenum, glClientWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout)_EOL
//_GLFUNC(void, glWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout)_EOL
//_GLFUNC(void, glGetInteger64v, GLenum pname, GLint64 *data)_EOL
//_GLFUNC(void, glGetSynciv, GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values)_EOL
//_GLFUNC(void, glGetInteger64i_v, GLenum target, GLuint index, GLint64 *data)_EOL
//_GLFUNC(void, glGetBufferParameteri64v, GLenum target, GLenum pname, GLint64 *params)_EOL
//_GLFUNC(void, glFramebufferTexture, GLenum target, GLenum attachment, GLuint texture, GLint level)_EOL
//_GLFUNC(void, glTexImage2DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)_EOL
//_GLFUNC(void, glTexImage3DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)_EOL
//_GLFUNC(void, glGetMultisamplefv, GLenum pname, GLuint index, GLfloat *val)_EOL
//_GLFUNC(void, glSampleMaski, GLuint maskNumber, GLbitfield mask)_EOL

// 3.3
//_GLFUNC(void, glBindFragDataLocationIndexed, GLuint program, GLuint colorNumber, GLuint index, const GLchar *name)_EOL
//_GLFUNC(GLint, glGetFragDataIndex, GLuint program, const GLchar *name)_EOL
_GLFUNC(void, glGenSamplers, GLsizei count, GLuint *samplers)_EOL
//_GLFUNC(void, glDeleteSamplers, GLsizei count, const GLuint *samplers)_EOL
//_GLFUNC(GLboolean, glIsSampler, GLuint sampler)_EOL
_GLFUNC(void, glBindSampler, GLuint unit, GLuint sampler)_EOL
_GLFUNC(void, glSamplerParameteri, GLuint sampler, GLenum pname, GLint param)_EOL
//_GLFUNC(void, glSamplerParameteriv, GLuint sampler, GLenum pname, const GLint *param)_EOL
//_GLFUNC(void, glSamplerParameterf, GLuint sampler, GLenum pname, GLfloat param)_EOL
//_GLFUNC(void, glSamplerParameterfv, GLuint sampler, GLenum pname, const GLfloat *param)_EOL
//_GLFUNC(void, glSamplerParameterIiv, GLuint sampler, GLenum pname, const GLint *param)_EOL
//_GLFUNC(void, glSamplerParameterIuiv, GLuint sampler, GLenum pname, const GLuint *param)_EOL
//_GLFUNC(void, glGetSamplerParameteriv, GLuint sampler, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetSamplerParameterIiv, GLuint sampler, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetSamplerParameterfv, GLuint sampler, GLenum pname, GLfloat *params)_EOL
//_GLFUNC(void, glGetSamplerParameterIuiv, GLuint sampler, GLenum pname, GLuint *params)_EOL
//_GLFUNC(void, glQueryCounter, GLuint id, GLenum target)_EOL
//_GLFUNC(void, glGetQueryObjecti64v, GLuint id, GLenum pname, GLint64 *params)_EOL
//_GLFUNC(void, glGetQueryObjectui64v, GLuint id, GLenum pname, GLuint64 *params)_EOL
//_GLFUNC(void, glVertexAttribDivisor, GLuint index, GLuint divisor)_EOL
//_GLFUNC(void, glVertexAttribP1ui, GLuint index, GLenum type, GLboolean normalized, GLuint value)_EOL
//_GLFUNC(void, glVertexAttribP1uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value)_EOL
//_GLFUNC(void, glVertexAttribP2ui, GLuint index, GLenum type, GLboolean normalized, GLuint value)_EOL
//_GLFUNC(void, glVertexAttribP2uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value)_EOL
//_GLFUNC(void, glVertexAttribP3ui, GLuint index, GLenum type, GLboolean normalized, GLuint value)_EOL
//_GLFUNC(void, glVertexAttribP3uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value)_EOL
//_GLFUNC(void, glVertexAttribP4ui, GLuint index, GLenum type, GLboolean normalized, GLuint value)_EOL
//_GLFUNC(void, glVertexAttribP4uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value)_EOL

// 4.0
//_GLFUNC(void, glMinSampleShading, GLfloat value)_EOL
//_GLFUNC(void, glBlendEquationi, GLuint buf, GLenum mode)_EOL
//_GLFUNC(void, glBlendEquationSeparatei, GLuint buf, GLenum modeRGB, GLenum modeAlpha)_EOL
//_GLFUNC(void, glBlendFunci, GLuint buf, GLenum src, GLenum dst)_EOL
//_GLFUNC(void, glBlendFuncSeparatei, GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)_EOL
//_GLFUNC(void, glDrawArraysIndirect, GLenum mode, const void *indirect)_EOL
//_GLFUNC(void, glDrawElementsIndirect, GLenum mode, GLenum type, const void *indirect)_EOL
//_GLFUNC(void, glUniform1d, GLint location, GLdouble x)_EOL
//_GLFUNC(void, glUniform2d, GLint location, GLdouble x, GLdouble y)_EOL
//_GLFUNC(void, glUniform3d, GLint location, GLdouble x, GLdouble y, GLdouble z)_EOL
//_GLFUNC(void, glUniform4d, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w)_EOL
//_GLFUNC(void, glUniform1dv, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glUniform2dv, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glUniform3dv, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glUniform4dv, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glUniformMatrix2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glUniformMatrix3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glUniformMatrix4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glUniformMatrix2x3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glUniformMatrix2x4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glUniformMatrix3x2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glUniformMatrix3x4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glUniformMatrix4x2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glUniformMatrix4x3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glGetUniformdv, GLuint program, GLint location, GLdouble *params)_EOL
//_GLFUNC(GLint, glGetSubroutineUniformLocation, GLuint program, GLenum shadertype, const GLchar *name)_EOL
//_GLFUNC(GLuint, glGetSubroutineIndex, GLuint program, GLenum shadertype, const GLchar *name)_EOL
//_GLFUNC(void, glGetActiveSubroutineUniformiv, GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values)_EOL
//_GLFUNC(void, glGetActiveSubroutineUniformName, GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name)_EOL
//_GLFUNC(void, glGetActiveSubroutineName, GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name)_EOL
//_GLFUNC(void, glUniformSubroutinesuiv, GLenum shadertype, GLsizei count, const GLuint *indices)_EOL
//_GLFUNC(void, glGetUniformSubroutineuiv, GLenum shadertype, GLint location, GLuint *params)_EOL
//_GLFUNC(void, glGetProgramStageiv, GLuint program, GLenum shadertype, GLenum pname, GLint *values)_EOL
//_GLFUNC(void, glPatchParameteri, GLenum pname, GLint value)_EOL
//_GLFUNC(void, glPatchParameterfv, GLenum pname, const GLfloat *values)_EOL
//_GLFUNC(void, glBindTransformFeedback, GLenum target, GLuint id)_EOL
//_GLFUNC(void, glDeleteTransformFeedbacks, GLsizei n, const GLuint *ids)_EOL
//_GLFUNC(void, glGenTransformFeedbacks, GLsizei n, GLuint *ids)_EOL
//_GLFUNC(GLboolean, glIsTransformFeedback, GLuint id)_EOL
//_GLFUNC(void, glPauseTransformFeedback, void)_EOL
//_GLFUNC(void, glResumeTransformFeedback, void)_EOL
//_GLFUNC(void, glDrawTransformFeedback, GLenum mode, GLuint id)_EOL
//_GLFUNC(void, glDrawTransformFeedbackStream, GLenum mode, GLuint id, GLuint stream)_EOL
//_GLFUNC(void, glBeginQueryIndexed, GLenum target, GLuint index, GLuint id)_EOL
//_GLFUNC(void, glEndQueryIndexed, GLenum target, GLuint index)_EOL
//_GLFUNC(void, glGetQueryIndexediv, GLenum target, GLuint index, GLenum pname, GLint *params)_EOL

// 4.1 (GL_ARB_separate_shader_objects)
//_GLFUNC(void, glReleaseShaderCompiler, void)_EOL
//_GLFUNC(void, glShaderBinary, GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length)_EOL
//_GLFUNC(void, glGetShaderPrecisionFormat, GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision)_EOL
//_GLFUNC(void, glDepthRangef, GLfloat n, GLfloat f)_EOL
//_GLFUNC(void, glClearDepthf, GLfloat d)_EOL
//_GLFUNC(void, glGetProgramBinary, GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)_EOL
//_GLFUNC(void, glProgramBinary, GLuint program, GLenum binaryFormat, const void *binary, GLsizei length)_EOL
//_GLFUNC(void, glProgramParameteri, GLuint program, GLenum pname, GLint value)_EOL
_GLFUNC(void, glUseProgramStages, GLuint pipeline, GLbitfield stages, GLuint program)_EOL
_GLFUNC(void, glActiveShaderProgram, GLuint pipeline, GLuint program)_EOL
_GLFUNC(GLuint, glCreateShaderProgramv, GLenum type, GLsizei count, const GLchar *const*strings)_EOL
_GLFUNC(void, glBindProgramPipeline, GLuint pipeline)_EOL
_GLFUNC(void, glDeleteProgramPipelines, GLsizei n, const GLuint *pipelines)_EOL
_GLFUNC(void, glGenProgramPipelines, GLsizei n, GLuint *pipelines)_EOL
//_GLFUNC(GLboolean, glIsProgramPipeline, GLuint pipeline)_EOL
#ifdef _DEBUG_GRAPHICS
//_GLFUNC(void, glGetProgramPipelineiv, GLuint pipeline, GLenum pname, GLint *params)_EOL
#endif
_GLFUNC(void, glProgramUniform1i, GLuint program, GLint location, GLint v0)_EOL
//_GLFUNC(void, glProgramUniform1iv, GLuint program, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glProgramUniform1f, GLuint program, GLint location, GLfloat v0)_EOL
//_GLFUNC(void, glProgramUniform1fv, GLuint program, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniform1d, GLuint program, GLint location, GLdouble v0)_EOL
//_GLFUNC(void, glProgramUniform1dv, GLuint program, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniform1ui, GLuint program, GLint location, GLuint v0)_EOL
//_GLFUNC(void, glProgramUniform1uiv, GLuint program, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glProgramUniform2i, GLuint program, GLint location, GLint v0, GLint v1)_EOL
//_GLFUNC(void, glProgramUniform2iv, GLuint program, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glProgramUniform2f, GLuint program, GLint location, GLfloat v0, GLfloat v1)_EOL
//_GLFUNC(void, glProgramUniform2fv, GLuint program, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniform2d, GLuint program, GLint location, GLdouble v0, GLdouble v1)_EOL
//_GLFUNC(void, glProgramUniform2dv, GLuint program, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniform2ui, GLuint program, GLint location, GLuint v0, GLuint v1)_EOL
//_GLFUNC(void, glProgramUniform2uiv, GLuint program, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glProgramUniform3i, GLuint program, GLint location, GLint v0, GLint v1, GLint v2)_EOL
//_GLFUNC(void, glProgramUniform3iv, GLuint program, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glProgramUniform3f, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)_EOL
//_GLFUNC(void, glProgramUniform3fv, GLuint program, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniform3d, GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2)_EOL
//_GLFUNC(void, glProgramUniform3dv, GLuint program, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniform3ui, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2)_EOL
//_GLFUNC(void, glProgramUniform3uiv, GLuint program, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glProgramUniform4i, GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)_EOL
//_GLFUNC(void, glProgramUniform4iv, GLuint program, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glProgramUniform4f, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)_EOL
//_GLFUNC(void, glProgramUniform4fv, GLuint program, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniform4d, GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)_EOL
//_GLFUNC(void, glProgramUniform4dv, GLuint program, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniform4ui, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)_EOL
//_GLFUNC(void, glProgramUniform4uiv, GLuint program, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2x3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3x2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2x4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4x2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3x4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4x3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2x3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3x2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2x4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4x2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3x4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4x3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
#ifdef _DEBUG_GRAPHICS
//_GLFUNC(void, glValidateProgramPipeline, GLuint pipeline)_EOL
//_GLFUNC(void, glGetProgramPipelineInfoLog, GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog)_EOL
#endif
//_GLFUNC(void, glVertexAttribL1d, GLuint index, GLdouble x)_EOL
//_GLFUNC(void, glVertexAttribL2d, GLuint index, GLdouble x, GLdouble y)_EOL
//_GLFUNC(void, glVertexAttribL3d, GLuint index, GLdouble x, GLdouble y, GLdouble z)_EOL
//_GLFUNC(void, glVertexAttribL4d, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)_EOL
//_GLFUNC(void, glVertexAttribL1dv, GLuint index, const GLdouble *v)_EOL
//_GLFUNC(void, glVertexAttribL2dv, GLuint index, const GLdouble *v)_EOL
//_GLFUNC(void, glVertexAttribL3dv, GLuint index, const GLdouble *v)_EOL
//_GLFUNC(void, glVertexAttribL4dv, GLuint index, const GLdouble *v)_EOL
//_GLFUNC(void, glVertexAttribLPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer)_EOL
//_GLFUNC(void, glGetVertexAttribLdv, GLuint index, GLenum pname, GLdouble *params)_EOL
//_GLFUNC(void, glViewportArrayv, GLuint first, GLsizei count, const GLfloat *v)_EOL
//_GLFUNC(void, glViewportIndexedf, GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h)_EOL
//_GLFUNC(void, glViewportIndexedfv, GLuint index, const GLfloat *v)_EOL
//_GLFUNC(void, glScissorArrayv, GLuint first, GLsizei count, const GLint *v)_EOL
//_GLFUNC(void, glScissorIndexed, GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glScissorIndexedv, GLuint index, const GLint *v)_EOL
//_GLFUNC(void, glDepthRangeArrayv, GLuint first, GLsizei count, const GLdouble *v)_EOL
//_GLFUNC(void, glDepthRangeIndexed, GLuint index, GLdouble n, GLdouble f)_EOL
//_GLFUNC(void, glGetFloati_v, GLenum target, GLuint index, GLfloat *data)_EOL
//_GLFUNC(void, glGetDoublei_v, GLenum target, GLuint index, GLdouble *data)_EOL

// 4.2
//_GLFUNC(void, glDrawArraysInstancedBaseInstance, GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance)_EOL
//_GLFUNC(void, glDrawElementsInstancedBaseInstance, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance)_EOL
//_GLFUNC(void, glDrawElementsInstancedBaseVertexBaseInstance, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance)_EOL
//_GLFUNC(void, glGetInternalformativ, GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params)_EOL
//_GLFUNC(void, glGetActiveAtomicCounterBufferiv, GLuint program, GLuint bufferIndex, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glBindImageTexture, GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)_EOL
//_GLFUNC(void, glMemoryBarrier, GLbitfield barriers)_EOL
//_GLFUNC(void, glTexStorage1D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)_EOL
//_GLFUNC(void, glTexStorage2D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glTexStorage3D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)_EOL
//_GLFUNC(void, glDrawTransformFeedbackInstanced, GLenum mode, GLuint id, GLsizei instancecount)_EOL
//_GLFUNC(void, glDrawTransformFeedbackStreamInstanced, GLenum mode, GLuint id, GLuint stream, GLsizei instancecount)_EOL

// 4.3 (GL_ARB_vertex_attrib_binding, GL_ARB_debug_output)
//_GLFUNC(void, glClearBufferData, GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data)_EOL
//_GLFUNC(void, glClearBufferSubData, GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data)_EOL
//_GLFUNC(void, glDispatchCompute, GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)_EOL
//_GLFUNC(void, glDispatchComputeIndirect, GLintptr indirect)_EOL
//_GLFUNC(void, glCopyImageSubData, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth)_EOL
//_GLFUNC(void, glFramebufferParameteri, GLenum target, GLenum pname, GLint param)_EOL
//_GLFUNC(void, glGetFramebufferParameteriv, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetInternalformati64v, GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 *params)_EOL
//_GLFUNC(void, glInvalidateTexSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth)_EOL
//_GLFUNC(void, glInvalidateTexImage, GLuint texture, GLint level)_EOL
//_GLFUNC(void, glInvalidateBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr length)_EOL
//_GLFUNC(void, glInvalidateBufferData, GLuint buffer)_EOL
//_GLFUNC(void, glInvalidateFramebuffer, GLenum target, GLsizei numAttachments, const GLenum *attachments)_EOL
//_GLFUNC(void, glInvalidateSubFramebuffer, GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glMultiDrawArraysIndirect, GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride)_EOL
//_GLFUNC(void, glMultiDrawElementsIndirect, GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride)_EOL
//_GLFUNC(void, glGetProgramInterfaceiv, GLuint program, GLenum programInterface, GLenum pname, GLint *params)_EOL
//_GLFUNC(GLuint, glGetProgramResourceIndex, GLuint program, GLenum programInterface, const GLchar *name)_EOL
//_GLFUNC(void, glGetProgramResourceName, GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name)_EOL
//_GLFUNC(void, glGetProgramResourceiv, GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params)_EOL
//_GLFUNC(GLint, glGetProgramResourceLocation, GLuint program, GLenum programInterface, const GLchar *name)_EOL
//_GLFUNC(GLint, glGetProgramResourceLocationIndex, GLuint program, GLenum programInterface, const GLchar *name)_EOL
//_GLFUNC(void, glShaderStorageBlockBinding, GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding)_EOL
//_GLFUNC(void, glTexBufferRange, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)_EOL
//_GLFUNC(void, glTexStorage2DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)_EOL
//_GLFUNC(void, glTexStorage3DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)_EOL
//_GLFUNC(void, glTextureView, GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers)_EOL
_GLFUNC(void, glBindVertexBuffer, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)_EOL
_GLFUNC(void, glVertexAttribFormat, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)_EOL
_GLFUNC(void, glVertexAttribIFormat, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)_EOL
//_GLFUNC(void, glVertexAttribLFormat, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)_EOL
_GLFUNC(void, glVertexAttribBinding, GLuint attribindex, GLuint bindingindex)_EOL
_GLFUNC(void, glVertexBindingDivisor, GLuint bindingindex, GLuint divisor)_EOL
#ifdef _DEBUG_GRAPHICS
_GLFUNC(void, glDebugMessageControl, GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled)_EOL
//_GLFUNC(void, glDebugMessageInsert, GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf)_EOL
_GLFUNC(void, glDebugMessageCallback, GLDEBUGPROC callback, const void *userParam)_EOL
//_GLFUNC(GLuint, glGetDebugMessageLog, GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog)_EOL
//_GLFUNC(void, glPushDebugGroup, GLenum source, GLuint id, GLsizei length, const GLchar *message)_EOL
//_GLFUNC(void, glPopDebugGroup, void)_EOL
//_GLFUNC(void, glObjectLabel, GLenum identifier, GLuint name, GLsizei length, const GLchar *label)_EOL
//_GLFUNC(void, glGetObjectLabel, GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label)_EOL
//_GLFUNC(void, glObjectPtrLabel, const void *ptr, GLsizei length, const GLchar *label)_EOL
//_GLFUNC(void, glGetObjectPtrLabel, const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label)_EOL
#endif
// 4.4
//_GLFUNC(void, glBufferStorage, GLenum target, GLsizeiptr size, const void *data, GLbitfield flags)_EOL
//_GLFUNC(void, glClearTexImage, GLuint texture, GLint level, GLenum format, GLenum type, const void *data)_EOL
//_GLFUNC(void, glClearTexSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data)_EOL
//_GLFUNC(void, glBindBuffersBase, GLenum target, GLuint first, GLsizei count, const GLuint *buffers)_EOL
//_GLFUNC(void, glBindBuffersRange, GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes)_EOL
//_GLFUNC(void, glBindTextures, GLuint first, GLsizei count, const GLuint *textures)_EOL
//_GLFUNC(void, glBindSamplers, GLuint first, GLsizei count, const GLuint *samplers)_EOL
//_GLFUNC(void, glBindImageTextures, GLuint first, GLsizei count, const GLuint *textures)_EOL
//_GLFUNC(void, glBindVertexBuffers, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides)_EOL

// 4.5
//_GLFUNC(void, glClipControl, GLenum origin, GLenum depth)_EOL
//_GLFUNC(void, glCreateTransformFeedbacks, GLsizei n, GLuint *ids)_EOL
//_GLFUNC(void, glTransformFeedbackBufferBase, GLuint xfb, GLuint index, GLuint buffer)_EOL
//_GLFUNC(void, glTransformFeedbackBufferRange, GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)_EOL
//_GLFUNC(void, glGetTransformFeedbackiv, GLuint xfb, GLenum pname, GLint *param)_EOL
//_GLFUNC(void, glGetTransformFeedbacki_v, GLuint xfb, GLenum pname, GLuint index, GLint *param)_EOL
//_GLFUNC(void, glGetTransformFeedbacki64_v, GLuint xfb, GLenum pname, GLuint index, GLint64 *param)_EOL
//_GLFUNC(void, glCreateBuffers, GLsizei n, GLuint *buffers)_EOL
//_GLFUNC(void, glNamedBufferStorage, GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags)_EOL
//_GLFUNC(void, glNamedBufferData, GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)_EOL
//_GLFUNC(void, glNamedBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data)_EOL
//_GLFUNC(void, glCopyNamedBufferSubData, GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)_EOL
//_GLFUNC(void, glClearNamedBufferData, GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data)_EOL
//_GLFUNC(void, glClearNamedBufferSubData, GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data)_EOL
//_GLFUNC(void *, glMapNamedBuffer, GLuint buffer, GLenum access)_EOL
//_GLFUNC(void *, glMapNamedBufferRange, GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)_EOL
//_GLFUNC(GLboolean, glUnmapNamedBuffer, GLuint buffer)_EOL
//_GLFUNC(void, glFlushMappedNamedBufferRange, GLuint buffer, GLintptr offset, GLsizeiptr length)_EOL
//_GLFUNC(void, glGetNamedBufferParameteriv, GLuint buffer, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetNamedBufferParameteri64v, GLuint buffer, GLenum pname, GLint64 *params)_EOL
//_GLFUNC(void, glGetNamedBufferPointerv, GLuint buffer, GLenum pname, void **params)_EOL
//_GLFUNC(void, glGetNamedBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr size, void *data)_EOL
//_GLFUNC(void, glCreateFramebuffers, GLsizei n, GLuint *framebuffers)_EOL
//_GLFUNC(void, glNamedFramebufferRenderbuffer, GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)_EOL
//_GLFUNC(void, glNamedFramebufferParameteri, GLuint framebuffer, GLenum pname, GLint param)_EOL
//_GLFUNC(void, glNamedFramebufferTexture, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)_EOL
//_GLFUNC(void, glNamedFramebufferTextureLayer, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer)_EOL
//_GLFUNC(void, glNamedFramebufferDrawBuffer, GLuint framebuffer, GLenum buf)_EOL
//_GLFUNC(void, glNamedFramebufferDrawBuffers, GLuint framebuffer, GLsizei n, const GLenum *bufs)_EOL
//_GLFUNC(void, glNamedFramebufferReadBuffer, GLuint framebuffer, GLenum src)_EOL
//_GLFUNC(void, glInvalidateNamedFramebufferData, GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments)_EOL
//_GLFUNC(void, glInvalidateNamedFramebufferSubData, GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glClearNamedFramebufferiv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value)_EOL
//_GLFUNC(void, glClearNamedFramebufferuiv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value)_EOL
//_GLFUNC(void, glClearNamedFramebufferfv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value)_EOL
//_GLFUNC(void, glClearNamedFramebufferfi, GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)_EOL
//_GLFUNC(void, glBlitNamedFramebuffer, GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)_EOL
#ifdef _DEBUG_GRAPHICS
//_GLFUNC(GLenum, glCheckNamedFramebufferStatus, GLuint framebuffer, GLenum target)_EOL
#endif
//_GLFUNC(void, glGetNamedFramebufferParameteriv, GLuint framebuffer, GLenum pname, GLint *param)_EOL
//_GLFUNC(void, glGetNamedFramebufferAttachmentParameteriv, GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glCreateRenderbuffers, GLsizei n, GLuint *renderbuffers)_EOL
//_GLFUNC(void, glNamedRenderbufferStorage, GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glNamedRenderbufferStorageMultisample, GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glGetNamedRenderbufferParameteriv, GLuint renderbuffer, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glCreateTextures, GLenum target, GLsizei n, GLuint *textures)_EOL
//_GLFUNC(void, glTextureBuffer, GLuint texture, GLenum internalformat, GLuint buffer)_EOL
//_GLFUNC(void, glTextureBufferRange, GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)_EOL
//_GLFUNC(void, glTextureStorage1D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width)_EOL
//_GLFUNC(void, glTextureStorage2D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glTextureStorage3D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)_EOL
//_GLFUNC(void, glTextureStorage2DMultisample, GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)_EOL
//_GLFUNC(void, glTextureStorage3DMultisample, GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)_EOL
//_GLFUNC(void, glTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glCompressedTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data)_EOL
//_GLFUNC(void, glCompressedTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)_EOL
//_GLFUNC(void, glCompressedTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data)_EOL
//_GLFUNC(void, glCopyTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)_EOL
//_GLFUNC(void, glCopyTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glCopyTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glTextureParameterf, GLuint texture, GLenum pname, GLfloat param)_EOL
//_GLFUNC(void, glTextureParameterfv, GLuint texture, GLenum pname, const GLfloat *param)_EOL
//_GLFUNC(void, glTextureParameteri, GLuint texture, GLenum pname, GLint param)_EOL
//_GLFUNC(void, glTextureParameterIiv, GLuint texture, GLenum pname, const GLint *params)_EOL
//_GLFUNC(void, glTextureParameterIuiv, GLuint texture, GLenum pname, const GLuint *params)_EOL
//_GLFUNC(void, glTextureParameteriv, GLuint texture, GLenum pname, const GLint *param)_EOL
//_GLFUNC(void, glGenerateTextureMipmap, GLuint texture)_EOL
//_GLFUNC(void, glBindTextureUnit, GLuint unit, GLuint texture)_EOL
//_GLFUNC(void, glGetTextureImage, GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels)_EOL
//_GLFUNC(void, glGetCompressedTextureImage, GLuint texture, GLint level, GLsizei bufSize, void *pixels)_EOL
//_GLFUNC(void, glGetTextureLevelParameterfv, GLuint texture, GLint level, GLenum pname, GLfloat *params)_EOL
//_GLFUNC(void, glGetTextureLevelParameteriv, GLuint texture, GLint level, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetTextureParameterfv, GLuint texture, GLenum pname, GLfloat *params)_EOL
//_GLFUNC(void, glGetTextureParameterIiv, GLuint texture, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetTextureParameterIuiv, GLuint texture, GLenum pname, GLuint *params)_EOL
//_GLFUNC(void, glGetTextureParameteriv, GLuint texture, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glCreateVertexArrays, GLsizei n, GLuint *arrays)_EOL
//_GLFUNC(void, glDisableVertexArrayAttrib, GLuint vaobj, GLuint index)_EOL
//_GLFUNC(void, glEnableVertexArrayAttrib, GLuint vaobj, GLuint index)_EOL
//_GLFUNC(void, glVertexArrayElementBuffer, GLuint vaobj, GLuint buffer)_EOL
//_GLFUNC(void, glVertexArrayVertexBuffer, GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)_EOL
//_GLFUNC(void, glVertexArrayVertexBuffers, GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides)_EOL
//_GLFUNC(void, glVertexArrayAttribBinding, GLuint vaobj, GLuint attribindex, GLuint bindingindex)_EOL
//_GLFUNC(void, glVertexArrayAttribFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)_EOL
//_GLFUNC(void, glVertexArrayAttribIFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)_EOL
//_GLFUNC(void, glVertexArrayAttribLFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)_EOL
//_GLFUNC(void, glVertexArrayBindingDivisor, GLuint vaobj, GLuint bindingindex, GLuint divisor)_EOL
//_GLFUNC(void, glGetVertexArrayiv, GLuint vaobj, GLenum pname, GLint *param)_EOL
//_GLFUNC(void, glGetVertexArrayIndexediv, GLuint vaobj, GLuint index, GLenum pname, GLint *param)_EOL
//_GLFUNC(void, glGetVertexArrayIndexed64iv, GLuint vaobj, GLuint index, GLenum pname, GLint64 *param)_EOL
//_GLFUNC(void, glCreateSamplers, GLsizei n, GLuint *samplers)_EOL
//_GLFUNC(void, glCreateProgramPipelines, GLsizei n, GLuint *pipelines)_EOL
//_GLFUNC(void, glCreateQueries, GLenum target, GLsizei n, GLuint *ids)_EOL
//_GLFUNC(void, glGetQueryBufferObjecti64v, GLuint id, GLuint buffer, GLenum pname, GLintptr offset)_EOL
//_GLFUNC(void, glGetQueryBufferObjectiv, GLuint id, GLuint buffer, GLenum pname, GLintptr offset)_EOL
//_GLFUNC(void, glGetQueryBufferObjectui64v, GLuint id, GLuint buffer, GLenum pname, GLintptr offset)_EOL
//_GLFUNC(void, glGetQueryBufferObjectuiv, GLuint id, GLuint buffer, GLenum pname, GLintptr offset)_EOL
//_GLFUNC(void, glMemoryBarrierByRegion, GLbitfield barriers)_EOL
//_GLFUNC(void, glGetTextureSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels)_EOL
//_GLFUNC(void, glGetCompressedTextureSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels)_EOL
//_GLFUNC(GLenum, glGetGraphicsResetStatus, void)_EOL
//_GLFUNC(void, glGetnCompressedTexImage, GLenum target, GLint lod, GLsizei bufSize, void *pixels)_EOL
//_GLFUNC(void, glGetnTexImage, GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels)_EOL
//_GLFUNC(void, glGetnUniformdv, GLuint program, GLint location, GLsizei bufSize, GLdouble *params)_EOL
//_GLFUNC(void, glGetnUniformfv, GLuint program, GLint location, GLsizei bufSize, GLfloat *params)_EOL
//_GLFUNC(void, glGetnUniformiv, GLuint program, GLint location, GLsizei bufSize, GLint *params)_EOL
//_GLFUNC(void, glGetnUniformuiv, GLuint program, GLint location, GLsizei bufSize, GLuint *params)_EOL
//_GLFUNC(void, glReadnPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data)_EOL
//_GLFUNC(void, glTextureBarrier, void)_EOL

// GL_EXT_direct_state_access
//_GLFUNC(void, glMatrixLoadfEXT, GLenum mode, const GLfloat *m)_EOL
//_GLFUNC(void, glMatrixLoaddEXT, GLenum mode, const GLdouble *m)_EOL
//_GLFUNC(void, glMatrixMultfEXT, GLenum mode, const GLfloat *m)_EOL
//_GLFUNC(void, glMatrixMultdEXT, GLenum mode, const GLdouble *m)_EOL
//_GLFUNC(void, glMatrixLoadIdentityEXT, GLenum mode)_EOL
//_GLFUNC(void, glMatrixRotatefEXT, GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)_EOL
//_GLFUNC(void, glMatrixRotatedEXT, GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z)_EOL
//_GLFUNC(void, glMatrixScalefEXT, GLenum mode, GLfloat x, GLfloat y, GLfloat z)_EOL
//_GLFUNC(void, glMatrixScaledEXT, GLenum mode, GLdouble x, GLdouble y, GLdouble z)_EOL
//_GLFUNC(void, glMatrixTranslatefEXT, GLenum mode, GLfloat x, GLfloat y, GLfloat z)_EOL
//_GLFUNC(void, glMatrixTranslatedEXT, GLenum mode, GLdouble x, GLdouble y, GLdouble z)_EOL
//_GLFUNC(void, glMatrixFrustumEXT, GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)_EOL
//_GLFUNC(void, glMatrixOrthoEXT, GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)_EOL
//_GLFUNC(void, glMatrixPopEXT, GLenum mode)_EOL
//_GLFUNC(void, glMatrixPushEXT, GLenum mode)_EOL
//_GLFUNC(void, glClientAttribDefaultEXT, GLbitfield mask)_EOL
//_GLFUNC(void, glPushClientAttribDefaultEXT, GLbitfield mask)_EOL
//_GLFUNC(void, glTextureParameterfEXT, GLuint texture, GLenum target, GLenum pname, GLfloat param)_EOL
//_GLFUNC(void, glTextureParameterfvEXT, GLuint texture, GLenum target, GLenum pname, const GLfloat *params)_EOL
_GLFUNC(void, glTextureParameteriEXT, GLuint texture, GLenum target, GLenum pname, GLint param)_EOL
//_GLFUNC(void, glTextureParameterivEXT, GLuint texture, GLenum target, GLenum pname, const GLint *params)_EOL
//_GLFUNC(void, glTextureImage1DEXT, GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glTextureImage2DEXT, GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glTextureSubImage1DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels)_EOL
_GLFUNC(void, glTextureSubImage2DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glCopyTextureImage1DEXT, GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)_EOL
//_GLFUNC(void, glCopyTextureImage2DEXT, GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)_EOL
//_GLFUNC(void, glCopyTextureSubImage1DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)_EOL
//_GLFUNC(void, glCopyTextureSubImage2DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)_EOL
_GLFUNC(void, glGetTextureImageEXT, GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void *pixels)_EOL
//_GLFUNC(void, glGetTextureParameterfvEXT, GLuint texture, GLenum target, GLenum pname, GLfloat *params)_EOL
//_GLFUNC(void, glGetTextureParameterivEXT, GLuint texture, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetTextureLevelParameterfvEXT, GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat *params)_EOL
//_GLFUNC(void, glGetTextureLevelParameterivEXT, GLuint texture, GLenum target, GLint level, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glTextureImage3DEXT, GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels)_EOL
_GLFUNC(void, glTextureSubImage3DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glCopyTextureSubImage3DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)_EOL
_GLFUNC(void, glBindMultiTextureEXT, GLenum texunit, GLenum target, GLuint texture)_EOL
//_GLFUNC(void, glMultiTexCoordPointerEXT, GLenum texunit, GLint size, GLenum type, GLsizei stride, const void *pointer)_EOL
//_GLFUNC(void, glMultiTexEnvfEXT, GLenum texunit, GLenum target, GLenum pname, GLfloat param)_EOL
//_GLFUNC(void, glMultiTexEnvfvEXT, GLenum texunit, GLenum target, GLenum pname, const GLfloat *params)_EOL
//_GLFUNC(void, glMultiTexEnviEXT, GLenum texunit, GLenum target, GLenum pname, GLint param)_EOL
//_GLFUNC(void, glMultiTexEnvivEXT, GLenum texunit, GLenum target, GLenum pname, const GLint *params)_EOL
//_GLFUNC(void, glMultiTexGendEXT, GLenum texunit, GLenum coord, GLenum pname, GLdouble param)_EOL
//_GLFUNC(void, glMultiTexGendvEXT, GLenum texunit, GLenum coord, GLenum pname, const GLdouble *params)_EOL
//_GLFUNC(void, glMultiTexGenfEXT, GLenum texunit, GLenum coord, GLenum pname, GLfloat param)_EOL
//_GLFUNC(void, glMultiTexGenfvEXT, GLenum texunit, GLenum coord, GLenum pname, const GLfloat *params)_EOL
//_GLFUNC(void, glMultiTexGeniEXT, GLenum texunit, GLenum coord, GLenum pname, GLint param)_EOL
//_GLFUNC(void, glMultiTexGenivEXT, GLenum texunit, GLenum coord, GLenum pname, const GLint *params)_EOL
//_GLFUNC(void, glGetMultiTexEnvfvEXT, GLenum texunit, GLenum target, GLenum pname, GLfloat *params)_EOL
//_GLFUNC(void, glGetMultiTexEnvivEXT, GLenum texunit, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetMultiTexGendvEXT, GLenum texunit, GLenum coord, GLenum pname, GLdouble *params)_EOL
//_GLFUNC(void, glGetMultiTexGenfvEXT, GLenum texunit, GLenum coord, GLenum pname, GLfloat *params)_EOL
//_GLFUNC(void, glGetMultiTexGenivEXT, GLenum texunit, GLenum coord, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glMultiTexParameteriEXT, GLenum texunit, GLenum target, GLenum pname, GLint param)_EOL
//_GLFUNC(void, glMultiTexParameterivEXT, GLenum texunit, GLenum target, GLenum pname, const GLint *params)_EOL
//_GLFUNC(void, glMultiTexParameterfEXT, GLenum texunit, GLenum target, GLenum pname, GLfloat param)_EOL
//_GLFUNC(void, glMultiTexParameterfvEXT, GLenum texunit, GLenum target, GLenum pname, const GLfloat *params)_EOL
//_GLFUNC(void, glMultiTexImage1DEXT, GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glMultiTexImage2DEXT, GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glMultiTexSubImage1DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glMultiTexSubImage2DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glCopyMultiTexImage1DEXT, GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)_EOL
//_GLFUNC(void, glCopyMultiTexImage2DEXT, GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)_EOL
//_GLFUNC(void, glCopyMultiTexSubImage1DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)_EOL
//_GLFUNC(void, glCopyMultiTexSubImage2DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glGetMultiTexImageEXT, GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void *pixels)_EOL
//_GLFUNC(void, glGetMultiTexParameterfvEXT, GLenum texunit, GLenum target, GLenum pname, GLfloat *params)_EOL
//_GLFUNC(void, glGetMultiTexParameterivEXT, GLenum texunit, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetMultiTexLevelParameterfvEXT, GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat *params)_EOL
//_GLFUNC(void, glGetMultiTexLevelParameterivEXT, GLenum texunit, GLenum target, GLint level, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glMultiTexImage3DEXT, GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glMultiTexSubImage3DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)_EOL
//_GLFUNC(void, glCopyMultiTexSubImage3DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glEnableClientStateIndexedEXT, GLenum array, GLuint index)_EOL
//_GLFUNC(void, glDisableClientStateIndexedEXT, GLenum array, GLuint index)_EOL
//_GLFUNC(void, glGetFloatIndexedvEXT, GLenum target, GLuint index, GLfloat *data)_EOL
//_GLFUNC(void, glGetDoubleIndexedvEXT, GLenum target, GLuint index, GLdouble *data)_EOL
//_GLFUNC(void, glGetPointerIndexedvEXT, GLenum target, GLuint index, void **data)_EOL
//_GLFUNC(void, glEnableIndexedEXT, GLenum target, GLuint index)_EOL
//_GLFUNC(void, glDisableIndexedEXT, GLenum target, GLuint index)_EOL
//_GLFUNC(GLboolean, glIsEnabledIndexedEXT, GLenum target, GLuint index)_EOL
//_GLFUNC(void, glGetIntegerIndexedvEXT, GLenum target, GLuint index, GLint *data)_EOL
//_GLFUNC(void, glGetBooleanIndexedvEXT, GLenum target, GLuint index, GLboolean *data)_EOL
//_GLFUNC(void, glCompressedTextureImage3DEXT, GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *bits)_EOL
//_GLFUNC(void, glCompressedTextureImage2DEXT, GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *bits)_EOL
//_GLFUNC(void, glCompressedTextureImage1DEXT, GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *bits)_EOL
_GLFUNC(void, glCompressedTextureSubImage3DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *bits)_EOL
_GLFUNC(void, glCompressedTextureSubImage2DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *bits)_EOL
//_GLFUNC(void, glCompressedTextureSubImage1DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *bits)_EOL
//_GLFUNC(void, glGetCompressedTextureImageEXT, GLuint texture, GLenum target, GLint lod, void *img)_EOL
//_GLFUNC(void, glCompressedMultiTexImage3DEXT, GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *bits)_EOL
//_GLFUNC(void, glCompressedMultiTexImage2DEXT, GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *bits)_EOL
//_GLFUNC(void, glCompressedMultiTexImage1DEXT, GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *bits)_EOL
//_GLFUNC(void, glCompressedMultiTexSubImage3DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *bits)_EOL
//_GLFUNC(void, glCompressedMultiTexSubImage2DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *bits)_EOL
//_GLFUNC(void, glCompressedMultiTexSubImage1DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *bits)_EOL
//_GLFUNC(void, glGetCompressedMultiTexImageEXT, GLenum texunit, GLenum target, GLint lod, void *img)_EOL
//_GLFUNC(void, glMatrixLoadTransposefEXT, GLenum mode, const GLfloat *m)_EOL
//_GLFUNC(void, glMatrixLoadTransposedEXT, GLenum mode, const GLdouble *m)_EOL
//_GLFUNC(void, glMatrixMultTransposefEXT, GLenum mode, const GLfloat *m)_EOL
//_GLFUNC(void, glMatrixMultTransposedEXT, GLenum mode, const GLdouble *m)_EOL
_GLFUNC(void, glNamedBufferDataEXT, GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)_EOL
_GLFUNC(void, glNamedBufferSubDataEXT, GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data)_EOL
//_GLFUNC(void *, glMapNamedBufferEXT, GLuint buffer, GLenum access)_EOL
_GLFUNC(GLboolean, glUnmapNamedBufferEXT, GLuint buffer)_EOL
//_GLFUNC(void, glGetNamedBufferParameterivEXT, GLuint buffer, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetNamedBufferPointervEXT, GLuint buffer, GLenum pname, void **params)_EOL
//_GLFUNC(void, glGetNamedBufferSubDataEXT, GLuint buffer, GLintptr offset, GLsizeiptr size, void *data)_EOL
//_GLFUNC(void, glProgramUniform1fEXT, GLuint program, GLint location, GLfloat v0)_EOL
//_GLFUNC(void, glProgramUniform2fEXT, GLuint program, GLint location, GLfloat v0, GLfloat v1)_EOL
//_GLFUNC(void, glProgramUniform3fEXT, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)_EOL
//_GLFUNC(void, glProgramUniform4fEXT, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)_EOL
//_GLFUNC(void, glProgramUniform1iEXT, GLuint program, GLint location, GLint v0)_EOL
//_GLFUNC(void, glProgramUniform2iEXT, GLuint program, GLint location, GLint v0, GLint v1)_EOL
//_GLFUNC(void, glProgramUniform3iEXT, GLuint program, GLint location, GLint v0, GLint v1, GLint v2)_EOL
//_GLFUNC(void, glProgramUniform4iEXT, GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)_EOL
//_GLFUNC(void, glProgramUniform1fvEXT, GLuint program, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniform2fvEXT, GLuint program, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniform3fvEXT, GLuint program, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniform4fvEXT, GLuint program, GLint location, GLsizei count, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniform1ivEXT, GLuint program, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glProgramUniform2ivEXT, GLuint program, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glProgramUniform3ivEXT, GLuint program, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glProgramUniform4ivEXT, GLuint program, GLint location, GLsizei count, const GLint *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2x3fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3x2fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2x4fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4x2fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3x4fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4x3fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)_EOL
//_GLFUNC(void, glTextureBufferEXT, GLuint texture, GLenum target, GLenum internalformat, GLuint buffer)_EOL
//_GLFUNC(void, glMultiTexBufferEXT, GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer)_EOL
//_GLFUNC(void, glTextureParameterIivEXT, GLuint texture, GLenum target, GLenum pname, const GLint *params)_EOL
//_GLFUNC(void, glTextureParameterIuivEXT, GLuint texture, GLenum target, GLenum pname, const GLuint *params)_EOL
//_GLFUNC(void, glGetTextureParameterIivEXT, GLuint texture, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetTextureParameterIuivEXT, GLuint texture, GLenum target, GLenum pname, GLuint *params)_EOL
//_GLFUNC(void, glMultiTexParameterIivEXT, GLenum texunit, GLenum target, GLenum pname, const GLint *params)_EOL
//_GLFUNC(void, glMultiTexParameterIuivEXT, GLenum texunit, GLenum target, GLenum pname, const GLuint *params)_EOL
//_GLFUNC(void, glGetMultiTexParameterIivEXT, GLenum texunit, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetMultiTexParameterIuivEXT, GLenum texunit, GLenum target, GLenum pname, GLuint *params)_EOL
//_GLFUNC(void, glProgramUniform1uiEXT, GLuint program, GLint location, GLuint v0)_EOL
//_GLFUNC(void, glProgramUniform2uiEXT, GLuint program, GLint location, GLuint v0, GLuint v1)_EOL
//_GLFUNC(void, glProgramUniform3uiEXT, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2)_EOL
//_GLFUNC(void, glProgramUniform4uiEXT, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)_EOL
//_GLFUNC(void, glProgramUniform1uivEXT, GLuint program, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glProgramUniform2uivEXT, GLuint program, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glProgramUniform3uivEXT, GLuint program, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glProgramUniform4uivEXT, GLuint program, GLint location, GLsizei count, const GLuint *value)_EOL
//_GLFUNC(void, glNamedProgramLocalParameters4fvEXT, GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat *params)_EOL
//_GLFUNC(void, glNamedProgramLocalParameterI4iEXT, GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w)_EOL
//_GLFUNC(void, glNamedProgramLocalParameterI4ivEXT, GLuint program, GLenum target, GLuint index, const GLint *params)_EOL
//_GLFUNC(void, glNamedProgramLocalParametersI4ivEXT, GLuint program, GLenum target, GLuint index, GLsizei count, const GLint *params)_EOL
//_GLFUNC(void, glNamedProgramLocalParameterI4uiEXT, GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)_EOL
//_GLFUNC(void, glNamedProgramLocalParameterI4uivEXT, GLuint program, GLenum target, GLuint index, const GLuint *params)_EOL
//_GLFUNC(void, glNamedProgramLocalParametersI4uivEXT, GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint *params)_EOL
//_GLFUNC(void, glGetNamedProgramLocalParameterIivEXT, GLuint program, GLenum target, GLuint index, GLint *params)_EOL
//_GLFUNC(void, glGetNamedProgramLocalParameterIuivEXT, GLuint program, GLenum target, GLuint index, GLuint *params)_EOL
//_GLFUNC(void, glEnableClientStateiEXT, GLenum array, GLuint index)_EOL
//_GLFUNC(void, glDisableClientStateiEXT, GLenum array, GLuint index)_EOL
//_GLFUNC(void, glGetFloati_vEXT, GLenum pname, GLuint index, GLfloat *params)_EOL
//_GLFUNC(void, glGetDoublei_vEXT, GLenum pname, GLuint index, GLdouble *params)_EOL
//_GLFUNC(void, glGetPointeri_vEXT, GLenum pname, GLuint index, void **params)_EOL
//_GLFUNC(void, glNamedProgramStringEXT, GLuint program, GLenum target, GLenum format, GLsizei len, const void *string)_EOL
//_GLFUNC(void, glNamedProgramLocalParameter4dEXT, GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)_EOL
//_GLFUNC(void, glNamedProgramLocalParameter4dvEXT, GLuint program, GLenum target, GLuint index, const GLdouble *params)_EOL
//_GLFUNC(void, glNamedProgramLocalParameter4fEXT, GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)_EOL
//_GLFUNC(void, glNamedProgramLocalParameter4fvEXT, GLuint program, GLenum target, GLuint index, const GLfloat *params)_EOL
//_GLFUNC(void, glGetNamedProgramLocalParameterdvEXT, GLuint program, GLenum target, GLuint index, GLdouble *params)_EOL
//_GLFUNC(void, glGetNamedProgramLocalParameterfvEXT, GLuint program, GLenum target, GLuint index, GLfloat *params)_EOL
//_GLFUNC(void, glGetNamedProgramivEXT, GLuint program, GLenum target, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glGetNamedProgramStringEXT, GLuint program, GLenum target, GLenum pname, void *string)_EOL
//_GLFUNC(void, glNamedRenderbufferStorageEXT, GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glGetNamedRenderbufferParameterivEXT, GLuint renderbuffer, GLenum pname, GLint *params)_EOL
_GLFUNC(void, glNamedRenderbufferStorageMultisampleEXT, GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)_EOL
//_GLFUNC(void, glNamedRenderbufferStorageMultisampleCoverageEXT, GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height)_EOL
#ifdef _DEBUG_GRAPHICS
_GLFUNC(GLenum, glCheckNamedFramebufferStatusEXT, GLuint framebuffer, GLenum target)_EOL
#endif
//_GLFUNC(void, glNamedFramebufferTexture1DEXT, GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level)_EOL
_GLFUNC(void, glNamedFramebufferTexture2DEXT, GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level)_EOL
_GLFUNC(void, glNamedFramebufferTexture3DEXT, GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)_EOL
_GLFUNC(void, glNamedFramebufferRenderbufferEXT, GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)_EOL
//_GLFUNC(void, glGetNamedFramebufferAttachmentParameterivEXT, GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params)_EOL
_GLFUNC(void, glGenerateTextureMipmapEXT, GLuint texture, GLenum target)_EOL
//_GLFUNC(void, glGenerateMultiTexMipmapEXT, GLenum texunit, GLenum target)_EOL
//_GLFUNC(void, glFramebufferDrawBufferEXT, GLuint framebuffer, GLenum mode)_EOL
_GLFUNC(void, glFramebufferDrawBuffersEXT, GLuint framebuffer, GLsizei n, const GLenum *bufs)_EOL
//_GLFUNC(void, glFramebufferReadBufferEXT, GLuint framebuffer, GLenum mode)_EOL
//_GLFUNC(void, glGetFramebufferParameterivEXT, GLuint framebuffer, GLenum pname, GLint *params)_EOL
_GLFUNC(void, glNamedCopyBufferSubDataEXT, GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)_EOL
_GLFUNC(void, glNamedFramebufferTextureEXT, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)_EOL
_GLFUNC(void, glNamedFramebufferTextureLayerEXT, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer)_EOL
//_GLFUNC(void, glNamedFramebufferTextureFaceEXT, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face)_EOL
//_GLFUNC(void, glTextureRenderbufferEXT, GLuint texture, GLenum target, GLuint renderbuffer)_EOL
//_GLFUNC(void, glMultiTexRenderbufferEXT, GLenum texunit, GLenum target, GLuint renderbuffer)_EOL
//_GLFUNC(void, glVertexArrayVertexOffsetEXT, GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glVertexArrayColorOffsetEXT, GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glVertexArrayEdgeFlagOffsetEXT, GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glVertexArrayIndexOffsetEXT, GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glVertexArrayNormalOffsetEXT, GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glVertexArrayTexCoordOffsetEXT, GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glVertexArrayMultiTexCoordOffsetEXT, GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glVertexArrayFogCoordOffsetEXT, GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glVertexArraySecondaryColorOffsetEXT, GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glVertexArrayVertexAttribOffsetEXT, GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glVertexArrayVertexAttribIOffsetEXT, GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glEnableVertexArrayEXT, GLuint vaobj, GLenum array)_EOL
//_GLFUNC(void, glDisableVertexArrayEXT, GLuint vaobj, GLenum array)_EOL
//_GLFUNC(void, glEnableVertexArrayAttribEXT, GLuint vaobj, GLuint index)_EOL
//_GLFUNC(void, glDisableVertexArrayAttribEXT, GLuint vaobj, GLuint index)_EOL
//_GLFUNC(void, glGetVertexArrayIntegervEXT, GLuint vaobj, GLenum pname, GLint *param)_EOL
//_GLFUNC(void, glGetVertexArrayPointervEXT, GLuint vaobj, GLenum pname, void **param)_EOL
//_GLFUNC(void, glGetVertexArrayIntegeri_vEXT, GLuint vaobj, GLuint index, GLenum pname, GLint *param)_EOL
//_GLFUNC(void, glGetVertexArrayPointeri_vEXT, GLuint vaobj, GLuint index, GLenum pname, void **param)_EOL
_GLFUNC(void *, glMapNamedBufferRangeEXT, GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)_EOL
//_GLFUNC(void, glFlushMappedNamedBufferRangeEXT, GLuint buffer, GLintptr offset, GLsizeiptr length)_EOL
//_GLFUNC(void, glNamedBufferStorageEXT, GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags)_EOL
//_GLFUNC(void, glClearNamedBufferDataEXT, GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data)_EOL
//_GLFUNC(void, glClearNamedBufferSubDataEXT, GLuint buffer, GLenum internalformat, GLsizeiptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data)_EOL
//_GLFUNC(void, glNamedFramebufferParameteriEXT, GLuint framebuffer, GLenum pname, GLint param)_EOL
//_GLFUNC(void, glGetNamedFramebufferParameterivEXT, GLuint framebuffer, GLenum pname, GLint *params)_EOL
//_GLFUNC(void, glProgramUniform1dEXT, GLuint program, GLint location, GLdouble x)_EOL
//_GLFUNC(void, glProgramUniform2dEXT, GLuint program, GLint location, GLdouble x, GLdouble y)_EOL
//_GLFUNC(void, glProgramUniform3dEXT, GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z)_EOL
//_GLFUNC(void, glProgramUniform4dEXT, GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w)_EOL
//_GLFUNC(void, glProgramUniform1dvEXT, GLuint program, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniform2dvEXT, GLuint program, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniform3dvEXT, GLuint program, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniform4dvEXT, GLuint program, GLint location, GLsizei count, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2x3dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix2x4dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3x2dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix3x4dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4x2dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glProgramUniformMatrix4x3dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)_EOL
//_GLFUNC(void, glTextureBufferRangeEXT, GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)_EOL
//_GLFUNC(void, glTextureStorage1DEXT, GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)_EOL
_GLFUNC(void, glTextureStorage2DEXT, GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)_EOL
_GLFUNC(void, glTextureStorage3DEXT, GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)_EOL
//_GLFUNC(void, glTextureStorage2DMultisampleEXT, GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)_EOL
//_GLFUNC(void, glTextureStorage3DMultisampleEXT, GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)_EOL
//_GLFUNC(void, glVertexArrayBindVertexBufferEXT, GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)_EOL
//_GLFUNC(void, glVertexArrayVertexAttribFormatEXT, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)_EOL
//_GLFUNC(void, glVertexArrayVertexAttribIFormatEXT, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)_EOL
//_GLFUNC(void, glVertexArrayVertexAttribLFormatEXT, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)_EOL
//_GLFUNC(void, glVertexArrayVertexAttribBindingEXT, GLuint vaobj, GLuint attribindex, GLuint bindingindex)_EOL
//_GLFUNC(void, glVertexArrayVertexBindingDivisorEXT, GLuint vaobj, GLuint bindingindex, GLuint divisor)_EOL
//_GLFUNC(void, glVertexArrayVertexAttribLOffsetEXT, GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset)_EOL
//_GLFUNC(void, glTexturePageCommitmentEXT, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit)_EOL
//_GLFUNC(void, glVertexArrayVertexAttribDivisorEXT, GLuint vaobj, GLuint index, GLuint divisor)_EOL

// WGL_EXT_swap_control
_GLFUNC(BOOL, wglSwapIntervalEXT, int interval)_EOL
_GLFUNC(int, wglGetSwapIntervalEXT, void)_EOL

#undef _EOL
#undef _GLFUNC

