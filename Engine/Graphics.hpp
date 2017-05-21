#pragma once

#include "Core.hpp"
#include "Math.hpp"
#include "System.hpp"
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
// BlendState
//----------------------------------------------------------------------------//

struct BlendFactor
{
	enum Enum : uint16
	{
		One = GL_ZERO, 
		Zero = GL_ONE, 
		SrcColor = GL_SRC_COLOR, 
		OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
		DstColor = GL_DST_COLOR,  
		OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,
		SrcAlpha = GL_SRC_ALPHA, 
		OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA, 
		DstAlpha = GL_DST_ALPHA, 
		OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,
		//GL_SRC_ALPHA_SATURATE,
	};
};

struct BlendMode
{
	enum Enum : uint16
	{
		Add = GL_FUNC_ADD,
		Subtract = GL_FUNC_SUBTRACT,
		ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT,
		Min = GL_MIN,
		Max = GL_MAX,
	};
};

struct BlendState
{
	enum ID
	{
		Default,
		Translucent,

		__Count,
	};

	struct Desc
	{
		bool blendEnabled;
		bool colorWrite;
		BlendFactor::Enum src;
		BlendFactor::Enum dst;
		BlendMode::Enum mode;
	};

	static const Desc Variants[];
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

// TODO:

struct PackedNormal
{
	int8 v[4];
};

struct PackedTexCoord
{
	uint16 v[2];
};

//----------------------------------------------------------------------------//
// Vertex
//----------------------------------------------------------------------------//

//! Generic vertex, 36 bytes 
struct Vertex
{
	Vector3 position; // 0
	float16 texcoord[2]; // 1
	PackedColor color; // 2

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
			float size[3]; // 8 // [0] - width, [1] - height, [2] - angle 
		};
	};

	Vertex& SetRect(const Rect& _r, float _z = 0)
	{
		Vector2 _center = _r.Center();
		Vector2 _size = _r.Size();
		position.Set(_center.x, _center.y, _z);
		size[0] = _size.x;
		size[1] = _size.y;
		return *this;
	}

	Vertex& CopyTexCoord(const Vertex& _v) { texcoord[0] = _v.texcoord[0], texcoord[1] = _v.texcoord[1]; return *this; }
	Vertex& SetTexCoord(float _x, float _y) { texcoord[0] = FloatToHalf(_x), texcoord[1] = FloatToHalf(_y); return *this; }
	Vertex& SetTexRect(const Rect& _r)
	{
		texcoord[0] = FloatToHalf(_r.left);
		texcoord[1] = FloatToHalf(_r.top);
		texcoord2[0] = FloatToHalf(_r.right);
		texcoord2[1] = FloatToHalf(_r.bottom);
		return *this;
	}

	Vertex& SetColor(const Vector4& _color)
	{
		color = _color;
		return *this;
	}
	Vector4 GetColor(void) const { return color; }

	Vertex& SetNormal(const Vector3& _normal)
	{
		normal[0] = FloatToSByte(_normal.x);
		normal[1] = FloatToSByte(_normal.y);
		normal[2] = FloatToSByte(_normal.z);
		normal[3] = 0; // unused (?)
		return *this;
	}

	struct Attrib
	{
		uint16 format;
		uint8 components;
		bool normalized;
		uint16 offset;
		uint8 stream; // 0 - Vertex, 1 - InstanceData,  2 - SkeletonOffset
		uint8 divisor;
	};

	static const Attrib Format[];
};

//! Per-instance data: world matrix and instance parameters
struct InstanceData
{
	Matrix34 matrix = Matrix44::Identity;  // 9, 10, 11
	Vector4 params = Vector4::Zero; // 12 {metallic, smoothness, emission, time}? 
};

//! Per-instance data: first bone index in skeleton buffer
struct SkeletonOffset
{
	int index = 0; // 13
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
	WriteDiscardUnsynchronized = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT,
};

typedef SharedPtr<class Buffer> BufferPtr;

