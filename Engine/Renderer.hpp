#pragma once

#include "Graphics.hpp"
#include "Scene.hpp"
#include "Thread.hpp"
#include "Material.hpp"
#include "Image.hpp"
#include "Mesh.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gRenderer Renderer::Get()

typedef SharedPtr<class Camera> CameraPtr;

//----------------------------------------------------------------------------//
// Camera 
//----------------------------------------------------------------------------//

class Camera : public Entity
{
public:
	RTTI("Camera");

	struct Params
	{
		float fov = QUAD_PI; //!< in radians; camera is orthographic when fov equals zero
		float nearClip = .1f;
		float farClip = 1000;
		float zoom = 1;
		bool useRelativeSize = true;
		Rect viewport = Rect::Identity;
		float depthC = 0.001f;

		void Export(UCamera* _cam) const;

		Rect rect;
		Vector3 pos;
		Matrix44 viewMat;
		Matrix44 projMat;
		Matrix44 viewProjMat;
		Matrix44 invViewProjMat;
		Matrix44 normMat;
		Frustum frustum;
		Vector4	depth; // (near, far, C=constant, FC = 1.0/log(far*C + 1))

		void Update(const Matrix44& _worldMat, const Vector2& _windowSize);
	};

	Camera(void);
	~Camera(void);

	void SetPriority(int _priority) { m_priority = _priority; }
	int GetPriority(void) { return m_priority; }

	Rect& Viewport(void) { return m_params.viewport; }
	void SetRelativeSize(bool _use);
	bool IsRelativeSize(void) { return m_params.useRelativeSize; }

	void SetFov(float _fovY);
	float GetFov(void) { return m_params.fov; }
	bool IsOrhto(void) { return m_params.fov <= 0; }

	void SetNear(float _near);
	float GetNear(void) { return m_params.nearClip; }

	void SetFar(float _far);
	float GetFar(void) { return m_params.farClip; }

	void SetZoom(float _zoom);
	float GetZoom(void) { return m_params.zoom; }

	const Params* GetParams(void);

	Ray ScreenToWorld(const Vector2& _cursor = .5f);
	Vector3 WorldToScreen(const Vector3& _pt, bool _clamp01 = true);

protected:
	friend class Renderer;

	void _Register(bool _newState) override;
	void _OnTransformChanged(void) override;
	void _InvalidateParams(void);
	void _UpdateParams(void);

	int m_priority = 0;
	uint m_lastFrame = 0;

	Camera* m_prevCamera = nullptr;
	Camera* m_nextCamera = nullptr;

	Params m_params;
	bool m_paramsUpdated = false;

	static Camera* s_root;
	static Camera* s_active;
};

//----------------------------------------------------------------------------//
// RenderOp
//----------------------------------------------------------------------------//

struct RenderOp
{
	enum  Type
	{
		StaticModel,
		SkinnedModel,
		Sprite,
		Billboard,
		BillboardY,
		Terrain,
	};

	Material::RenderMode GetRenderMode(void)
	{
		return material ? material->GetRenderMode() : Material::RenderMode::Opaque;
	}

	class Renderable* object = nullptr;
	
	Material* material = nullptr;
	Texture* texture = nullptr;	// ignored if material is not null; use for simple color/textured materials

	Type type = StaticModel;

	PrimitiveType::Enum topology = PrimitiveType::Points;
	Buffer* vertexBuffer = nullptr;
	Buffer* indexBuffer = nullptr;
	uint count = 0;
	uint firstIndex = 0;
	uint firstVertex = 0;

	const Matrix44* matrix = nullptr;
	const Vector4* instanceParams = nullptr;
	const Matrix44** bones = nullptr;
	uint numBones = 0;

	float sortKey = 0; // distance or priority
	uint reserved = 0; // for interanl usage

	static bool Cmp(RenderOp* _lhs, RenderOp* _rhs);
	static bool Equals(RenderOp* _lhs, RenderOp* _rhs);
};

//----------------------------------------------------------------------------//
// RenderBatch
//----------------------------------------------------------------------------//

//\note Must be work asynchronously
class RenderBatch : public NonCopyable
{
public:
	struct ViewParams
	{
		Frustum frustum;
		// todo: camera, pass, ...
		uint tag = 0;
		uint layer = 0;
	};

	struct Item
	{
		RenderOp* op = nullptr;

