#pragma once

#include "Graphics.hpp"
#include "Assets.hpp"
#include "Device.hpp"

//----------------------------------------------------------------------------//
// GLDebug
//----------------------------------------------------------------------------//

#ifdef _DEBUG_GRAPHICS

struct GLDebug : public NonCopyable
{
	static bool Enabled;

	GLDebug(const char* _func, const char* _file, int _line, const char* _desc = nullptr) :
		func(_func),
		desc(_desc),
		file(_file),
		line(_line),
		prev(Top)
	{
		Top = this;
	}

	~GLDebug(void)
	{
		Top = prev;
	}

	static void __stdcall OnMessage(uint _source, uint _type, uint _id, uint _severity, int _length, const char* _message, const void* _ud)
	{
		const char* _src;
		const char* _typeStr;
		const char* _levelStr;
		int  _level = 0;
		const char* _func = nullptr;
		const char* _file = nullptr;
		int _line = 0;

		switch (_source)
		{
		case GL_DEBUG_SOURCE_API_ARB:
			_src = "OpenGL";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
			_src = "Window system";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
			_src = "Shader compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
			_src = "Third party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION_ARB:
			_src = "Application";
			break;
		case GL_DEBUG_SOURCE_OTHER_ARB:
			_src = "Other";
			break;
		default:
			return;
		}

		switch (_type)
		{
		case GL_DEBUG_TYPE_ERROR_ARB:
			_typeStr = "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
			_typeStr = "Deprecated behavior";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
			_typeStr = "Undefined behavior";
			break;
		case GL_DEBUG_TYPE_PORTABILITY_ARB:
			_typeStr = "Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE_ARB:
			_typeStr = "Performance";
			break;
		case GL_DEBUG_TYPE_OTHER_ARB:
			_typeStr = "Message";
			break;
#if 0
		case GL_DEBUG_TYPE_MARKER:
			_dtype = "Marker";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			_dtype = "Push group";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			_dtype = "Pop group";
			break;
#endif
		default:
			return;
		}

		switch (_severity)
		{
		case GL_DEBUG_SEVERITY_HIGH_ARB:
			_levelStr = "high";
			_level = 1;
			break;
		case GL_DEBUG_SEVERITY_MEDIUM_ARB:
			_levelStr = "medium";
			_level = 0;
			break;
		case GL_DEBUG_SEVERITY_LOW_ARB:
			_levelStr = "low";
			_level = -1;
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			_levelStr = "notification";
			_level = -2;
			break;
		default:
			return;
		}

		if (_level >= 0 && Enabled)
		{
			if (Top)
			{
				if(Top->desc)
					LOG("%s(%d)[%s [%s]]: %s: %s(%s): %s", Top->file, Top->line, Top->func, Top->desc, _src, _typeStr, _levelStr, _message);
				else
					LOG("%s(%d)[%s]: %s: %s(%s): %s", Top->file, Top->line, Top->func, _src, _typeStr, _levelStr, _message);
			}
			else
				LOG("%s: %s(%s): %s", _src, _typeStr, _levelStr, _message);
		}
	}

	static void _Init(void)
	{
		if (glDebugMessageCallback)
		{
			Enabled = true;
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(&OnMessage, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, false);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, true);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, true);
		}
	}

	const char* func;
	const char* desc;
	const char* file;
	int line;
	GLDebug* prev;
	static GLDebug* Top;
};

bool GLDebug::Enabled = false;
GLDebug* GLDebug::Top = nullptr;

#endif//_DEBUG_GRAPHICS

//----------------------------------------------------------------------------//
// GL_DEBUG helper
//----------------------------------------------------------------------------//

#ifdef _DEBUG_GRAPHICS
#	define GL_DEBUG(...) GLDebug _gldbg_(__FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#	define GL_DEBUG(...)
#endif