class Buffer : public RefCounted
{
public:
	Buffer(BufferUsage _usage = BufferUsage::Static);
	~Buffer(void);
	BufferUsage Usage(void) { return m_usage; }
	uint Size(void) { return m_size; }
	void Realloc(uint _newSize, const void* _data = nullptr, bool _discard = false);
	uint8* Map(LockMode _mode, uint _offset = 0, int _size = -1);
	void Unmap(void);
	void Write(const void* _src, uint _offset, uint _size);
	void Copy(Buffer* _src, uint _srcOffset, uint _dstOffset, uint _size);


	// it's additional mechanism for streaming draw. Do not use for important data.
	
	uint8* GetStreamData(void) { return m_streamData.Data(); }
	uint GetStreamSize(void) { return m_streamData.Size(); }
	void ClearStream(bool _invalidate = true);
	void InvalidateStream(void);
	uint8* ReallocStreamData(uint _size);
	uint8* AppendStreamData(uint _size);
	void AppendStreamData(const void* _data, uint _size);

	//! discard buffer and initialize him by stream data. Called automatically by Graphics
	void UploadStream(void);

	template<class T> T* Elements(void)
	{
		return reinterpret_cast<T*>(m_streamData.Data());
	}
	template <class T> uint ElementsCount(void)
	{ 
		return m_streamData.Size() / sizeof(T);
	}
	template <class T> T* ReallocElements(uint _size)
	{ 
		return reinterpret_cast<T*>(ReallocStreamData.Data());
	}
	template <class T> T* AppendElements(uint _count)
	{
		return reinterpret_cast<T*>(AppendStreamData(_count * sizeof(T)));
	}
	template <class T> uint AppendElements(const T* _data, uint _count)
	{
		uint _index = ElementsCount<T>();
		AppendStreamData(_data, _count * sizeof(T));
		return _index;
	}
	template <class T> uint AppendElements(const T* _data, uint _count, uint _base)
	{
		uint _index = ElementsCount<T>();
		T* _dst = AppendElements<T>(_count);
		for (uint i = 0; i < _count; ++i)
			_dst[i] = _data[i] + _base;
		return _index;
	}
	template <class T> uint AppendElement(const T& _e)
	{
		return AppendElements<T>(&_e, 1);
	}

protected:
	friend class Graphics;

	BufferUsage m_usage;
	uint m_size = 0;
	uint m_handle = 0;
	bool m_uploaded = true;
	Array<uint8> m_streamData;
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
class Texture : public Object
{
public:
	RTTI("Texture");

	Texture(void);
	~Texture(void);

	TextureType::Enum Type() { return m_type; }
	PixelFormat::Enum Format(void) { return m_format; }
	uint Width(void) { return m_width; }
	uint Height(void) { return m_height; }
	uint Depth(void) { return m_depth; }
	const Vector2& InvSize(void) { return m_invSize; }
	uint _Handle(void) { return m_handle; }

	void Init(TextureType::Enum _type = TextureType::Quad, PixelFormat::Enum _format = PixelFormat::DXT5, int _lods = -1);
	void Realloc(uint _width, uint _height, uint _depth = 0);
	void Write(PixelFormat::Enum _format, const void* _data) { Write(_format, 0, 0, 0, m_width, m_height, m_depth, _data); }
	void Write(PixelFormat::Enum _format, uint _x, uint _y, uint _z, uint _width, uint _height, uint _depth, const void* _data);
	void GenerateMipmap(void);

	Array<Rect>& Atlas(void) { return m_atlas; }
	static const Rect& GetRect(Texture* _texture, uint _index);

	static const uint8 MinDepth[];
	static const uint16 MaxDepth[];

//protected:
	friend class Graphics;

	void _CreateHandle(void);
	void _DeleteHandle(void);
	void _ResetParams(void);
	static void _Bind(uint _slot, Texture* _texture);

	TextureType::Enum m_type = TextureType::Quad;
	PixelFormat::Enum m_format = PixelFormat::RGBA8;
	bool m_initialized = false;
	int m_desiredLods = -1;
	uint m_lods = 0;
	uint m_width = 0;
	uint m_height = 0;
	uint m_depth = 0;
	uint m_handle = 0;
	Vector2 m_invSize = Vector2::Zero;
	Array<Rect> m_atlas;
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

//----------------------------------------------------------------------------//
// Shader
//----------------------------------------------------------------------------//

#define ivec2 IntVector2
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
		VS_Sprite,
		VS_Terrain,