		uint numInstances = 0;
		uint baseInstance = 0; // first index in per-instance data buffers
	};

	RenderBatch(void);

	void AddObject(class Renderable* _object);
	RenderOp* AddRenderOp(void);

	Buffer* TempVertices(void) { return m_tempVertices; }
	Buffer* TempIndices(void) { return m_tempIndices; }

	ViewParams* View(void) { return &m_view; }

	Array<Item>& Items(Material::RenderMode _group) { return m_items[_group]; }
	Buffer* InstanceBuffer(void) { return m_instanceData; }

	void Build(void);
	void Clear(void);

protected:
	BufferPtr m_tempVertices;
	BufferPtr m_tempIndices;
	Array<RenderOp*> m_rops;

	BufferPtr m_instanceData;
	//BufferPtr m_skeletonOffset;
	//BufferPtr m_skeletonData;

	Array<Item> m_items[2]; // 0-opaque, 1-transparent 

	PageAllocator<RenderOp> m_ropStorage;
	Array<Renderable*> m_objects;
	ViewParams m_view;
};

//----------------------------------------------------------------------------//
// Renderable
//----------------------------------------------------------------------------//

struct RenderTag
{
	enum Enum
	{
		//Light = 0x1,
		PointLight = 0x2,
		SpotLight = 0x4,
		DirectionalLight = 0x8,

		Model = 0x10,
		Terrain = 0x20,
		BillboardSet = 0x80,
		Text3D = 0x100,	// --
		Particles = 0x200,
		SkyDome = 0x400,
		RibbonTrail = 0x800,

		Flat = 0x1000, // 2D
		FlatOverlay = 0x2000, // 2DOverlay
	};

	enum Mask
	{
		//Light = PointLight | SpotLight | DirectionalLight,
	};
};

class Renderable abstract : public Entity
{
public:
	RTTI("Renderable"); //!>\note it's abstract base class: typeinfo is not registered 

	Renderable(void);
	~Renderable(void);

	uint GetRenderTag(void) { return m_renderTag; }
	bool HasRenderTag(uint _mask) { return (m_renderTag & _mask) != 0; }
	bool MatchRenderTag(uint _mask) { return (m_renderTag & _mask) == _mask; }


protected:
	friend class Renderer;
	friend class RenderBatch;

	void _Register(bool _newState)  override;
	void _OnTransformChanged(void) override;

	void _InvalidateDbvt(void);
	void _UpdateDbvt(void);

	virtual void _GetWorldBounds(AlignedBox& _bounds) { _bounds.Reset(); }

	//\note Must be work asynchronously
	virtual void _Draw(RenderBatch* _batch) { }

	uint m_renderTag = 0; //!< it must be initialized in derived class

	Renderable* m_prevQueuedDbvtNode = nullptr;
	Renderable* m_nextQueuedDbvtNode = nullptr;
	DbvtNode m_dbvtNode;
	bool m_dbvtNodeInTree = false;
	bool m_dbvtNodeUpdated = true;

	static Renderable* s_dbvtUpdateQueue;
};

//----------------------------------------------------------------------------//
// Light
//----------------------------------------------------------------------------//

class Light : public Renderable
{
public:
	RTTI("Light");

	enum Type
	{
		Point,
		Spot,
		Directional,
	};

	Light(void);
	~Light(void);

protected:

	Type m_type = Point;
	Vector4 m_params;
};

//----------------------------------------------------------------------------//
// Model
//----------------------------------------------------------------------------//

class Model : public Renderable
{
public:
	RTTI("Model");

	Model(void);
	~Model(void);

	MeshPtr m_mesh;

	void _GetWorldBounds(AlignedBox& _bounds) override
	{
		_bounds = m_mesh->GetBounds() * GetWorldTransform();
		ASSERT(_bounds.IsFinite());
	}

	void _Draw(RenderBatch* _batch) override;

};

//----------------------------------------------------------------------------//
// BillboardSet 
//----------------------------------------------------------------------------//

class BillboardSet : public Renderable
{
public:
	RTTI("BillboardSet");

	BillboardSet(void);
	~BillboardSet(void);
};

//----------------------------------------------------------------------------//
// Text3D 
//----------------------------------------------------------------------------//

class Text3D : public BillboardSet
{
public:
	RTTI("Text3D");

