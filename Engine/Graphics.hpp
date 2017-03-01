#pragma once

#include "Core.hpp"
#include "Math.hpp"
#include "System.hpp"
#include "Resource.hpp"
#include "OpenGL.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gGraphics Graphics::Get()
					
enum FrameBufferType : uint16
{
	FBT_Color = GL_COLOR_BUFFER_BIT,
	FBT_Depth = GL_DEPTH_BUFFER_BIT,
	FBT_Stencil = GL_STENCIL_BUFFER_BIT,
	FBT_DepthStencil = FBT_Depth | FBT_Stencil,
	FBT_All = FBT_Color | FBT_DepthStencil,
};

enum PolygonFace
{
	PF_Front = GL_FRONT,
	PF_Back = GL_BACK,
	PF_Both = GL_FRONT_AND_BACK,
};

//----------------------------------------------------------------------------//
// CompareFunc
//----------------------------------------------------------------------------//

struct CompareFunc
{
	enum Enum : uint16
	{
		Never = GL_NEVER,
		Less = GL_LESS,
		LessEqual = GL_LEQUAL,
		Equal = GL_EQUAL,
		NotEqual = GL_NOTEQUAL,
		GreaterEqual = GL_GEQUAL,
		Greater = GL_GREATER,
		Always = GL_ALWAYS,
	};
};

//----------------------------------------------------------------------------//
// StencilOp
//----------------------------------------------------------------------------//

struct StencilOp
{
	enum Enum : uint16
	{
		Keep = GL_KEEP,
		Zero = GL_ZERO,
		Replace = GL_REPLACE,
		IncrWrap = GL_INCR_WRAP,
		DecrWrap = GL_DECR_WRAP,
		Incr = GL_INCR,
		Decr = GL_DECR,
		Invert = GL_INVERT,
	};
};

//----------------------------------------------------------------------------//
// PrimitiveType
//----------------------------------------------------------------------------//

struct PrimitiveType
{
	enum Enum : uint16
	{
		Points = 1,
		Lines = 2,
		Triangles = 3,
	};

	static const uint16 GLTypes[];
};

//----------------------------------------------------------------------------//
// DepthState
//----------------------------------------------------------------------------//

struct DepthState
{
	enum ID
	{
		Default, // no test, write
		Disabled, // no test, no write
		Enabled, // test, write
		StencilSet,
		StencilTest,

		__Count,
	};

	struct Desc
	{
		bool depthTest;
		bool depthWrite;
		CompareFunc::Enum depthFunc;

		bool stencilTest;
		uint8 stencilMask; // rw
		CompareFunc::Enum stencilfunc;
		StencilOp::Enum stencilFail;
		StencilOp::Enum depthFail;
		StencilOp::Enum depthPass;
	};

	static const Desc Variants[];
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

struct BlendState
{
	void Bind(uint _mask) const { }
};

//----------------------------------------------------------------------------//
// Buffer
//----------------------------------------------------------------------------//

enum class BufferUsage : uint16
{
	Static = GL_STATIC_DRAW,
	Dynamic = GL_DYNAMIC_DRAW,
	//Stream = GL_STREAM_DRAW,
};

enum class LockMode : uint16
{
	ReadOnly = GL_MAP_READ_BIT,
	WriteDiscard = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT, 
	WriteNoOverwrite = GL_MAP_WRITE_BIT,
	//WriteDiscardUnsynchronized = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT,
};

typedef SharedPtr<class Buffer> BufferPtr;

class Buffer : public RefCounted
{
public:
	Buffer(BufferUsage _usage = BufferUsage::Static);
	~Buffer(void);
	BufferUsage Usage(void) { return m_usage; }
	uint Size(void) { return m_size; }
	uint Capacity(void) { return m_capacity; }
	void Realloc(uint _newSize, const void* _data = nullptr);
	uint8* Map(LockMode _mode, uint _offset = 0, int _size = -1);
	void Unmap(void);
	void Write(const void* _src, uint _offset, uint _size);
	void Copy(Buffer* _src, uint _srcOffset, uint _dstOffset, uint _size);

protected:
	friend class Graphics;