		GS_Sprite,
		GS_Billboard,
		GS_BillboardY,

		PS_Default, // vertex_color*material_color
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
		Skinned = 1 << 0,
		Sprite = 1 << 1,
		Billboard = 1 << 2,
		BillboardY = 1 << 3,
		Terrain = 1 << 4,

		// PS flags
		Albedo = 1 << 5,

		//
		NormalMap,
		DetailAlbedo,
		DetailNormalMap,
		EnvMap,
		CombineTextures2,
		CombineTextures4,
		CombineTextures6,
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

	Buffer* m_buffer = nullptr;
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

TODO_EX("Graphics", "Add RasterizerParams, TextureBuffer, SetSkeletonData, offset in per-instance data");
TODO_EX("Graphics", "Remove DrawIndirect, DrawIndexedIndirect, BaseInstance");

class Graphics : public System, public Singleton<Graphics>
{
public:
	Graphics(void);
	~Graphics(void);

	// state

	void SetDepthState(DepthState::ID _id, uint _stencilRef = 0);
	void SetBlendState(BlendState::ID _id);

	// geometry

	void SetVertexBuffer(Buffer* _buffer);
	void SetIndexBuffer(Buffer* _buffer);

	// instance data

	void SetInstanceData(Buffer* _buffer);
	void SetSkeletonOffset(Buffer* _buffer);
	//void SetSkeletonData(TextureBuffer* _buffer);


	// textures/samplers

	//! Unbind all textures and set samplers to default.
	void ResetTextureUnits(void);
	void SetTexture(uint _slot, Texture* _texture);
	void SetSampler(uint _slot, Sampler::ID _id);

	// framebuffer

	void SetViewport(const IntRect& _viewport);

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
	URasterizerParams* const RasterizerParams = nullptr;

	// draw

	void Draw(PrimitiveType::Enum _type, uint _start, uint _count, uint _numInstances = 1, uint _baseInstance = 0);
	void DrawIndexed(PrimitiveType::Enum _type, uint _baseVertex, uint _start, uint _count, uint _numInstances = 1, uint _baseInstance = 0);

	// 
#ifdef _STATISTICS
	GraphicsStatistics stats;
protected:
	void _AddDrawCall(PrimitiveType::Enum _type, uint _count, uint _numInstances)
	{
		stats.frame.drawCalls++;
		stats.frame.drawInstances += _numInstances;
		stats.frame.drawVertices += _count * _numInstances;
		switch (_type)
		{
		case PrimitiveType::Points:
			stats.frame.drawPoints += _count * _numInstances;
			break;
		case PrimitiveType::Lines:
			stats.frame.drawLines += _count / 2 * _numInstances;
			break;
		case PrimitiveType::Triangles:
			stats.frame.drawTriangles += _count / 3 * _numInstances;
			break;
		}
	}
#endif

protected:
	EventResult _OnEvent(int _type, void* _data) override;

	void _ResetState(void); // reset opengl each frame
	void _BeforeDraw(uint _baseInstance);
	void _SetDefaultInstanceData(void);
	void _SetDefaultSkeletonOffset(void);

	// context

	HDC m_dc;
	HGLRC m_rc;

	// depth/stencil

	const DepthState::Desc* m_currentDepthState;
	uint m_currentStencilRef;

	// blend
	const BlendState::Desc* m_currentBlendState;

	// geometry

	uint m_vertexArray = 0;
	Buffer* m_currentVertexBuffer = nullptr;
	Buffer* m_currentIndexBuffer = nullptr;

	// instance data

	Buffer* m_currentInstanceData = nullptr;
	Buffer* m_currentSkeletonOffset = nullptr;
	uint m_currentInstanceDataBufferBase = (uint)-1;
	uint m_currentSkeletonOffsetBufferBase = (uint)-1;

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
	int m_baseInstanceIdLoc = -1;
	ShaderParam m_shaderParams[ShaderParam::__Count];
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