	Text3D(void);
	~Text3D(void);
};

//----------------------------------------------------------------------------//
// Particles
//----------------------------------------------------------------------------//

class Particles : public Renderable
{
public:
	RTTI("Particles");

	Particles(void);
	~Particles(void);
};

//----------------------------------------------------------------------------//
// SkyDome 
//----------------------------------------------------------------------------//

class SkyDome : public Renderable
{
public:
	RTTI("SkyDome");

	SkyDome(void);
	~SkyDome(void);
};

//----------------------------------------------------------------------------//
// TerrainSector
//----------------------------------------------------------------------------//

//! TerrainSector. \note do not create directly. For creation do use Terrain.
struct TerrainSector : public Renderable
{
public:
	RTTI("TerrainSector");

	TerrainSector(void);
	~TerrainSector(void);


protected:
	friend class Terrain;

	void _GetWorldBounds(AlignedBox& _bounds) override;
	void _Draw(RenderBatch* _batch) override;

	Terrain* m_terrain = nullptr;
	Vector4 m_params; // offset.x, offset.y, tex_offset.x, tex_offset.y
	Matrix44 m_matrix;

};

//----------------------------------------------------------------------------//
// Terrain
//----------------------------------------------------------------------------//

class Terrain : public Entity
{
public:
	RTTI("Terrain");

	Terrain(void);
	~Terrain(void);

	void Create(Image* _heightmap, float _width, uint _sectors, float _height);
	
	void SetLodDistance(float _dist) { m_lodDistance = Abs(_dist); }
	float GetLodDistance(void) { return m_lodDistance; }

	void SetLodBias(uint _bias) { m_lodBias = Min(_bias, m_lods.Size() - 1); }
	uint GetLodBias(void) { return m_lodBias; }

	uint GetLodCount(void) { return m_lods.Size(); }
	Mesh* GetLod(uint _lod) { return m_lods[_lod]; }

	Material* GetMaterial(void) { return m_material; }

protected:

	ImagePtr m_heightmap;
	TexturePtr m_heightmapTexture;
	TexturePtr m_normalmapTexture;

	float m_lodDistance = 1000;
	uint m_lodBias = 0;
	Array<MeshPtr> m_lods;
	Array<TerrainSector> m_sectors;

	MaterialPtr m_material;
};

//----------------------------------------------------------------------------//
// RibbonTrail
//----------------------------------------------------------------------------//

class RibbonTrail : public Renderable
{
public:
	RTTI("RibbonTrail");

	RibbonTrail(void);
	~RibbonTrail(void);
};

//----------------------------------------------------------------------------//
// Renderable2D
//----------------------------------------------------------------------------//

class Renderable2D : public Renderable
{
public:
	RTTI("Renderable2D");

	enum RenderMode	: uint8
	{
		ScreenSpace,
		WorldSpace,
	};

	Renderable2D(void);
	~Renderable2D(void);

	void SetRenderMode(RenderMode _mode);
	RenderMode GetRenderMode(void) { return m_renderMode; }

	void SetAnchor(const Rect& _anchor);
	const Rect& GetAnchor(void) { return m_anchor; }

	void SetPivot(const Vector2& _pivot);
	const Vector2& GetPivot(void) { return m_pivot; }

	void SetSize(const Vector2& _size);
	const Vector2& GetSize(void) { return m_size; }

	void SetPosition(const Vector2& _pos);
	const Vector2& GetPosition(void) { return m_position; }

	void SetColor(const Vector4& _color) { m_color = _color; }
	const Vector4& GetColor(void) { return m_color; }
	Vector4& Color(void) { return m_color; }

protected:
	friend class Renderer;

	void _Register(bool _newState) override;
	void _OnChangeParent(Entity* _newParent) override;
	void _OnTransformChanged(void) override;
	void _GetWorldBounds(AlignedBox& _bounds) override;

	void _InvalidateTransform2D(void);
	void _UpdateTransform2D(void);
	void _UpdateZBiasRecursive(float _parentZBias);

	Renderable2D* m_parent2D = nullptr;
	Renderable2D* m_prev2D = nullptr;
	Renderable2D* m_next2D = nullptr;
	Renderable2D* m_child2D = nullptr;