	BufferUsage m_usage;
	uint m_capacity = 0;
	uint m_size = 0;
	uint m_handle = 0;
};

//----------------------------------------------------------------------------//
// Vertex
//----------------------------------------------------------------------------//

//! Generic vertex, 36 bytes 
struct Vertex 
{
	Vector3 position; // 0
	float16 texcoord[2]; // 1
	uint8 color[4]; // 2

	union
	{
		struct
		{
			int8 normal[4]; // 3
			int8 tangent[4]; // 4
			uint8 weights[4]; // 5
			uint8 indices[4]; // 6
		};

		struct
		{
			float16 texcoord2[2]; // 7
			float size[2]; // 8
			float rotation; // 9
		};
	};

	Vertex& SetTexCoord(float _x, float _y) { texcoord[0] = FloatToHalf(_x), texcoord[1] = FloatToHalf(_y); return *this; }

	struct Attrib
	{
		uint16 format;
		uint8 components;
		bool normalized;
		uint offset;
	};

	static const Attrib Format[];
};

//----------------------------------------------------------------------------//
// PixelFormat
//----------------------------------------------------------------------------//

struct PixelFormat
{
	enum Enum : uint8
	{
		RGB8,
		RGBA8,
		RGB10A2,
		R32F,
		RG32F,
		RGB32F,
		RGBA32F,
		RGBA16F,
		RG11B10F,
		D24S8,
		DXT1,
		DXT5,
	};

	static uint BitsPerPixel(PixelFormat::Enum _format) { return GLFormats[_format].bpp; }
	static bool IsCompressed(PixelFormat::Enum _format) { return _format >= DXT1; }

	struct GLFormat
	{
		uint16 bpp;
		uint16 iformat;
		uint16 format;
		uint16 type;
	};

	static const GLFormat GLFormats[];
};

//----------------------------------------------------------------------------//
// Texture
//----------------------------------------------------------------------------//
	 
#define MAX_TEXTURE_UNITS 15

struct TextureType
{
	enum Enum : uint8
	{
		Quad, // 2D
		Volume, // 3D
		Array,
		Cube,
		//Buffer,
	};

	static const uint16 GLTypes[];
};

enum class TextureUsage
{
	Default,
	RenderTarget,
};

typedef SharedPtr<class Texture> TexturePtr;

//!	Texture
class Texture : public Resource
{
public:
	RTTI("Texture");

	Texture(void);
	~Texture(void);

	TextureType::Enum TexType() { return m_type; }
	PixelFormat::Enum Format(void) { return m_format; }
	uint Width(void) { return m_width; }
	uint Height(void) { return m_height; }
	uint Depth(void) { return m_depth; }
	uint _Handle(void) { return m_handle; }

	void Init(TextureType::Enum _type = TextureType::Quad, PixelFormat::Enum _format = PixelFormat::DXT5, int _lods = -1);
	void Realloc(uint _width, uint _height, uint _depth = 0);
	void Write(PixelFormat::Enum _format, const void* _data) { Write(_format, 0, 0, 0, m_width, m_height, m_depth, _data); }
	void Write(PixelFormat::Enum _format, uint _x, uint _y, uint _z, uint _width, uint _height, uint _depth, const void* _data);
	void GenerateMipmap(void);

	static const uint8 MinDepth[];
	static const uint16 MaxDepth[];

protected:
	friend class Graphics;

	void _CreateHandle(void);
	void _DeleteHandle(void);
	void _ResetParams(void);
	static void _Bind(uint _slot, Texture* _texture);

	TextureType::Enum m_type = TextureType::Quad;
	PixelFormat::Enum m_format = PixelFormat::RGBA8;
	bool m_initialized;
	int m_desiredLods = -1;
	uint m_lods;
	uint m_width;
	uint m_height;
	uint m_depth;
	uint m_handle;
};

//----------------------------------------------------------------------------//
// Sampler
//----------------------------------------------------------------------------//

enum class TextureWrap : uint16
{
	Repeat = GL_REPEAT,
	Clamp = GL_CLAMP_TO_EDGE,
};

struct TextureFilter
{
	enum Enum : uint32
	{
		Nearest, // (GL_NEAREST << 16) | GL_NEAREST,
		Linear,	// (GL_LINEAR << 16) | GL_LINEAR,
		Trilinear, // (GL_LINEAR_MIPMAP_LINEAR << 16) | GL_LINEAR,
	};