//----------------------------------------------------------------------------//
// Utils
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void glEnableEx(uint _state, bool _enabled)
{
	_enabled ? glEnable(_state) : glDisable(_state);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// PrimitiveType
//----------------------------------------------------------------------------//

const uint16 PrimitiveType::GLTypes[] =
{
	0,
	GL_POINTS, // Points = 1,
	GL_LINES, // Lines = 2,
	GL_TRIANGLES, // Triangles = 3,
};

//----------------------------------------------------------------------------//
// DepthState
//----------------------------------------------------------------------------//

const DepthState::Desc DepthState::Variants[] =
{
	{ false, true, CompareFunc::LessEqual, false, 0xff, CompareFunc::Always, StencilOp::Keep, StencilOp::Keep, StencilOp::Keep }, // Default
	{ false, false, CompareFunc::LessEqual, false, 0xff, CompareFunc::Always, StencilOp::Keep, StencilOp::Keep, StencilOp::Keep }, // Disabled
	{ true, true, CompareFunc::LessEqual, false, 0xff, CompareFunc::Always, StencilOp::Keep, StencilOp::Keep, StencilOp::Keep },	// Enabled
	{ false, false, CompareFunc::LessEqual, true, 0xff, CompareFunc::Always, StencilOp::Replace, StencilOp::Replace, StencilOp::Replace }, // StencilSet
	{ false, false, CompareFunc::LessEqual, true, 0xff, CompareFunc::Equal, StencilOp::Keep, StencilOp::Keep, StencilOp::Keep },	// StencilTest
};

//----------------------------------------------------------------------------//
// Buffer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Buffer::Buffer(BufferUsage _usage) : m_usage(_usage)
{
	glGenBuffers(1, &m_handle);
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);

#ifdef _STATISTICS
	gGraphics->stats.bufferCount++;
#endif
}
//----------------------------------------------------------------------------//
Buffer::~Buffer(void)
{
	GL_DEBUG();
	if(m_handle)
		glDeleteBuffers(1, &m_handle);
#ifdef _STATISTICS
	gGraphics->stats.bufferMemory -= m_capacity;
	gGraphics->stats.bufferCount--;
#endif
}
//----------------------------------------------------------------------------//
void Buffer::Realloc(uint _newSize, const void* _data)
{
	GL_DEBUG();

	if (!_newSize && _data)
		_data = nullptr;

	if (m_capacity < _newSize || (m_usage == BufferUsage::Static && m_capacity != _newSize))
	{
#ifdef _STATISTICS
	gGraphics->stats.bufferMemory -= m_capacity;
	gGraphics->stats.bufferMemory += _newSize;
	if(_data)
		gGraphics->stats.frame.bufferWrite += _newSize;
#endif
		glNamedBufferDataEXT(m_handle, _newSize, _data, (uint16)m_usage);
		m_capacity = _newSize;
	}
	else if (_data)
	{
		Write(_data, 0, _newSize);
	}
	m_size = _newSize;
}
//----------------------------------------------------------------------------//
uint8* Buffer::Map(LockMode _mode, uint _offset, int _size)
{
	GL_DEBUG();
	ASSERT(_offset <= m_size);
	if (_size < 0)
		_size = m_size - _offset;
	ASSERT((uint)(_offset + _size) <= m_size);

#ifdef _STATISTICS
	switch (_mode)
	{
	case LockMode::ReadOnly:
		gGraphics->stats.frame.bufferRead += _size;
		break;
	case LockMode::WriteDiscard:
		gGraphics->stats.frame.bufferWrite += _size;
		break;
	case LockMode::WriteNoOverwrite:
		gGraphics->stats.frame.bufferRead += _size;
		gGraphics->stats.frame.bufferWrite += _size;
		break;
	}
#endif

	return (uint8*)glMapNamedBufferRangeEXT(m_handle, _offset, _size, (uint16)_mode);
}
//----------------------------------------------------------------------------//
void Buffer::Unmap(void)
{
	GL_DEBUG();
	glUnmapNamedBufferEXT(m_handle);
}
//----------------------------------------------------------------------------//
void Buffer::Write(const void* _src, uint _offset, uint _size)
{
	GL_DEBUG();
	ASSERT(_src || !_size);
	ASSERT((uint)(_offset + _size) <= m_size);

	//memcpy(Map(LockMode::WriteDiscard, _offset, _size), _src, _size);
	//Unmap();
	glNamedBufferSubDataEXT(m_handle, _offset, _size, _src);
#ifdef _STATISTICS
	gGraphics->stats.frame.bufferWrite += _size;
#endif
}
//----------------------------------------------------------------------------//
void Buffer::Copy(Buffer* _src, uint _srcOffset, uint _dstOffset, uint _size)
{
	GL_DEBUG();
	ASSERT(_src != nullptr);
	ASSERT(_dstOffset + _size <= m_size);
	ASSERT(_srcOffset + _size <= _src->m_size);

	glNamedCopyBufferSubDataEXT(_src->m_handle, m_handle, _srcOffset, _dstOffset, _size);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Vertex
//----------------------------------------------------------------------------//

const Vertex::Attrib Vertex::Format[] =
{
	{ GL_FLOAT, 3, false, offsetof(Vertex, position) },
	{ GL_HALF_FLOAT, 2, false, offsetof(Vertex, texcoord) },
	{ GL_UNSIGNED_BYTE, 4, true, offsetof(Vertex, color) },
	{ GL_BYTE, 4, true, offsetof(Vertex, normal) },
	{ GL_UNSIGNED_BYTE, 4, true, offsetof(Vertex, tangent) },
	{ GL_UNSIGNED_BYTE, 4, true, offsetof(Vertex, weights) },
	{ GL_UNSIGNED_BYTE, 4, false, offsetof(Vertex, indices) },
	{ GL_HALF_FLOAT, 2, false, offsetof(Vertex, texcoord2) },
	{ GL_FLOAT, 2, false, offsetof(Vertex, size) },
	{ GL_FLOAT, 1, false, offsetof(Vertex, rotation) },
};

//----------------------------------------------------------------------------//
// PixelFormat
//----------------------------------------------------------------------------//

const PixelFormat::GLFormat PixelFormat::GLFormats[] =
{
	{ 24, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE }, // RGB8
	{ 32, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE }, // RGBA8
	{ 32, GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV }, // RGB10A2
	{ 32, GL_R32F, GL_RED, GL_FLOAT }, // R32F
	{ 64, GL_RG32F, GL_RG, GL_FLOAT }, // RG32F
	{ 96, GL_RGB32F, GL_RGB, GL_FLOAT }, // RGB32F
	{ 128, GL_RGBA32F, GL_RGBA, GL_FLOAT }, // RGBA32F
	{ 64, GL_RGBA16F, GL_RGBA, GL_FLOAT }, // RGBA16F
	{ 32, GL_R11F_G11F_B10F, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV }, // RG11B10F
	{ 32, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8 }, // D24S8
	{ 4, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 0, 0 }, // DXT1
	{ 8, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 0, 0 }, // DXT5
};

//----------------------------------------------------------------------------//
// Texture
//----------------------------------------------------------------------------//

const uint16 TextureType::GLTypes[] =
{
	GL_TEXTURE_2D, // Quad
	GL_TEXTURE_3D, // Volume
	GL_TEXTURE_2D_ARRAY, // Array
	GL_TEXTURE_CUBE_MAP, // Cube
};

const uint8 Texture::MinDepth[] =
{
	1, // Quad
	1, // Volume
	1, // Array
	6, // Cube
};

const uint16 Texture::MaxDepth[] =
{
	1, // Quad
	4096, // Volume
	4096, // Array
	6, // Cube
};

//----------------------------------------------------------------------------//
Texture::Texture(void)
{
	_ResetParams();
}
//----------------------------------------------------------------------------//
Texture::~Texture(void)
{
	_DeleteHandle();
}
//----------------------------------------------------------------------------//
void Texture::Init(TextureType::Enum _type, PixelFormat::Enum _format, int _lods)
{
	if (m_type == _type && m_format == _format && m_desiredLods == _lods) // no changes
		return;

	m_type = _type;
	m_format = _format;
	m_desiredLods = _lods;
	_ResetParams();
	_CreateHandle();
}
//----------------------------------------------------------------------------//
void Texture::Realloc(uint _width, uint _height, uint _depth)
{
	GL_DEBUG();

	if (!_depth)
		_depth = m_depth;
	_depth = Clamp<uint>(_depth, MinDepth[m_type], MaxDepth[m_type]);

	if (m_width == _width && m_height == _height && m_depth == _depth) // no changes
		return;


	if (!m_initialized)
		_CreateHandle();
	m_width = _width;
	m_height = _height;
	m_depth = _depth;

	uint _side = Max(m_width, m_height);
	uint _maxLods = _side ? Log2i(_side) + 1 : 1;
	m_lods = (m_desiredLods < 0) ? _maxLods : Clamp<uint>(m_desiredLods, 1, _maxLods);
	uint _iformat = PixelFormat::GLFormats[m_format].iformat;
	uint _target = TextureType::GLTypes[m_type];

	if (m_type == TextureType::Quad || m_type == TextureType::Cube)
		glTextureStorage2DEXT(m_handle, _target, m_lods, _iformat, m_width, m_height);
	else
		glTextureStorage3DEXT(m_handle, _target, m_lods, _iformat, m_width, m_height, m_depth);
}
//----------------------------------------------------------------------------//
void Texture::Write(PixelFormat::Enum _format, uint _x, uint _y, uint _z, uint _width, uint _height, uint _depth, const void* _data)
{
	GL_DEBUG();
	ASSERT(m_initialized == true);
	ASSERT(_data != nullptr);
	ASSERT(_x + _width <= m_width);
	ASSERT(_y + _height <= m_height);
	ASSERT(_z + _depth <= m_depth);

	const PixelFormat::GLFormat& _pf = PixelFormat::GLFormats[_format];
	bool _compressed = PixelFormat::IsCompressed(_format);
	uint _bpl = (_width * _height * _pf.bpp) >> 3;
	uint _bpi = _bpl * _depth;
	uint _target = TextureType::GLTypes[m_type];

	if (m_type == TextureType::Quad)
	{
		if (_compressed)
			glCompressedTextureSubImage2DEXT(m_handle, _target, 0, _x, _y, _width, _height, _pf.iformat, _bpi, _data);
		else
			glTextureSubImage2DEXT(m_handle, _target, 0, _x, _y, _width, _height, _pf.format, _pf.type, _data);
	}
	else if (m_type == TextureType::Volume || m_type == TextureType::Array)
	{
		if (_compressed)
			glCompressedTextureSubImage3DEXT(m_handle, _target, 0, _x, _y, _z, _width, _height, _depth, _pf.iformat, _bpi, _data);
		else
			glTextureSubImage3DEXT(m_handle, _target, 0, _x, _y, _z, _width, _height, _depth, _pf.format, _pf.type, _data);
	}
	else for (uint i = 0; i < 6; ++i) // Cube
	{
		_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		const void* _img = ((const uint8*)_data) + _bpl * i;
		if (_compressed)
			glCompressedTextureSubImage2DEXT(m_handle, _target, 0, _x, _y, _width, _height, _pf.iformat, _bpl, _img);
		else
			glTextureSubImage2DEXT(m_handle, _target, 0, _x, _y, _width, _height, _pf.format, _pf.type, _img);
	}
}
//----------------------------------------------------------------------------//
void Texture::GenerateMipmap(void)
{
	GL_DEBUG();

	if (m_lods && m_handle)
		glGenerateTextureMipmapEXT(m_handle, TextureType::GLTypes[m_type]);
}
//----------------------------------------------------------------------------//
void Texture::_CreateHandle()
{
	GL_DEBUG();

	_DeleteHandle();
	glGenTextures(1, &m_handle);
	_Bind(MAX_TEXTURE_UNITS + 1, this);
	m_initialized = true;
}
//----------------------------------------------------------------------------//
void Texture::_DeleteHandle()
{
	GL_DEBUG();

	if (m_handle)
		glDeleteTextures(1, &m_handle);
}
//----------------------------------------------------------------------------//
void Texture::_ResetParams(void)
{
	m_lods = 0;
	m_width = 0;
	m_height = 0;
	m_depth = MinDepth[m_type];
	m_initialized = false;
}
//----------------------------------------------------------------------------//
void Texture::_Bind(uint _slot, Texture* _texture)
{
	GL_DEBUG();

	uint _target, _handle;
	if (_texture)
		_target = TextureType::GLTypes[_texture->m_type], _handle = _texture->m_handle;
	else
		_target = GL_TEXTURE_2D, _handle = 0;
	glBindMultiTextureEXT(GL_TEXTURE0 + _slot, _target, _handle);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Sampler
//----------------------------------------------------------------------------//

const uint16 TextureFilter::GLMinFilter[] =
{
	GL_NEAREST, // Nearest
	GL_LINEAR, // Linear
	GL_LINEAR_MIPMAP_LINEAR, // Trilinear
};

const uint16 TextureFilter::GLMagFilter[] =
{
	GL_NEAREST, // Nearest
	GL_LINEAR, // Linear
	GL_LINEAR, // Trilinear
};

const Sampler::Desc Sampler::Variants[__Count] =
{
	{ TextureWrap::Clamp, TextureFilter::Nearest, 1, CompareFunc::Never },	// NearestClamp
	{ TextureWrap::Clamp, TextureFilter::Linear, 16, CompareFunc::Never },	// LinearClamp
	{ TextureWrap::Repeat, TextureFilter::Linear, 16, CompareFunc::Never },	// LinearRepeat
	{ TextureWrap::Clamp, TextureFilter::Trilinear, 16, CompareFunc::Never },	// TrilinearClamp
	{ TextureWrap::Repeat, TextureFilter::Trilinear, 16, CompareFunc::Never },	// TrilinearRepeat
	{ TextureWrap::Clamp, TextureFilter::Nearest, 1, CompareFunc::LessEqual },	// LessEqual
};

//----------------------------------------------------------------------------//
void Sampler::_Init(const Desc& _desc)
{
	GL_DEBUG();
	//ASSERT(m_handle == 0);

	glGenSamplers(1, &m_handle);
	glBindSampler(MAX_TEXTURE_UNITS, m_handle);
	glSamplerParameteri(m_handle, GL_TEXTURE_MIN_FILTER, TextureFilter::GLMinFilter[_desc.filter]);
	glSamplerParameteri(m_handle, GL_TEXTURE_MAG_FILTER, TextureFilter::GLMagFilter[_desc.filter]);
	glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_S, (int)_desc.wrap);
	glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_T, (int)_desc.wrap);
	//glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glSamplerParameteri(m_handle, GL_TEXTURE_MAX_ANISOTROPY_EXT, _desc.anisotropy);
	if (_desc.depthFunc != CompareFunc::Never)
	{
		glSamplerParameteri(m_handle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(m_handle, GL_TEXTURE_COMPARE_FUNC, _desc.depthFunc);
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// RenderBuffer
//----------------------------------------------------------------------------//

uint RenderBuffer::s_copyFBO[2];

//----------------------------------------------------------------------------//
RenderBuffer::RenderBuffer(void)
{
	GL_DEBUG();

	glGenRenderbuffers(1, &m_handle);
	glBindRenderbuffer(GL_RENDERBUFFER, m_handle);
}
//----------------------------------------------------------------------------//
RenderBuffer::~RenderBuffer(void)
{
	//glDeleteRenderbuffers(1, &m_handle);
}
//----------------------------------------------------------------------------//
void RenderBuffer::Init(PixelFormat::Enum _format, uint _samples)
{
	_samples = Clamp<uint>(_samples, 1, 16);
	if (m_format == _format && m_samples == _samples) // no changes
		return;

	m_samples = _samples;
	m_format = _format;
	m_initialized = false;
	Realloc(m_width, m_height);
}
//----------------------------------------------------------------------------//
void RenderBuffer::Realloc(uint _width, uint _height)
{
	GL_DEBUG();

	if (m_initialized && m_width == _width && m_height == _height)
		return;

	m_initialized = true;
	m_width = _width;
	m_height = _height;
	glNamedRenderbufferStorageMultisampleEXT(m_handle, m_samples, PixelFormat::GLFormats[m_format].iformat, m_width, m_height);
}
//----------------------------------------------------------------------------//
void RenderBuffer::CopyToTexture(Texture* _texture, uint _z)
{
	GL_DEBUG();
	ASSERT(_texture != nullptr);
	ASSERT(_z < _texture->Depth());
	ASSERT(m_format != PixelFormat::D24S8 || _texture->Format() == PixelFormat::D24S8);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_copyFBO[0]); // dst
	glBindFramebuffer(GL_READ_FRAMEBUFFER, s_copyFBO[1]); // src

	uint _buffers;
	if (m_format == PixelFormat::D24S8)
	{
		_buffers = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		glNamedFramebufferRenderbufferEXT(s_copyFBO[1], GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_handle);
		//glNamedFramebufferRenderbufferEXT(s_copyFBO[1], GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_handle);
	}
	else
	{
		_buffers = GL_COLOR_BUFFER_BIT;
		glNamedFramebufferRenderbufferEXT(s_copyFBO[1], GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_handle);
	}

	if (_texture->Format() == PixelFormat::D24S8)
	{
		_BindRenderTargetTexture(s_copyFBO[0], GL_DEPTH_STENCIL_ATTACHMENT, _texture, _z);
		//_BindRenderTargetTexture(s_copyFBO[0], GL_STENCIL_ATTACHMENT, _texture, _z);
	}
	else
	{
		_BindRenderTargetTexture(s_copyFBO[0], GL_COLOR_ATTACHMENT0, _texture, _z);
	}

	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, _texture->Width(), _texture->Height(), _buffers, GL_NEAREST);
	_texture->GenerateMipmap();

	//glBindFramebuffer(GL_FRAMEBUFFER, gGraphics->IsRenderTargetsEnabled() ? gGraphics->_GetFramebuffer() : 0);
}
//----------------------------------------------------------------------------//
void RenderBuffer::_BindRenderTargetTexture(uint _framebuffer, uint _attachment, Texture* _texture, uint _z)
{
	GL_DEBUG();

	if (_texture)
	{
		uint _target = _texture->TexType() == TextureType::Cube ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + _z : TextureType::GLTypes[_texture->TexType()];
		if (_texture->TexType() == TextureType::Volume)
			glNamedFramebufferTexture3DEXT(_framebuffer, _attachment, _target, _texture->_Handle(), 0, _z);
		else if (_texture->TexType() == TextureType::Array)
			glNamedFramebufferTextureLayerEXT(_framebuffer, _attachment, _texture->_Handle(), 0, _z);
		else
			glNamedFramebufferTexture2DEXT(_framebuffer, _attachment, _target, _texture->_Handle(), 0);
	}
	else
		glNamedFramebufferTextureEXT(_framebuffer, _attachment, 0, 0);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Shader
//----------------------------------------------------------------------------//

const char* Shader::Defines[] =
{
	"INSTANCED", // Instanced
	"SKINNED", // Skinned
	"SPRITE", // Sprite
	"BILLBOARD", // Billboard
	"BILLBOARD_Y", // BillboardY
	"PARTICLE", // Particle
	"FSQUAD", // FSQuad
	"TEXTURE", // Texture
};

const Shader::Desc Shader::Variants[__Count] =
{
	{ Vertex, GLSL_GenericVS, Instanced }, // VS_StaticModel
	{ Vertex, GLSL_GenericVS, Skinned }, // VS_SkinnedModel
	{ Vertex, GLSL_GenericVS, Sprite }, // VS_Sprites
	{ Vertex, GLSL_GenericVS, Particle }, // VS_Particles
	{ Vertex, GLSL_GenericVS, FSQuad }, // VS_FSQuad

	{ Geometry, GLSL_QuadGS, Sprite }, // GS_Sprite
	{ Geometry, GLSL_QuadGS, Billboard }, // GS_Billboard
	{ Geometry, GLSL_QuadGS, BillboardY }, // GS_BillboardY
	{ Geometry, GLSL_QuadGS, Particle }, // GS_Particles

	{ Pixel, GLSL_GenericPS, 0 }, // PS_NoTexture
	{ Pixel, GLSL_GenericPS, Texture }, // PS_Texture
};

const uint16 Shader::GLTypes[__NumTypes] =
{
	GL_VERTEX_SHADER, // ST_Vertex
	GL_FRAGMENT_SHADER, // ST_Pixel
	GL_GEOMETRY_SHADER, // ST_Geometry
};

const char* Shader::Prefix[__NumTypes] =
{
	"VS", // ST_Vertex
	"PS", // ST_Pixel
	"GS", // ST_Geometry
};

//----------------------------------------------------------------------------//
void Shader:: _Init(const Desc& _desc)
{
	GL_DEBUG();
	//ASSERT(m_handle == 0);

	const Source& _src = Sources[_desc.source];
	// log
#ifdef _LOG
	{
		String _dbgString;
		for (uint i = 0; i < 31; ++i)
		{
			if ((_desc.flags & (1 << i)) != 0)
			{
				if (_dbgString.Length())
					_dbgString += " | ";
				_dbgString += Defines[i];
			}
		}
		LOG("Compile '%s' %s", _src.debugName, _dbgString.CStr());
	}
#endif

	String _srcs;
	// header
	_srcs += "#version 330 core\n"
		//"#extension GL_ARB_enhanced_layouts:enable\n"
		"#extension GL_ARB_shading_language_420pack:enable\n"
		"#extension GL_ARB_separate_shader_objects:enable\n"
		"#define GLSL\n"
		"#define COMPILE_";
	_srcs += Prefix[_desc.type];

	//defines
	for (uint i = 0; i < 31; ++i)
	{
		if ((_desc.flags & (1 << i)) != 0)
			_srcs.Append("\n#define ").Append(Defines[i]);
	}

	// common
	_srcs += "\n#line 1 0\n";
	uint _length;
	const char* _asset = (const char*)gAssets->GetAsset(Sources[GLSL_Common].name, _length);
	ASSERT(_asset != nullptr, "Shader 'Common.glsl' not found");
	_srcs.Append(_asset, _length);

	// code
	_srcs += "\n#line 1 1\n";
	_asset = (const char*)gAssets->GetAsset(_src.name, _length);
	ASSERT(_asset != nullptr, "Shader not found");
	_srcs.Append(_asset, _length);
	_srcs += '\n';

	// compile
	const char* _srcp = _srcs;
	m_handle = glCreateShaderProgramv(GLTypes[_desc.type], 1, &_srcp);
	m_type = _desc.type;

	// log
#ifdef _LOG
	{
		int _length;
		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &_length);
		if (_length > 1)
		{
			String _log;
			_log.Resize(_length);
			glGetProgramInfoLog(m_handle, _length, &_length, _log.Data());
			LOG("%s", _log.CStr());
		}
	}
#endif

	// status
	int _status = 0;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &_status);
	CHECK(_status != 0, "Couldn't compile shader");
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// ShaderParam
//----------------------------------------------------------------------------//

const ShaderParam::Desc ShaderParam::Variants[__Count] =
{
	{ 0, 0 }, // Unused
	{ 1, sizeof(UCamera) }, // Camera
	{ MAX_INSTANCES, sizeof(Matrix44) }, // InstanceMatrices
	{ MAX_INSTANCES, sizeof(Matrix24) }, // InstanceParams
	{ MAX_BONES, sizeof(Matrix44) }, // BoneMatrices
	{ 1, sizeof(URasterizerParams) }, // RasterizerParams
};

ShaderParam* ShaderParam::s_changedParams = nullptr;

//----------------------------------------------------------------------------//
void ShaderParam::_Init(uint _elementCount, uint _elementSize)
{
	ASSERT(m_buffer == nullptr);
	ASSERT(m_data == nullptr);

	uint _size = _elementSize * _elementCount;
	m_data = new uint8[_size];
	m_buffer = new Buffer(BufferUsage::Dynamic);
	m_buffer->Realloc(_size);

	m_size = _elementCount;
	m_elementSize = _elementSize;
}
//----------------------------------------------------------------------------//
/*void ShaderParam::_Destroy(void)
{
	if (m_changed)
		Link(s_changedParams, this, m_prev);
	delete[] m_data;
	delete m_buffer;
}*/
//----------------------------------------------------------------------------//
void ShaderParam::_Invalidate(uint _numElements)
{
	ASSERT(_numElements <= m_size);

	uint _size = _numElements *m_elementSize;
	if (m_changed < _size)
	{
		if (!m_changed)
			Link(s_changedParams, this, m_prev);
		m_changed = _size;
	}
}
//----------------------------------------------------------------------------//
void ShaderParam::_Flush(void)
{
	ASSERT(m_changed > 0);

#ifdef _STATISTICS
	gGraphics->stats.frame.spUploaded += m_changed;
#endif

	//memcpy(m_buffer->Map(LockMode::WriteDiscardUnsynchronized, 0, m_changed), m_data, m_changed);
	//m_buffer->Unmap();
	m_buffer->Write(m_data, 0, m_changed);
	Unlink(s_changedParams, this, m_prev);
	m_changed = 0;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Graphics
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Graphics::Graphics(void)
{
	LOG_NODE("Create Graphics");

	ASSERT(gDevice != nullptr, "The window must be created before it");

	// create context
	{
		PIXELFORMATDESCRIPTOR _pfd;
		memset(&_pfd, 0, sizeof(_pfd));
		_pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		_pfd.nVersion = 1;
		_pfd.nSize = sizeof(_pfd);
		_pfd.cColorBits = 32;
		_pfd.cDepthBits = 24;
		_pfd.cStencilBits = 8;

		m_dc = GetDC(gDevice->_WindowHandle());
		int _pf = ChoosePixelFormat(m_dc, &_pfd);
		SetPixelFormat(m_dc, _pf, &_pfd);

		m_rc = wglCreateContext(m_dc); // legacy context

#	if defined(_GL_ARB_CONTEXT)  || defined(_GL_CORE_PROFILE_CONTEXT)  || defined(_DEBUG_GRAPHICS) 

		wglMakeCurrent(m_dc, m_rc);

		PFNWGLCREATECONTEXTATTRIBSARBPROC _wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		CHECK(_wglCreateContextAttribsARB != nullptr, "OpenGL 3.x is not supported");

#		ifdef _GL_CORE_PROFILE_CONTEXT 
		int _flags = WGL_CONTEXT_CORE_PROFILE_BIT_ARB | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
#		else
		int _flags = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
#		endif

#		ifdef _DEBUG_GRAPHICS
		_flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
#		endif

		int _attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB, _flags,
			0, 0
		};

		wglDeleteContext(m_rc);
		m_rc = _wglCreateContextAttribsARB(m_dc, 0, _attribs);
#	endif

		CHECK(m_rc != nullptr, "Couldn't create OpenGL render context");
		wglMakeCurrent(m_dc, m_rc);
	}

	// log info
#	ifdef _LOG
	{
		const char* _context =
#	if defined(_GL_ARB_CONTEXT)  || defined(_GL_CORE_PROFILE_CONTEXT)  || defined(_DEBUG_GRAPHICS) 
			" ("
#		ifdef _GL_CORE_PROFILE_CONTEXT
			"core profile"
#		else
			"compatible profile"
#		endif
#		ifdef _DEBUG_GRAPHICS
			" debug"
#		endif
			" context)"
#	else
			""
#	endif
			;
		LOG("OpenGL %s%s", glGetString(GL_VERSION),	_context);
		LOG("%s, %s", glGetString(GL_RENDERER), glGetString(GL_VENDOR));
	}
#	endif //_LOG

	// load functions/extensions
	{
		LoadOpenGL();

		int _p[4];
		glGetIntegerv(GL_MAJOR_VERSION, _p);
		glGetIntegerv(GL_MINOR_VERSION, _p + 1);
		int _ver = _p[0] * 10 + _p[1];

		CHECK(HasOpenGLExtension("GL_ARB_separate_shader_objects") || _ver >= 41, "No GL_ARB_separate_shader_objects extension");
		CHECK(HasOpenGLExtension("GL_ARB_vertex_attrib_binding") || _ver >= 43, "No GL_ARB_vertex_attrib_binding extension");
		//CHECK(HasOpenGLExtension("GL_ARB_enhanced_layouts") || _ver >= 44, "No GL_ARB_enhanced_layouts extension"); // for layout(offset=n, align=n)
		CHECK(HasOpenGLExtension("GL_ARB_shading_language_420pack") || _ver >= 42, "No GL_ARB_shading_language_420pack extension"); // for layout(binding=n)
		CHECK(HasOpenGLExtension("GL_ARB_texture_storage") || _ver >= 42, "No GL_ARB_texture_storage extension");
		//CHECK(HasOpenGLExtension("GL_ARB_texture_storage_multisample") || _ver >= 43, "No GL_ARB_texture_storage_multisample extension");
		CHECK(HasOpenGLExtension("GL_EXT_direct_state_access"), "No GL_EXT_direct_state_access extension");
	}

#ifdef _DEBUG_GRAPHICS
	GLDebug::_Init();
#endif

#ifdef _STATISTICS
	stats.Init();
#endif

	// depth/stencil
	m_currentStencilRef = 0;

	// geometry
	{
		GL_DEBUG("init geometry");

		glGenVertexArrays(1, &m_vertexArray);
	}

	// textures
	{
		memset(m_currentTextures, 0, sizeof(m_currentTextures));
		gResources->RegisterType<Texture>();
	}

	// samplers
	{
		memset(m_currentSamplers, 0, sizeof(m_currentSamplers));

		for (uint i = 0; i < Sampler::__Count; ++i)
			m_samplers[i]._Init(Sampler::Variants[i]);
	}

	// framebuffer
	{
		GL_DEBUG("init framebuffers");

		m_drawToFramebuffer = false;

		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		glGenFramebuffers(2, RenderBuffer::s_copyFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, RenderBuffer::s_copyFBO[0]);
		glBindFramebuffer(GL_FRAMEBUFFER, RenderBuffer::s_copyFBO[1]);

		memset(m_renderTargets, 0, sizeof(m_renderTargets));
		memset(&m_depthStencilTarget, 0, sizeof(m_depthStencilTarget));
	}

	// shaders
	{
		GL_DEBUG("init shaders");

		glGenProgramPipelines(1, &m_shaderPipeline);

		for (uint i = 0; i < Shader::__Count; ++i)
			m_shaders[i]._Init(Shader::Variants[i]);
	}

	// uniforms
	{
		GL_DEBUG("init uniforms");

		for (uint i = 0; i < ShaderParam::__Count; ++i)
		{
			const ShaderParam::Desc& _desc = ShaderParam::Variants[i];
			m_shaderParams[i]._Init(_desc.size, _desc.elementSize);
			glBindBufferBase(GL_UNIFORM_BUFFER, i, m_shaderParams[i].m_buffer->m_handle);
		}

		const_cast<UCamera*>(Camera) = reinterpret_cast<UCamera*>(m_shaderParams[ShaderParam::Camera].m_data);
		const_cast<Matrix44*>(InstanceMatrices) = reinterpret_cast<Matrix44*>(m_shaderParams[ShaderParam::InstanceMatrices].m_data);
		const_cast<Matrix24*>(InstanceParams) = reinterpret_cast<Matrix24*>(m_shaderParams[ShaderParam::InstanceParams].m_data);
		const_cast<Matrix44*>(BoneMatrices) = reinterpret_cast<Matrix44*>(m_shaderParams[ShaderParam::BoneMatrices].m_data);
		const_cast<URasterizerParams*>(RasterizerParams) = reinterpret_cast<URasterizerParams*>(m_shaderParams[ShaderParam::RasterizerParams].m_data);
	}


	_ResetState();
}
//----------------------------------------------------------------------------//
Graphics::~Graphics(void)
{
	LOG_NODE("Destroy Graphics");

	/*
	for (uint i = 0; i < Shader::__Count; ++i)
		glDeleteSamplers(m_samplers[i].m_handle)

	glDeleteFramebuffers(1, &m_framebuffer);
	glDeleteFramebuffers(2, RenderBuffer::s_copyFBO);

	for (uint i = 0; i < Shader::__Count; ++i)
		glDeleteShader(m_shaders[i].m_handle)

	for (uint i = 0; i < ShaderParam::__Count; ++i)
		m_uniforms[i]._Destroy();
	*/

	wglDeleteContext(m_rc);
	DeleteDC(m_dc);
}
//----------------------------------------------------------------------------//
EventResult Graphics::_OnEvent(int _type, void* _data)
{
	switch (_type)
	{
	case ET_BeginFrame:
	{
		_ResetState();
#ifdef _STATISTICS
		stats._BeginFrame();
#endif

	} break;

	case ET_EndFrame:
	{
		wglSwapIntervalEXT(0); // TODO: VSync

#	if defined(_GL_CORE_PROFILE_CONTEXT) && defined(_DEBUG_GRAPHICS)
		bool _dbgEnabled = GLDebug::Enabled;
		GLDebug::Enabled = false; // disable debug output in core profile
#	endif

		SwapBuffers(m_dc);

#	if defined(_GL_CORE_PROFILE_CONTEXT) && defined(_DEBUG_GRAPHICS)
		GLDebug::Enabled = _dbgEnabled;
#	endif

#ifdef _STATISTICS
		stats._EndFrame();
#endif
	} break;
	}

	return ER_Pass;
}
//----------------------------------------------------------------------------//
void Graphics::_ResetState(void)
{
	GL_DEBUG();

	// depth/stencil
	m_currentDepthState = nullptr;
	SetDepthState(DepthState::Default);

	// geometry
	glBindVertexArray(m_vertexArray);
	m_currentVertexBuffer = nullptr;
	m_currentIndexBuffer = nullptr;

	// vertex format
	for (uint i = 0; i < sizeof(Vertex::Format) / sizeof(Vertex::Attrib); ++i)
	{
		const Vertex::Attrib& _attrib = Vertex::Format[i];
		glVertexAttribBinding(i, 0);
		glEnableVertexAttribArray(i);
		glVertexAttribFormat(i, _attrib.components, _attrib.format, _attrib.normalized, _attrib.offset);
	}

	// textures/samplers
	ResetTextureUnits();

	// framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_drawToFramebuffer = false;
	ResetRenderTargets();

	// shaders
	memset(m_currentShader, 0, sizeof(m_currentShader));
	glBindProgramPipeline(m_shaderPipeline);
	glUseProgramStages(m_shaderPipeline, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT | GL_GEOMETRY_SHADER_BIT, 0);
}
//----------------------------------------------------------------------------//
void Graphics::_FlushUniforms(void)
{
	while (ShaderParam::s_changedParams)
		ShaderParam::s_changedParams->_Flush();
}
//----------------------------------------------------------------------------//
void Graphics::SetDepthState(DepthState::ID _id, uint _stencilRef)
{
	GL_DEBUG();
	ASSERT(_id < DepthState::__Count);

	const DepthState::Desc* _state = &DepthState::Variants[_id];
	if (m_currentDepthState != _state || (_state->stencilTest && m_currentStencilRef != _stencilRef))
	{
		m_currentDepthState = _state;
		m_currentStencilRef = _stencilRef;

		glEnableEx(GL_DEPTH_TEST, _state->depthTest);
		glDepthMask(_state->depthWrite);
		glDepthFunc(_state->depthFunc);

		if (_state->stencilTest)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilMask(_state->stencilMask);
			glStencilFunc(_state->stencilfunc, _stencilRef, _state->stencilMask);
			glStencilOp(_state->stencilFail, _state->depthFail, _state->depthPass);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetVertexBuffer(Buffer* _buffer)
{
	GL_DEBUG();

	if (m_currentVertexBuffer != _buffer)
	{
		m_currentVertexBuffer = _buffer;
		glBindVertexBuffer(0, _buffer ? _buffer->m_handle : 0, 0, sizeof(Vertex));
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetIndexBuffer(Buffer* _buffer)
{
	GL_DEBUG();

	if (m_currentIndexBuffer != _buffer)
	{
		m_currentIndexBuffer = _buffer;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer ? _buffer->m_handle : 0);
	}
}
//----------------------------------------------------------------------------//
void Graphics::ResetTextureUnits(void)
{
	for (uint i = 0; i < MAX_TEXTURE_UNITS; ++i)
	{
		SetTexture(i, nullptr);
		SetSampler(i, Sampler::Default);
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetTexture(uint _slot, Texture* _texture)
{
	ASSERT(_slot < MAX_TEXTURE_UNITS);

	if (m_currentTextures[_slot] != _texture)
	{
		m_currentTextures[_slot] = _texture;
		Texture::_Bind(_slot, _texture);
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetSampler(uint _slot, Sampler::ID _id)
{
	GL_DEBUG();
	ASSERT(_slot < MAX_TEXTURE_UNITS);
	ASSERT(_id < Sampler::__Count);

	Sampler* _sampler = &m_samplers[_id];

	if (m_currentSamplers[_slot] != _sampler)
	{
		m_currentSamplers[_slot] = _sampler;
		glBindSampler(_slot, _sampler->m_handle);
	}
}
//----------------------------------------------------------------------------//
void Graphics::ClearFrameBuffers(uint _buffers, uint _start, uint _num, const Vector4& _color, float _depth, int _stencil)
{
	//TODO: ClearColorBuffers(start, num, color) + 	ClrearDepthStencilBuffers(flags, depth, stencil)
	GL_DEBUG();

	// disable blendig ?

	int _viewport[4], _scissor[4];
	glGetIntegerv(GL_VIEWPORT, _viewport);
	glGetIntegerv(GL_SCISSOR_BOX, _scissor);

	int _scissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
	glEnable(GL_SCISSOR_TEST);
	glScissor(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);

	if (_buffers & FBT_Color)
	{
		int _colorMask[4];
		glGetIntegerv(GL_COLOR_WRITEMASK, _colorMask);
		glColorMask(1, 1, 1, 1);
		for (uint i = _start; _num; ++i, --_num)
			glClearBufferfv(GL_COLOR, i, _color.v);
		glColorMask(_colorMask[0], _colorMask[1], _colorMask[2], _colorMask[3]);
	}

	if (_buffers & FBT_Depth)
	{
		glDepthMask(1);
		glClearBufferfv(GL_DEPTH, 0, &_depth);
		glDepthMask(m_currentDepthState->depthWrite);
	}

	if (_buffers & FBT_Stencil)
	{
		glStencilMask(0xff);
		glClearBufferiv(GL_STENCIL, 0, &_stencil);
		glStencilMask(m_currentDepthState->stencilMask);
	}

	glScissor(_scissor[0], _scissor[1], _scissor[2], _scissor[3]);

	if (!_scissorEnabled)
		glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------//
void Graphics::EnableRenderTargets(bool _enabled)
{
	GL_DEBUG();

	if (m_drawToFramebuffer != _enabled)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _enabled ? m_framebuffer : 0);
		m_drawToFramebuffer = _enabled;
	}
}
//----------------------------------------------------------------------------//
void Graphics::ResetRenderTargets(void)
{
	GL_DEBUG();

	SetDepthBuffer(nullptr);
	SetDepthTexture(nullptr);
	for (uint i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		SetRenderBuffer(i, nullptr);
		SetRenderTexture(i, nullptr);
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetRenderBuffer(uint _slot, RenderBuffer* _target)
{
	GL_DEBUG();
	ASSERT(_slot < MAX_RENDER_TARGETS);

	if (m_renderTargets[_slot].buffer != _target)
	{
		glNamedFramebufferRenderbufferEXT(m_framebuffer, GL_COLOR_ATTACHMENT0 + _slot, GL_RENDERBUFFER, _target ? _target->m_handle : 0);
		m_renderTargets[_slot].buffer = _target;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetDepthBuffer(RenderBuffer* _target)
{
	GL_DEBUG();
	ASSERT(!_target || _target->m_format == PixelFormat::D24S8);

	if (m_depthStencilTarget.buffer != _target)
	{
		uint _handle = _target ? _target->m_handle : 0;
		glNamedFramebufferRenderbufferEXT(m_framebuffer, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _handle);
		//glNamedFramebufferRenderbufferEXT(m_framebuffer, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _handle);
		m_depthStencilTarget.buffer = _target;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetRenderTexture(uint _slot, Texture* _target, uint _z)
{
	GL_DEBUG();
	ASSERT(_slot < MAX_RENDER_TARGETS);

	FramebufferAttachment& _dst = m_renderTargets[_slot];
	if (_dst.texture != _target || _dst.textureZ != _z)
	{
		RenderBuffer::_BindRenderTargetTexture(m_framebuffer, GL_COLOR_ATTACHMENT0 + _slot, _target, _z);
		_dst.texture = _target;
		_dst.textureZ = _z;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetDepthTexture(Texture* _target, uint _z)
{
	GL_DEBUG();
	ASSERT(!_target || _target->m_format == PixelFormat::D24S8);

	if (m_depthStencilTarget.texture != _target || m_depthStencilTarget.textureZ != _z)
	{
		RenderBuffer::_BindRenderTargetTexture(m_framebuffer, GL_DEPTH_STENCIL_ATTACHMENT, _target, _z);
		//RenderBuffer::_BindRenderTargetTexture(m_framebuffer, GL_STENCIL_ATTACHMENT, _target, _z);
		m_depthStencilTarget.texture = _target;
		m_depthStencilTarget.textureZ = _z;
	}
}
//----------------------------------------------------------------------------//
void Graphics::SetNumRenderTargets(uint _num)
{
	GL_DEBUG();
	ASSERT(_num <= MAX_RENDER_TARGETS);

	static const uint _buffers[MAX_RENDER_TARGETS] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
	};

	glFramebufferDrawBuffersEXT(m_framebuffer, _num, _buffers);
}
//----------------------------------------------------------------------------//
void Graphics::SetShader(Shader::ID _id)
{
	GL_DEBUG();

	uint _type, _handle;

	if (_id < Shader::__Count)
	{
		const Shader& _shader = m_shaders[_id];
		_handle = _shader.m_handle;
		_type = _shader.m_type;
	}
	else
	{
		_handle = 0;
		_type = Shader::__NumTypes - (_id - Shader::__Count);
	}

	ASSERT(_type < Shader::__NumTypes);

	if (m_currentShader[_type] != _handle)
	{
		m_currentShader[_type] = _handle;
		glUseProgramStages(m_shaderPipeline, 1 << _type, _handle);
	}
}
//----------------------------------------------------------------------------//
void Graphics::Invalidate(ShaderParam::ID _id, uint _numElements)
{
	ASSERT(_id < ShaderParam::__Count);
	m_shaderParams[_id]._Invalidate(_numElements);
}
//----------------------------------------------------------------------------//
void Graphics::Draw(PrimitiveType::Enum _type, uint _start, uint _count, uint _numInstances)
{
	GL_DEBUG();
	ASSERT(m_currentVertexBuffer != nullptr);

	_FlushUniforms();
	glDrawArraysInstanced(PrimitiveType::GLTypes[_type], _start, _count, _numInstances);

#ifdef _STATISTICS
	stats.frame.drawCalls++;
	stats.frame.drawInstances += _numInstances;
	stats.frame.drawVertices += _count * _type * _numInstances;
	switch (_type)
	{
	case PrimitiveType::Points:
		stats.frame.drawPoints += _count * _numInstances;
		break;
	case PrimitiveType::Lines:
		stats.frame.drawLines += _count * _numInstances;
		break;
	case PrimitiveType::Triangles:
		stats.frame.drawTriangles += _count * _numInstances;
		break;
	}
#endif
}
//----------------------------------------------------------------------------//
void Graphics::DrawIndexed(PrimitiveType::Enum _type, uint _baseVertex, uint _start, uint _count, uint _numInstances)
{
	GL_DEBUG();
	ASSERT(m_currentVertexBuffer != nullptr);
	ASSERT(m_currentIndexBuffer != nullptr);

	_FlushUniforms();
	glDrawElementsInstancedBaseVertex(PrimitiveType::GLTypes[_type], _count, GL_UNSIGNED_SHORT, (const void*)(_start * sizeof(uint16)), _numInstances, _baseVertex);

#ifdef _STATISTICS
	stats.frame.drawCalls++;
	stats.frame.drawInstances += _numInstances;
	stats.frame.drawVertices += _count * _type * _numInstances;
	switch (_type)
	{
	case PrimitiveType::Points:
		stats.frame.drawPoints += _count * _numInstances;
		break;
	case PrimitiveType::Lines:
		stats.frame.drawLines += _count * _numInstances;
		break;
	case PrimitiveType::Triangles:
		stats.frame.drawTriangles += _count * _numInstances;
		break;
	}
#endif
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