	bool m_transform2DUpdated = true;
	RenderMode m_renderMode = ScreenSpace;
	Rect m_anchor = Rect::Identity;
	Vector2 m_pivot = .5f;
	Vector2 m_size = Vector2::Zero;
	Vector2 m_position = Vector2::Zero;
	Rect m_rect = Rect::Zero;
	float m_localZBias = 0.02f;
	float m_zBias = 0;
	Vector4 m_color = Vector4::One;

	Vector3 m_worldRect[4]; 

	static Renderable2D* s_root;
};

//----------------------------------------------------------------------------//
// Sprite
//----------------------------------------------------------------------------//

class Sprite : public Renderable2D
{
public:
	RTTI("Sprite");

	Sprite(void);
	~Sprite(void);

	void SetTexture(Texture* _texture, uint _frame = (uint)-1);

protected:
	void _Draw(RenderBatch* _batch) override;

	TexturePtr m_texture;
	uint m_frame = (uint)-1;
};

//----------------------------------------------------------------------------//
// Text
//----------------------------------------------------------------------------//

class Text : public Renderable2D
{
public:
	RTTI("Text");

	Text(void);
	~Text(void);

	enum Alignment
	{
		Center,
		Left,
		Right,
		Top = Left,
		Bottom = Right,
	};

	enum WrapMode
	{
		Overflow,
		Wrap, // horizontal
		Truncate = Wrap, // vertical
	};

	void SetFont(Texture* _texture, uint _baseFrame = 0);
	void SetFontSize(float _size);
	void SetText(const String& _text) { m_text = _text; }
	void SetAlignment(Alignment _horizontal, Alignment _vertical);
	void SetWrapMode(WrapMode _horizontal, WrapMode _vertical);

protected:
	void _Draw(RenderBatch* _batch) override;

	TexturePtr m_texture;
	uint m_baseFrame = 0;
	float m_fontSize = -1;
	String m_text;
	Alignment m_hAlignment = Left;
	Alignment m_vAlignment = Top;
	WrapMode m_hWrap = Overflow;
	WrapMode m_vWrap = Truncate;
};

//----------------------------------------------------------------------------//
// Renderer
//----------------------------------------------------------------------------//

struct RendererEvent
{
	enum Enum
	{
#ifdef _DEBUG_GRAPHICS
		DebugDraw = String::ConstHash("RendererEvent::DebugDraw"), // arg = RenderBatch*
#endif
	};
};

class Renderer : public SceneSystem, public Singleton<Renderer>
{
public:
	Renderer(void);
	~Renderer(void);

	Dbvt* GetDbvt(void) { return &m_dbvt; }

	void SetClearColor(const Vector4& _color) { m_clearColor = _color; }
#ifdef _DEBUG_GRAPHICS

	void DrawDebugLine(const Vector3& _start, const Vector3& _end, const Vector4& _color = Vector4::Identity);

#endif

public:

	void Begin2D(void);

protected:

	EventResult _OnEvent(int _type, void* _data) override;


	Vector4 m_clearColor = Vector4::Zero;
	//////////////////////////////////////////////////////

	void _DrawBatch(RenderBatch* _batch, uint _subset);
	void _PrepareBatch(RenderBatch* _batch);

	bool _AddObject(void* _object, const AlignedBox& _bbox, bool _withTest, void* _userData);
	void _PrepareModels(void); // async


	void _Prepare2D(void); // async

	void _Prepare2DOverlay(void); // async
	void _Draw2DOverlay(void);

	RenderBatch m_models;
	RenderBatch m_2D;
	RenderBatch m_2DOverlay;

#ifdef _DEBUG_GRAPHICS
	RenderBatch m_debugGeometry;
	RenderOp* m_currentDebugRop = nullptr;
	void _DebugDraw(void);
	void _DrawDebugGeometry(bool _drawDbvt, bool _drawBBox);
#endif

	AsyncCall<void()> m_asyncPrepareModels;
	AsyncCall<void()> m_asyncPrepare2D;
	AsyncCall<void()> m_asyncPrepare2DOverlay;

	Camera::Params m_camera2DOverlay;

	void _Draw(void);
	void _Draw(Camera* _camera);


	Dbvt m_dbvt;

	DbvtFrustumCallback m_frustumCallback;
	uint m_currentMask;
	uint m_currentLayer;

	//////////////////////////////////////////////////////


	//////////////////////////////////////////////////////
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