	static const uint16 GLMinFilter[];
	static const uint16 GLMagFilter[];
};

//!	Texture sampler
struct Sampler : public NonCopyable
{
	//!
	enum ID
	{
		NearestClamp,
		LinearClamp,
		LinearRepeat,
		TrilinearClamp,
		TrilinearRepeat,

		LessEqual, // for depth textures

		__Count,

		Default = LinearRepeat,
	};

	//!
	struct Desc
	{
		TextureWrap wrap;
		TextureFilter::Enum filter;
		uint8 anisotropy;
		CompareFunc::Enum depthFunc;
	};

	static const Desc Variants[];

private:
	friend class Graphics;

	Sampler(void) = default;
	~Sampler(void) = default;

	void _Init(const Desc& _desc);

	uint m_handle;
};

//----------------------------------------------------------------------------//
// RenderBuffer
//----------------------------------------------------------------------------//

#define MAX_RENDER_TARGETS 4

class RenderBuffer : public NonCopyable
{
public:
	RenderBuffer(void);
	~RenderBuffer(void);

	void Init(PixelFormat::Enum _format, uint _samples = 1);
	void Realloc(uint _width, uint _height);
	void CopyToTexture(Texture* _texture, uint _z = 0);

protected:
	friend class Graphics;

	static void _BindRenderTargetTexture(uint _framebuffer, uint _attachment, Texture* _texture, uint _z);

	PixelFormat::Enum m_format = PixelFormat::RGBA8;
	bool m_initialized = false;
	uint m_width = 0;
	uint m_height = 0;
	uint m_samples = 1;
	uint m_handle;
	
	// initialized in Graphics
	static uint s_copyFBO[2];
};

/*class RenderBuffer : public NonCopyable
{
public:

	RenderBuffer(PixelFormat _format);
	~RenderBuffer(void);

	PixelFormat Format(void) { return m_format; }
	uint Width(void) { return m_width; }
	uint Height(void) { return m_height; }
	uint Samples(void) { return m_samples; }
	uint Handle(void) { return m_handle; }

	void Realloc(uint _width, uint _height, uint _samples = 0);
	void CopyToTexture(Texture* _texture, uint _z = 0);	// todo: filter

protected:
	friend class Graphics;

	static void _BindRenderTargetTexture(uint _framebuffer, uint _attachment, Texture* _texture, uint _z);

	PixelFormat m_format;
	uint16 m_iformat;
	uint m_width;
	uint m_height;
	uint m_samples;
	uint m_handle;
};
*/
//----------------------------------------------------------------------------//
// FrameBuffer
//----------------------------------------------------------------------------//

class FrameBuffer
{
public:

	//static void Clear(uint _buffers, uint _start = 0, uint _num = 1, const Vector4& _color = Vector4::Zero, float _depth = 1.f, int _stencil = 0xff);

protected:
};


//----------------------------------------------------------------------------//
// Shader
//----------------------------------------------------------------------------//

#define ivec2 Vector2i
#define vec2 Vector2
#define vec3 Vector3
#define vec4 Vector4
#define mat3 Matrix34
#define mat2x4 Matrix24
#define mat3x4 Matrix34
#define mat4 Matrix44
#define UBUFFER(Id, Name) enum : uint8 {U_##Name = Id}; struct U##Name
#define USAMPLER(Id, T, Name) enum : uint8 {U_##Name = Id};
#define layout(...)

// Shader parameters U* and binding points U_*
#include "../Data/Common.glsl"

//!
struct Shader : public NonCopyable
{
	//!
	enum Type : uint8
	{
		Vertex,
		Pixel,
		Geometry,

		__NumTypes,
	};

	//!
	enum ID
	{
		VS_StaticModel,
		VS_SkinnedModel,
		VS_Sprites,
		VS_Particles,
		VS_FSQuad,

		GS_Sprite,
		GS_Billboard,
		GS_BillboardY,
		GS_Particles,

		PS_NoTexture,
		PS_Texture,

		__Count,

		VS_None = __Count,
		PS_None,
		GS_None,
	};

	//!
	enum Name : uint8
	{
		GLSL_Common,
		GLSL_GenericVS,
		GLSL_QuadGS,
		GLSL_GenericPS,
	};

	//!
	enum Flag : uint
	{
		// VS/GS flags
		Instanced = 1 << 0,
		Skinned = 1 << 1,
		Sprite = 1 << 2,
		Billboard = 1 << 3,
		BillboardY = 1 << 4,
		Particle = 1 << 5,
		FSQuad = 1 << 6,

		// PS flags
		Texture = 1 << 7,
	};

	//!
	struct Source
	{
#ifdef _LOG
#define _DEF_SHADER_SOURCE(Name) { Name, (uint16)String::ConstHash(Name) }
		const char* debugName;
#else
#define _DEF_SHADER_SOURCE(Name) { (uint16)String::ConstHash(Name) }
#endif
		uint16 name;
	};

	//!
	struct Desc
	{
		Type type;
		Name source;
		uint flags;
	};

	static const char* Defines[];
	static const Desc Variants[];
	static const constexpr Source Sources[] =
	{
		_DEF_SHADER_SOURCE("Common.glsl"), // GLSL_Common
		_DEF_SHADER_SOURCE("Generic-VS.glsl"), // GLSL_GenericVS
		_DEF_SHADER_SOURCE("Quad-GS.glsl"), // GLSL_QuadGS
		_DEF_SHADER_SOURCE("Generic-PS.glsl"), // GLSL_GenericPS
	};

	static const uint16 GLTypes[];
	static const char* Prefix[];

private:
	friend class Graphics;

	Shader(void) = default;
	~Shader(void) = default;

	void _Init(const Desc& _desc);

	uint m_handle;
	Type m_type;
};

//----------------------------------------------------------------------------//
// ShaderParam
//----------------------------------------------------------------------------//

struct ShaderParam : public NonCopyable
{
	enum ID
	{
		Unused = 0,

		Camera = U_Camera,
		InstanceMatrices = U_InstanceMat,
		InstanceParams = U_InstanceParams,
		BoneMatrices = U_BoneMat,
		RasterizerParams = U_RasterizerParams,

		__Count
	};

	struct Desc
	{
		uint16 size;
		uint16 elementSize;
	};

	static const Desc Variants[];

private:
	friend class Graphics;

	ShaderParam(void) = default;
	~ShaderParam(void) = default;

	void _Init(uint _elementCount, uint _elementSize);
	//void _Destroy(void);
	void _Invalidate(uint _numElements);
	void _Flush(void);

	Buffer* m_buffer;
	uint8* m_data = nullptr;
	uint m_elementSize = 1;
	uint m_size = 0;
	uint m_changed = 0;

	ShaderParam* m_prev = nullptr;
	ShaderParam* m_next = nullptr;

	static ShaderParam* s_changedParams;
};

//----------------------------------------------------------------------------//
// GraphicsStatistics
//----------------------------------------------------------------------------//

#ifdef _STATISTICS
struct GraphicsStatistics
{
	uint bufferCount; //!< amount of buffers
	uint bufferMemory; //!< total memory size of buffers
	uint bufferWrite;  //!< bytes write to buffers on frame
	uint bufferRead;  //!< bytes read from buffers on frame

	uint textureMemory; //!< total memory size of textures
	uint spUploaded; //!< bytes uploaded to shader parameters on frame
	uint drawCalls;
	uint drawInstances;
	uint drawVertices;
	uint drawPoints;
	uint drawLines;
	uint drawTriangles;

	struct
	{
		uint bufferWrite;
		uint bufferRead;
		uint spUploaded;
		uint drawCalls;
		uint drawInstances;
		uint drawVertices;
		uint drawPoints;
		uint drawLines;
		uint drawTriangles;
	}
	frame; //!< current frame statistics

	void Init(void)
	{
		bufferCount = 0;
		bufferMemory = 0;
		textureMemory = 0;
		_BeginFrame();
		_EndFrame();
	}

	void _BeginFrame(void)
	{
		frame.bufferWrite = 0;
		frame.bufferRead = 0;
		frame.spUploaded = 0;
		frame.drawCalls = 0;
		frame.drawInstances = 0;
		frame.drawVertices = 0;
		frame.drawPoints = 0;
		frame.drawLines = 0;
		frame.drawTriangles = 0;
	}
	void _EndFrame(void)
	{
		bufferWrite = frame.bufferWrite;
		bufferRead = frame.bufferRead;
		spUploaded = frame.spUploaded;
		drawCalls = frame.drawCalls;
		drawInstances = frame.drawInstances;
		drawVertices = frame.drawVertices;
		drawPoints = frame.drawPoints;
		drawLines = frame.drawLines;
		drawTriangles = frame.drawTriangles;
	}
};
#endif

//----------------------------------------------------------------------------//
// Graphics
//----------------------------------------------------------------------------//

class Graphics : public System, public Singleton<Graphics>
{
public:
	Graphics(void);
	~Graphics(void);

	// state

	void SetDepthState(DepthState::ID _id, uint _stencilRef = 0);
	
	// geometry

	void SetVertexBuffer(Buffer* _buffer);
	void SetIndexBuffer(Buffer* _buffer);


	// textures/samplers

	//! Unbind all textures and set samplers to default.
	void ResetTextureUnits(void);
	void SetTexture(uint _slot, Texture* _texture);
	void SetSampler(uint _slot, Sampler::ID _id);

	// framebuffer

	void ClearFrameBuffers(uint _buffers, uint _start = 0, uint _num = 1, const Vector4& _color = Vector4::Zero, float _depth = 1.f, int _stencil = 0xff);

	void EnableRenderTargets(bool _enabled = true);
	bool IsRenderTargetsEnabled(void) { return m_drawToFramebuffer; }
	void ResetRenderTargets(void);
	void SetRenderBuffer(uint _slot, RenderBuffer* _target);
	void SetDepthBuffer(RenderBuffer* _target);
	void SetRenderTexture(uint _slot, Texture* _target, uint _z = 0);
	void SetDepthTexture(Texture* _target, uint _z = 0);
	void SetNumRenderTargets(uint _num);

	// shaders

	void SetShader(Shader::ID _id);

	// uniforms

	void Invalidate(ShaderParam::ID _id, uint _numElements = 1);

	UCamera* const Camera = nullptr;
	Matrix44* const InstanceMatrices = nullptr;
	Matrix24* const InstanceParams = nullptr;
	Matrix44* const BoneMatrices = nullptr;
	URasterizerParams* const RasterizerParams = nullptr;

	// draw

	void Draw(PrimitiveType::Enum _type, uint _start, uint _count, uint _numInstances = 1);
	void DrawIndexed(PrimitiveType::Enum _type, uint _baseVertex, uint _start, uint _count, uint _numInstances = 1);

	// 
#ifdef _STATISTICS
	GraphicsStatistics stats;
#endif

protected:
	EventResult _OnEvent(int _type, void* _data) override;

	void _ResetState(void); // reset opengl each frame
	void _FlushUniforms(void);

	// context

	HDC m_dc;
	HGLRC m_rc;

	// depth/stencil

	const DepthState::Desc* m_currentDepthState;
	uint m_currentStencilRef;

	// geometry

	uint m_vertexArray = 0;
	Buffer* m_currentVertexBuffer = nullptr;
	Buffer* m_currentIndexBuffer = nullptr;

	// textures

	Texture* m_currentTextures[MAX_TEXTURE_UNITS];

	// samplers

	Sampler m_samplers[Sampler::__Count];
	Sampler* m_currentSamplers[MAX_TEXTURE_UNITS];

	// framebuffer

	struct FramebufferAttachment
	{
		Texture* texture;
		RenderBuffer* buffer;
		uint textureZ;
	};

	uint m_framebuffer;
	bool m_drawToFramebuffer;
	FramebufferAttachment m_renderTargets[MAX_RENDER_TARGETS];
	FramebufferAttachment m_depthStencilTarget;

	// shaders

	uint m_shaderPipeline = 0;
	Shader m_shaders[Shader::__Count];
	uint m_currentShader[3]; // handle

	// uniforms
	ShaderParam m_shaderParams[ShaderParam::__Count];
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
