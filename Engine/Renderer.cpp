#pragma once

#include "Renderer.hpp"
#include "Timer.hpp"
#include "Device.hpp"

//----------------------------------------------------------------------------//
// Camera::Params
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void Camera::Params::Export(UCamera* _cam) const
{
	_cam->ViewMat = viewMat;
	_cam->ProjMat = projMat;
	_cam->ViewProjMat = viewProjMat;
	_cam->InvViewProjMat = invViewProjMat;
	_cam->NormMat = normMat;
	_cam->CameraPos = pos;
	_cam->Depth = depth;
	_cam->ScreenSize = rect.Size();
	_cam->InvScreenSize = 1.f / Vector2(_cam->ScreenSize);
}
//----------------------------------------------------------------------------//
void Camera::Params::Update(const Matrix44& _worldMat, const Vector2& _windowSize)
{
	rect = viewport;
	if (useRelativeSize)
		rect = viewport * _windowSize;

	float _w = rect.Width();
	float _h = rect.Height();

	pos = _worldMat.GetTranslation();
	viewMat.CreateTransform(_worldMat.GetTranslation(), _worldMat.GetRotation(), Vector3::One).Inverse();
	normMat = viewMat.Copy().Inverse();
	projMat.CreateProjection(fov, _w / _h, nearClip, farClip, zoom, rect.Position(), _h);
	viewProjMat = projMat * viewMat;
	invViewProjMat = viewProjMat.Copy().Inverse();
	frustum.FromCameraMatrices(viewMat, projMat);

	// http://outerra.blogspot.com/2012/11/maximizing-depth-buffer-range-and.html
	// depth params (near, far, C=constant, FC = 1.0/log(far*C + 1))
	// the camera depth is: (exp(depth/FC)-1.0)/C
	depth.x = nearClip;
	depth.y = farClip;
	depth.z = depthC;
	depth.w = 1 / Log(farClip * depthC + 1);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Camera
//----------------------------------------------------------------------------//

Camera* Camera::s_root = nullptr;
Camera* Camera::s_active = nullptr;

//----------------------------------------------------------------------------//
Camera::Camera(void)
{
}
//----------------------------------------------------------------------------//
Camera::~Camera(void)
{
}
//----------------------------------------------------------------------------//
void Camera::_Register(bool _newState)
{
	if (_newState)
	{
		Link(s_root, this, m_prevCamera);
		_InvalidateParams();
	}
	else
	{
		Unlink(s_root, this, m_prevCamera);
	}
}
//----------------------------------------------------------------------------//
void Camera::_OnTransformChanged(void)
{
	_InvalidateParams();
}
//----------------------------------------------------------------------------//
void Camera::_InvalidateParams(void)
{
	m_paramsUpdated = false;
}
//----------------------------------------------------------------------------//
void Camera::_UpdateParams(void)
{
	if (!m_paramsUpdated)
	{
		m_paramsUpdated = true;
		m_params.Update(GetWorldTransform(), gDevice->Size());
	}
}
//----------------------------------------------------------------------------//
void Camera::SetRelativeSize(bool _use)
{ 
	m_params.useRelativeSize = _use;
	_InvalidateParams();
}
//----------------------------------------------------------------------------//
void Camera::SetFov(float _fovY)
{ 
	m_params.fov = _fovY;
	_InvalidateParams();
}
//----------------------------------------------------------------------------//
void Camera::SetNear(float _near)
{ 
	m_params.nearClip = _near;
	_InvalidateParams();
}
//----------------------------------------------------------------------------//
void Camera::SetFar(float _far)
{ 
	m_params.farClip = _far;
	_InvalidateParams();
}
//----------------------------------------------------------------------------//
void Camera::SetZoom(float _zoom)
{ 
	m_params.zoom = _zoom;
	_InvalidateParams();
}
//----------------------------------------------------------------------------//
const Camera::Params* Camera::GetParams(void)
{
	_UpdateParams();
	return &m_params;
}
//----------------------------------------------------------------------------//
Ray Camera::ScreenToWorld(const Vector2& _cursor)
{
	_UpdateParams();

	Ray _r;
	Vector2 _pt;
	_pt.x = 2 * _cursor.x - 1;
	_pt.y = 1 - 2 * _cursor.y;
	_r.origin = m_params.invViewProjMat.TransformProj({ _pt.x, _pt.y, 0 });
	_r.dir = (m_params.invViewProjMat.TransformProj({ _pt.x, _pt.y, 1 }) - _r.origin).Normalize();

	return _r;
}
//----------------------------------------------------------------------------//
Vector3 Camera::WorldToScreen(const Vector3& _pt, bool _clamp01)
{
	_UpdateParams();

	Vector3 _pos = m_params.viewMat.Transform(_pt);
	float _z = _pos.z; // distance
	_pos = m_params.projMat.TransformProj(_pos);
	_pos.x = _pos.x * .5f + .5f;
	_pos.y = 1 - (_pos.y * .5f + .5f);
	_pos.z = _z;

	if (_clamp01)
	{
		_pos.x = Clamp01(_pos.x);
		_pos.y = Clamp01(_pos.y);
	}

	return _pos;

}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// RenderOp
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
bool RenderOp::Cmp(RenderOp* _lhs, RenderOp* _rhs)
{
	if (_lhs->texture == _rhs->texture)
	{
		if (_lhs->material == _rhs->material)
		{
			if (_lhs->type == _rhs->type)
			{
				if (_lhs->vertexBuffer == _rhs->vertexBuffer)
				{
					if (_lhs->indexBuffer == _rhs->indexBuffer)
					{
						if (_lhs->firstVertex == _rhs->firstVertex)
						{
							if (_lhs->firstIndex == _rhs->firstIndex)
							{
								if (_lhs->count == _rhs->count)
								{
									return _lhs->topology < _rhs->topology;
								}
								return _lhs->count < _rhs->count;
							}
							return _lhs->firstIndex < _rhs->firstIndex;
						}
						return _lhs->firstVertex < _rhs->firstVertex;
					}
					return _lhs->indexBuffer < _rhs->indexBuffer;
				}
				return _lhs->vertexBuffer < _rhs->vertexBuffer;
			}
			return _lhs->type < _rhs->type;
		}
		return _lhs->material < _rhs->material;
	}
	return _lhs->texture < _rhs->texture;
}
//----------------------------------------------------------------------------//
bool RenderOp::Equals(RenderOp* _lhs, RenderOp* _rhs)
{
	// TODO: use hash
	return _lhs->material == _rhs->material &&
		_lhs->texture == _rhs->texture &&
		_lhs->vertexBuffer == _rhs->vertexBuffer &&
		_lhs->indexBuffer == _rhs->indexBuffer &&
		_lhs->count == _rhs->count &&
		_lhs->firstIndex == _rhs->firstIndex &&
		_lhs->firstVertex == _rhs->firstVertex &&
		_lhs->type == _rhs->type &&
		_lhs->topology == _rhs->topology;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// RenderBatch
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
RenderBatch::RenderBatch(void) :
	m_tempVertices(new Buffer(BufferUsage::Dynamic)),
	m_tempIndices(new Buffer(BufferUsage::Dynamic)),
	m_instanceData(new Buffer(BufferUsage::Dynamic))
{
}
//----------------------------------------------------------------------------//
void RenderBatch::AddObject(class Renderable* _object)
{
	ASSERT(_object != nullptr);
	m_objects.Push(_object);
}
//----------------------------------------------------------------------------//
RenderOp* RenderBatch::AddRenderOp(void)
{
	RenderOp* _rop = m_ropStorage.New();
	m_rops.Push(_rop);
	return _rop;
}
//----------------------------------------------------------------------------//
void RenderBatch::Build(void)
{
	for (Renderable* i : m_objects)
		i->_Draw(this);

	Sort(m_rops.Begin(), m_rops.End(), RenderOp::Cmp);

	for (uint i = 0; i < m_rops.Size();)
	{
		Item _item;
		_item.op = m_rops[i];
		_item.baseInstance = m_instanceData->ElementsCount<InstanceData>();
		while (i < m_rops.Size())
		{
			RenderOp* _op = m_rops[i];

			if (RenderOp::Equals(_item.op, _op))
			{
				_item.numInstances++;
				InstanceData* _data = m_instanceData->AppendElements<InstanceData>(1);
				_data->matrix = *_op->matrix;
				_data->params = *_op->instanceParams;

				if (_op->type == RenderOp::SkinnedModel)
				{
					TODO("Write bones and skeleton offset");
				}

				i++;
			}
			else
				break;
		}

		m_items[_item.op->GetRenderMode()].Push(_item);
	};
}
//----------------------------------------------------------------------------//
void RenderBatch::Clear(void)
{
	for (RenderOp* i : m_rops)
		m_ropStorage.Delete(i);
	m_rops.Clear();
	m_items[0].Clear();
	m_items[1].Clear();
	m_objects.Clear();
	m_tempVertices->ClearStream();
	m_tempIndices->ClearStream();
	m_instanceData->ClearStream();
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Renderable
//----------------------------------------------------------------------------//

Renderable* Renderable::s_dbvtUpdateQueue = nullptr;

//----------------------------------------------------------------------------//
Renderable::Renderable(void)
{
	m_dbvtNode.object = this;
}
//----------------------------------------------------------------------------//
Renderable::~Renderable(void)
{
}
//----------------------------------------------------------------------------//
void Renderable::_Register(bool _newState)
{
	if (_newState)
	{
		m_dbvtNodeUpdated = false;
		Link(s_dbvtUpdateQueue, this, m_prevQueuedDbvtNode);
	}
	else
	{
		if (!m_dbvtNodeUpdated)
		{
			Unlink(s_dbvtUpdateQueue, this, m_prevQueuedDbvtNode);
			m_dbvtNodeUpdated = true;
		}

		if (m_dbvtNodeInTree)
		{
			gRenderer->GetDbvt()->Remove(&m_dbvtNode);
			m_dbvtNodeInTree = false;
		}
	}
}
//----------------------------------------------------------------------------//
void Renderable::_OnTransformChanged(void)
{
	_InvalidateDbvt();
}
//----------------------------------------------------------------------------//
void Renderable::_InvalidateDbvt(void)
{
	if (m_inScene && m_dbvtNodeUpdated)
	{
		m_dbvtNodeUpdated = false;
		Link(s_dbvtUpdateQueue, this, m_prevQueuedDbvtNode);
	}
}
//----------------------------------------------------------------------------//
void Renderable::_UpdateDbvt(void)
{
	ASSERT(m_dbvtNodeUpdated == false);

	_GetWorldBounds(m_dbvtNode.box); // update world bounds (local bounds * world matrix)

	if (m_dbvtNode.box.IsFinite())
	{
		if (m_dbvtNodeInTree)
		{
			gRenderer->GetDbvt()->Update(&m_dbvtNode);
		}
		else
		{
			gRenderer->GetDbvt()->Insert(&m_dbvtNode);
			m_dbvtNodeInTree = true;
		}
	}
	else if (m_dbvtNodeInTree)
	{
		gRenderer->GetDbvt()->Remove(&m_dbvtNode);
		m_dbvtNodeInTree = false;
	}

	Unlink(s_dbvtUpdateQueue, this, m_prevQueuedDbvtNode);
	m_dbvtNodeUpdated = true;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Light
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Light::Light(void)
{
	m_renderTag = RenderTag::PointLight;
}
//----------------------------------------------------------------------------//
Light::~Light(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Model
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Model::Model(void)
{
	m_renderTag = RenderTag::Model;
}
//----------------------------------------------------------------------------//
Model::~Model(void)
{
}
//----------------------------------------------------------------------------//
void Model::_Draw(RenderBatch* _batch)
{
	ASSERT(m_mesh != nullptr);

	float _distance = _batch->View()->frustum.origin.DistanceSq(GetWorldPosition());

	//for (Mesh* _mesh = m_mesh; _mesh; _mesh = _mesh->NextSubmesh())
	Mesh* _mesh = m_mesh;
	{
		RenderOp* _op = _batch->AddRenderOp();
		_op->object = this;
		
		_op->material = _mesh->GetMaterial(); // TODO
		_op->texture = nullptr; // TODO

		_op->type = RenderOp::StaticModel; // TODO

		_op->topology = _mesh->Type();
		_op->vertexBuffer = _mesh->VertexBuffer();
		_op->indexBuffer = _mesh->IndexBuffer();
		_op->count = _mesh->Indices().Size();
		_op->firstIndex = 0; // TODO
		_op->firstVertex = 0; // TODO

		_op->matrix = &m_worldTransform;
		_op->instanceParams = &Vector4::Zero; // TODO
		_op->bones = nullptr; // TODO
		_op->numBones = 0; // TODO

		_op->sortKey = _distance;
	}

}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// BillboardSet 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
BillboardSet::BillboardSet(void)
{
	m_renderTag = RenderTag::BillboardSet;
}
//----------------------------------------------------------------------------//
BillboardSet::~BillboardSet(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Text3D 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Text3D::Text3D(void)
{
	m_renderTag = RenderTag::Text3D;
}
//----------------------------------------------------------------------------//
Text3D::~Text3D(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Particles
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Particles::Particles(void)
{
	m_renderTag = RenderTag::Particles;
}
//----------------------------------------------------------------------------//
Particles::~Particles(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// SkyDome 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
SkyDome::SkyDome(void)
{
	m_renderTag = RenderTag::SkyDome;
}
//----------------------------------------------------------------------------//
SkyDome::~SkyDome(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// TerrainSector
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
TerrainSector::TerrainSector(void)
{
	//m_renderTag = RenderTag::Terrain;
	m_renderTag = RenderTag::Model; // temp
}
//----------------------------------------------------------------------------//
TerrainSector::~TerrainSector(void)
{
}
//----------------------------------------------------------------------------//
void TerrainSector::_GetWorldBounds(AlignedBox& _bounds)
{
	ASSERT(m_terrain != nullptr);
	
	_bounds = m_terrain->GetLod(0)->GetBounds() * GetWorldTransform();
}
//----------------------------------------------------------------------------//
void TerrainSector::_Draw(RenderBatch* _batch)
{
	float _distance = _batch->View()->frustum.origin.DistanceSq(m_dbvtNode.box.Center()) - m_dbvtNode.box.RadiusSq();
	if (_distance <= 0)
		_distance = 0;
	else
		_distance = Sqrt(_distance);

	uint _minLod = m_terrain->GetLodBias();
	uint _maxLod = m_terrain->GetLodCount()-1;
	float _t = Clamp01(_distance / m_terrain->GetLodDistance());

	uint _lod = (uint)Cerp((float)_minLod, (float)(_maxLod), _t);
	Mesh* _mesh = m_terrain->GetLod(_lod);
	//if(0)
	{
		RenderOp* _op = _batch->AddRenderOp();
		_op->object = this;

		_op->material = _mesh->GetMaterial(); // TODO
		_op->texture = nullptr;

		_op->type = RenderOp::Terrain; // TODO

		_op->topology = _mesh->Type();
		_op->vertexBuffer = _mesh->VertexBuffer();
		_op->indexBuffer = _mesh->IndexBuffer();
		_op->count = _mesh->Indices().Size();
		_op->firstIndex = 0; // TODO
		_op->firstVertex = 0; // TODO

		_op->matrix = &m_worldTransform;
		_op->instanceParams = &m_params;
		_op->bones = nullptr; // TODO
		_op->numBones = 0; // TODO

		_op->sortKey = _distance;
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Terrain
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Terrain::Terrain(void)
{
}
//----------------------------------------------------------------------------//
Terrain::~Terrain(void)
{
}
//----------------------------------------------------------------------------//
void Terrain::Create(Image* _heightmap, float _width, uint _sectors, float _height)
{
	ASSERT(_sectors > 0);
	ASSERT(_heightmap != nullptr);

	m_heightmap = _heightmap;

	//ImagePtr _normalmap = new Image;
	//_normalmap->CreateNormalmapFromHeightmap(_heightmap, _height);

	m_normalmapTexture = _heightmap->CreateNormalmapFromHeightmap(_height * .5f, false)->Export2D(PixelFormat::RGB10A2); // temp

	// create material

	m_material = new Material;
	m_material->texture[MaterialTexture::TerrainHeight] = m_heightmap->Export2D(PixelFormat::R32F); // temp
	m_material->texture[MaterialTexture::TerrainNormal] = m_normalmapTexture; // temp

	// create lods

	float _sectorWidth = _width / _sectors;
	float _texWidth = 1.f / _sectors;
	for (uint i = 0; i < 8; ++i) // 129, 65, 33, 17, 9, 5, 3, 2
	{
		MeshPtr _lod = new Mesh;
		_lod->CreateTerrainSector(_sectorWidth, (1 << (7 - i)) + 1, _texWidth, 1);
		_lod->SetMaterial(m_material);

		m_lods.Push(_lod);
	}

	// create sectors

	Vector3 _scale(1, _height, 1);
	for (uint y = 0; y < _sectors; ++y)
	{
		float _py = (_width * -.5f) + y * _sectorWidth;
		for (uint x = 0; x < _sectors; ++x)
		{
			float _px = (_width * -.5f) + x * _sectorWidth;
			TerrainSector* _sector = AddChild<TerrainSector>();
			_sector->m_terrain = this;
			_sector->m_params.x = x * _texWidth;
			_sector->m_params.y = y * _texWidth;

			_sector->SetLocalPosition({ _px, _height * -.5f, _py });
			_sector->SetLocalScale(_scale);
		}
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// RibbonTrail
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
RibbonTrail::RibbonTrail(void)
{
	m_renderTag = RenderTag::RibbonTrail;
}
//----------------------------------------------------------------------------//
RibbonTrail::~RibbonTrail(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Renderable2D
//----------------------------------------------------------------------------//

Renderable2D* Renderable2D::s_root = nullptr;

//----------------------------------------------------------------------------//
Renderable2D::Renderable2D(void)
{
	m_renderTag = RenderTag::FlatOverlay;
}
//----------------------------------------------------------------------------//
Renderable2D::~Renderable2D(void)
{
}
//----------------------------------------------------------------------------//
void Renderable2D::SetRenderMode(RenderMode _mode)
{
	if (m_renderMode != _mode)
	{
		m_renderMode = _mode;
		_InvalidateDbvt();
		if (m_inScene)
		{
			if (_mode == ScreenSpace)
				Link(s_root, this, m_prev2D);
			else
				Unlink(s_root, this, m_prev2D);
		}

		m_renderTag &= ~(RenderTag::Flat | RenderTag::FlatOverlay);
		m_renderTag |= (_mode == ScreenSpace) ? RenderTag::Flat : RenderTag::FlatOverlay;

		for (Renderable2D* i = m_child2D; i; i = i->m_next2D)
			i->SetRenderMode(_mode);
	}
}
//----------------------------------------------------------------------------//
void Renderable2D::SetAnchor(const Rect& _anchor)
{
	_InvalidateTransform2D();
	m_anchor = _anchor;
}
//----------------------------------------------------------------------------//
void Renderable2D::SetPivot(const Vector2& _pivot)
{
	m_pivot = _pivot;
	_InvalidateTransform2D();
}
//----------------------------------------------------------------------------//
void Renderable2D::SetSize(const Vector2& _size)
{
	m_size = _size;
	_InvalidateTransform2D();
}
//----------------------------------------------------------------------------//
void Renderable2D::SetPosition(const Vector2& _pos)
{
	m_position = _pos;
	_InvalidateTransform2D();
}
//----------------------------------------------------------------------------//
void Renderable2D::_Register(bool _newState)
{
	Renderable::_Register(_newState);
	if (m_renderMode == ScreenSpace)
	{
		if (_newState)
		{
			Link(s_root, this, m_prev2D);
			_InvalidateTransform2D();
		}
		else
			Unlink(s_root, this, m_prev2D);
	}
}
//----------------------------------------------------------------------------//
void Renderable2D::_OnChangeParent(Entity* _newParent)
{
	if (m_parent2D)
		Unlink(m_parent2D->m_child2D, this, m_prev2D);
	else if (m_inScene && m_renderMode == ScreenSpace)
		Unlink(s_root, this, m_prev2D);

	if (_newParent && _newParent->IsTypeOf<Renderable2D>())
	{
		m_parent2D = static_cast<Renderable2D*>(_newParent);
		Link(m_parent2D->m_child2D, this, m_prev2D);
		SetRenderMode(m_parent2D->m_renderMode);
	}
	else
	{
		if (m_inScene && m_renderMode == ScreenSpace)
			Link(s_root, this, m_prev2D);
		m_parent2D = nullptr;
	}

	_UpdateZBiasRecursive(m_parent2D ? m_parent2D->m_zBias : 0);
	_InvalidateTransform2D();
}
//----------------------------------------------------------------------------//
void Renderable2D::_OnTransformChanged(void)
{
	Renderable::_OnTransformChanged();
	_InvalidateTransform2D();
}
//----------------------------------------------------------------------------//
void Renderable2D::_GetWorldBounds(AlignedBox& _bounds)
{
	_UpdateTransform2D();
	_bounds.Reset();
	_bounds.AddVertices(m_worldRect, 4);
}
//----------------------------------------------------------------------------//
void Renderable2D::_InvalidateTransform2D(void)
{
	if (m_transform2DUpdated)
	{
		m_transform2DUpdated = false;
		_InvalidateDbvt();

		for (Renderable2D* i = m_child2D; i; i = i->m_next2D)
			i->_InvalidateTransform2D();
	}
}
//----------------------------------------------------------------------------//
void Renderable2D::_UpdateTransform2D(void)
{
	if (!m_transform2DUpdated)
	{
		m_transform2DUpdated = true;

		Rect _parentRect;
		if (m_parent2D)
		{
			m_parent2D->_UpdateTransform2D();
			_parentRect = m_parent2D->m_rect;
		}
		else
		{
			_parentRect.SetOrigin(gDevice->Size() * -.5f);
			_parentRect.SetSize(gDevice->Size());
		}

		//rect

		m_rect.left = _parentRect.left + _parentRect.Width() * m_anchor.left;
		m_rect.top = _parentRect.top + _parentRect.Height() * m_anchor.top;

		if (m_anchor.AbsWidth() < EPSILON)
			m_rect.right = m_rect.left + m_size.x;
		else
			m_rect.right = _parentRect.left + _parentRect.Width() * m_anchor.right;

		if (m_anchor.AbsHeight() < EPSILON)
			m_rect.bottom = m_rect.top + m_size.y;
		else
			m_rect.bottom = _parentRect.top + _parentRect.Height() * m_anchor.bottom;

		// pivot 

		m_rect.SetOrigin(m_position + (m_rect.Origin() - (m_rect.Size() * m_pivot)));
		
		// world rect

		m_worldRect[0].Set(m_rect.left, m_rect.top, m_zBias);
		m_worldRect[1].Set(m_rect.left, m_rect.bottom, m_zBias);
		m_worldRect[2].Set(m_rect.right, m_rect.bottom, m_zBias);
		m_worldRect[3].Set(m_rect.right, m_rect.top, m_zBias);

		_UpdateTransform();
		for (uint i = 0; i < 4; ++i)
			m_worldRect[i] = m_worldRect[i] * m_worldTransform;
	}
}
//----------------------------------------------------------------------------//
void Renderable2D::_UpdateZBiasRecursive(float _parentZBias)
{
	m_zBias = m_localZBias - _parentZBias;
	for (Renderable2D* i = m_child2D; i; i = i->m_next2D)
		i->_UpdateZBiasRecursive(m_zBias);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Sprite
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Sprite::Sprite(void)
{
}
//----------------------------------------------------------------------------//
Sprite::~Sprite(void)
{
}
//----------------------------------------------------------------------------//
void Sprite::_Draw(RenderBatch* _batch)
{
	ASSERT(m_transformUpdated);
	ASSERT(m_transform2DUpdated);

	Vertex _sprite;
	_sprite.SetRect(m_rect, m_zBias);
	_sprite.SetColor(m_color); // TODO: transparency
	_sprite.SetTexRect(Texture::GetRect(m_texture, m_frame));

	RenderOp* _op = _batch->AddRenderOp();
	_op->object = this;

	_op->texture = m_texture;

	_op->type = RenderOp::Sprite;

	_op->topology = PrimitiveType::Points;
	_op->vertexBuffer = _batch->TempVertices();
	_op->count = 1;
	_op->firstVertex = _batch->TempVertices()->AppendElement(_sprite);

	_op->matrix = &m_worldTransform; // TODO
	_op->instanceParams = &Vector4::Zero; // TODO

	_op->sortKey = _batch->View()->frustum.origin.Distance(GetWorldPosition()); // TODO: +_zBias
}
//----------------------------------------------------------------------------//
void Sprite::SetTexture(Texture* _texture, uint _frame)
{
	m_texture = _texture;
	m_frame = _frame;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Text
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Text::Text(void)
{
}
//----------------------------------------------------------------------------//
Text::~Text(void)
{
}
//----------------------------------------------------------------------------//
void Text::SetFont(Texture* _texture, uint _baseFrame)
{
	m_texture = _texture;
	m_baseFrame = _baseFrame;
}
//----------------------------------------------------------------------------//
void Text::SetFontSize(float _size)
{
	m_fontSize = _size;
}
//----------------------------------------------------------------------------//
void Text::SetAlignment(Alignment _horizontal, Alignment _vertical)
{
	m_hAlignment = _horizontal;
	m_vAlignment = _vertical;
}
//----------------------------------------------------------------------------//
void Text::SetWrapMode(WrapMode _horizontal, WrapMode _vertical)
{
	m_hWrap = _horizontal;
	m_vWrap = _vertical;
}
//----------------------------------------------------------------------------//
void Text::_Draw(RenderBatch* _batch)
{
	ASSERT(m_transformUpdated);
	ASSERT(m_transform2DUpdated);

	if (!m_texture || m_text.Length() < 1)
		return;

	Vector2 _scale((float)m_texture->Width(), (float)m_texture->Height());
	float _height = (Texture::GetRect(m_texture, m_baseFrame + (uint8)'\n') * _scale).Height();

	if(m_fontSize >= 0)
		_scale *= m_fontSize / _height;

	Array<Rect> _rects;
	{
		Vector2 _pos = m_rect.Origin();
		uint e = 0, s = 0;
		while (e < m_text.Length())
		{
			// line

			s = e;
			while (e < m_text.Length())
			{
				Rect _r = Texture::GetRect(m_texture, m_baseFrame + (uint8)m_text[e]) * _scale;
				if (m_text[e] == '\n')
					_r = Rect::Zero;
				_r.SetOrigin(_pos);
				
				if ((_pos.x + _r.Width() > m_rect.right && (m_text[e] == ' ' || m_text[e] == '\t' || m_hWrap == Wrap || e == s)) || m_text[e] == '\n')
				{
					if (e == s)
					{
						_rects.Push(_r);
						++e;
					}

					if(m_text[e] != '\n')
						_pos.y += _r.Height();
					_pos.x = m_rect.left;
					break;
				}

				_rects.Push(_r);
				_pos.x += _r.Width();

				++e;
			}

			// horizontal alignment

			float _lineWidth = _rects[e - 1].right - _rects[s].left;
			if (m_hAlignment != Left)
			{
				float _move = m_rect.Width() - _lineWidth;
				if (m_hAlignment == Center)
					_move *= .5f;

				for (uint j = s; j < e; ++j)
				{
					_rects[j].left += _move;
					_rects[j].right += _move;
				}
			}
		}

		// vertical alignment
		{
			float _textHeight = _rects.Back().bottom - _rects.Front().top;
			if (m_vAlignment != Top)
			{
				float _move =  m_rect.Height() - _textHeight;
				if (m_vAlignment == Center)
					_move *= .5f;

				for (Rect& _r : _rects)
				{
					_r.top += _move;
					_r.bottom += _move;
				}
			}
		}
	}

	// draw

	Vertex _sprite;
	_sprite.SetColor(m_color);

	RenderOp* _op = _batch->AddRenderOp();
	_op->object = this;

	_op->material = 0;
	_op->texture = m_texture;

	_op->type = RenderOp::Sprite;

	_op->topology = PrimitiveType::Points;
	_op->vertexBuffer = _batch->TempVertices();
	_op->firstVertex = _batch->TempVertices()->ElementsCount<Vertex>();

	_op->matrix = &m_worldTransform;
	_op->instanceParams = &Vector4::Zero;

	_op->sortKey = _batch->View()->frustum.origin.Distance(GetWorldPosition());

	for(uint i = 0; i < _rects.Size(); ++i)
	{
		const Rect& _r = _rects[i];
		if (m_vWrap != Truncate)
		{
			if (_r.top < m_rect.top)
				continue;
			if (_r.bottom > m_rect.bottom)
				break;
		}

		const Rect& _tc = Texture::GetRect(m_texture, m_baseFrame + (uint8)m_text[i]);

		_sprite.SetRect(_r, m_zBias);
		_sprite.SetTexRect(_tc);
		_batch->TempVertices()->AppendElement(_sprite);
		_op->count++;
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Renderer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Renderer::Renderer(void)
{
	LOG_NODE("Create Renderer");

	gReflection->GetOrCreateTypeInfo<Camera>()->SetFactory([]() { return ObjectPtr(new Camera); });
	gReflection->GetOrCreateTypeInfo<Light>()->SetFactory([]() { return ObjectPtr(new Light); });
	gReflection->GetOrCreateTypeInfo<Model>()->SetFactory([]() { return ObjectPtr(new Model); });
	gReflection->GetOrCreateTypeInfo<Particles>()->SetFactory([]() { return ObjectPtr(new Particles); });
	gReflection->GetOrCreateTypeInfo<SkyDome>()->SetFactory([]() { return ObjectPtr(new SkyDome); });
	gReflection->GetOrCreateTypeInfo<TerrainSector>()->SetFactory([]() { return ObjectPtr(new TerrainSector); });
	gReflection->GetOrCreateTypeInfo<Terrain>()->SetFactory([]() { return ObjectPtr(new Terrain); });
	gReflection->GetOrCreateTypeInfo<RibbonTrail>()->SetFactory([]() { return ObjectPtr(new RibbonTrail); });

	gReflection->GetOrCreateTypeInfo<Sprite>()->SetFactory([]() { return ObjectPtr(new Sprite); });
	gReflection->GetOrCreateTypeInfo<Text>()->SetFactory([]() { return ObjectPtr(new Text); });


	m_asyncPrepareModels.func = { this, &Renderer::_PrepareModels };
	m_asyncPrepare2D.func = { this, &Renderer::_Prepare2D };
	m_asyncPrepare2DOverlay.func = { this, &Renderer::_Prepare2DOverlay };
}
//----------------------------------------------------------------------------//
Renderer::~Renderer(void)
{
	LOG_NODE("Destroy Renderer");
}
//----------------------------------------------------------------------------//
EventResult Renderer::_OnEvent(int _type, void* _data)
{
	switch (_type)
	{
	case SystemEvent::BeginFrame:
		break;

	case SystemEvent::Render:
	{
		_Draw();
#ifdef _DEBUG_GRAPHICS
		_DebugDraw();
#endif
	}
	break;

	case DeviceEvent::Resize:
	{
		Vector2 _size = *reinterpret_cast<const Vector2*>(_data);
		for (Renderable2D* i = Renderable2D::s_root; i; i = i->m_next2D)
		{
			i->_InvalidateTransform2D();
		}

		for (Camera* i = Camera::s_root; i; i = i->m_nextCamera)
		{
			i->_InvalidateParams();
		}

	} break;

	}

	return ER_Pass;
}
//----------------------------------------------------------------------------//
void Renderer::Begin2D(void)
{
	//if (!m_2dmode)
	{
		//m_2dmode = true;
		/*Rect _viewport = Rect::Identity * gDevice->Size();
		TODO("Set viewport");

		gGraphics->ResetRenderTargets();
		gGraphics->EnableRenderTargets(false);

		Camera::CreateParams(gGraphics->Camera, Matrix44::Identity, _viewport, 0, 0, 1, 1);
		gGraphics->Invalidate(ShaderParam::Camera);

		gGraphics->RasterizerParams->DepthBias = 0.1f;
		gGraphics->Invalidate(ShaderParam::RasterizerParams);

		gGraphics->SetInstanceData(nullptr);
		gGraphics->SetSkeletonOffset(nullptr);

		gGraphics->SetDepthState(DepthState::Disabled); //
		gGraphics->SetBlendState(BlendState::Translucent);
		gGraphics->SetSampler(0, Sampler::TrilinearRepeat);	*/
	}
}
//----------------------------------------------------------------------------//
void Renderer::_Draw(void)
{
	// Update Dbvt
	while (Renderable::s_dbvtUpdateQueue)
		Renderable::s_dbvtUpdateQueue->_UpdateDbvt();

	// prepare 2D overlay
	gThreadPool->AddTask(&m_asyncPrepare2DOverlay);

	gGraphics->ClearFrameBuffers(FrameBufferType::FBT_All, 0, 1, m_clearColor); // Vector4(.6f, .6f, .65f, 1)

	TODO("One active camera");
	for(int _priority = -10000;;) // sort cameras and render it
	{
		Camera* _camera = nullptr;
		for (Camera* i = Camera::s_root; i; i = i->m_nextCamera)
		{
			if (i->IsEnabled() && i->m_lastFrame != gTimer->FrameCount())
			{
				if (_priority <= i->m_priority)
				{
					_priority = i->m_priority;
					_camera = i;
				}
			}
		}
		if (_camera)
		{
			_Draw(_camera);
			_camera->m_lastFrame = gTimer->FrameCount();
		}
		else
			break;
	}

	_Draw2DOverlay();
}
//----------------------------------------------------------------------------//
void Renderer::_Draw(Camera* _camera)
{
	// setup camera
	{
		const Camera::Params* _params = _camera->GetParams();
		_params->Export(gGraphics->Camera);
		gGraphics->Invalidate(ShaderParam::Camera);
		//gGraphics->SetViewport(_params.rect);
	}

	// rasterizer params
	{
		gGraphics->RasterizerParams->DepthBias = 0;
		gGraphics->RasterizerParams->SilhouetteOffset = 0;
		gGraphics->Invalidate(ShaderParam::RasterizerParams);
	}

	// 
	{
		m_currentMask = 0xffffffff; // TODO
		m_currentLayer = _camera->GetLayer();
		m_frustumCallback.volume.FromCameraMatrices(gGraphics->Camera->ViewMat, gGraphics->Camera->ProjMat);

		Frustum _frustum;
		_frustum.FromCameraMatrices(gGraphics->Camera->ViewMat, gGraphics->Camera->ProjMat);

		m_models.View()->frustum = _frustum;
		m_2D.View()->frustum = _frustum;

#ifdef _DEBUG_GRAPHICS
		m_debugGeometry.View()->frustum = _frustum;
#endif
	}

	// prepare
	{
		gThreadPool->AddTask(&m_asyncPrepareModels);
		gThreadPool->AddTask(&m_asyncPrepare2D);
	}



	// TODO: ...


	{
		gGraphics->RasterizerParams->DepthBias = 0;
		gGraphics->RasterizerParams->SilhouetteOffset = 0;
		gGraphics->Invalidate(ShaderParam::RasterizerParams);
	}				
	
	gGraphics->SetDepthState(DepthState::Enabled);
	gGraphics->SetBlendState(BlendState::Default);

	gGraphics->SetBlendState(BlendState::Default); // temp

	gThreadPool->WaitTask(&m_asyncPrepareModels);
	_DrawBatch(&m_models, 0);

	gThreadPool->WaitTask(&m_asyncPrepare2D);

	// temp
	// TODO: bind default samplers
	//_DrawModels();

	//Begin2D();
	/*gGraphics->RasterizerParams->DepthBias = 0.001f;
	gGraphics->Invalidate(ShaderParam::RasterizerParams);
	gGraphics->SetBlendState(BlendState::Translucent);
	gGraphics->SetDepthState(DepthState::Enabled);
	_DrawBatch(&m_2DOverlay, 0);*/
}
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
void Renderer::_DrawBatch(RenderBatch* _batch, uint _subset)
{
	Shader::ID _shaders[Shader::__NumTypes];
	for (RenderBatch::Item& item : _batch->Items((Material::RenderMode)_subset))
	{
		// select shaders
		switch (item.op->type)
		{
			 
		case RenderOp::StaticModel:
			_shaders[Shader::Vertex] = Shader::VS_StaticModel;
			_shaders[Shader::Geometry] = Shader::GS_None;
			break;

		case RenderOp::SkinnedModel:
			_shaders[Shader::Vertex] = Shader::VS_StaticModel;
			_shaders[Shader::Geometry] = Shader::GS_None;
			break;

		case RenderOp::Sprite:
			_shaders[Shader::Vertex] = Shader::VS_Sprite;
			_shaders[Shader::Geometry] = Shader::GS_Sprite;
			break;

		case RenderOp::Billboard:
			_shaders[Shader::Vertex] = Shader::VS_Sprite;
			_shaders[Shader::Geometry] = Shader::GS_Billboard;
			break;

		case RenderOp::BillboardY:
			_shaders[Shader::Vertex] = Shader::VS_Sprite;
			_shaders[Shader::Geometry] = Shader::GS_BillboardY;
			break;

		case RenderOp::Terrain:
			_shaders[Shader::Vertex] = Shader::VS_Terrain;
			_shaders[Shader::Geometry] = Shader::GS_None;
			break;
		}

		// setup material
		_shaders[Shader::Pixel] = Shader::PS_Default; // TODO

		if (item.op->texture) // simple material
		{
			_shaders[Shader::Pixel] = Shader::PS_Texture; // TODO
			gGraphics->SetTexture(0, item.op->texture);
			gGraphics->SetSampler(0, Sampler::TrilinearRepeat); // TODO
		}
		else if(item.op->material)
		{
			for (uint i = 0; i < MaterialTexture::__Count; ++i)
			{
				gGraphics->SetTexture(i, item.op->material->texture[i]);
				gGraphics->SetSampler(i, Sampler::TrilinearClamp); // TODO
			}
		}

		// bind shaders
		for (uint i = 0; i < Shader::__NumTypes; ++i)
			gGraphics->SetShader(_shaders[i]);

		// bind buffers
		gGraphics->SetInstanceData(_batch->InstanceBuffer());
		gGraphics->SetVertexBuffer(item.op->vertexBuffer);
		gGraphics->SetIndexBuffer(item.op->indexBuffer);
		//TODO: set skeleton


		if (item.op->indexBuffer)
		{
			gGraphics->DrawIndexed(item.op->topology, item.op->firstVertex, item.op->firstIndex, item.op->count, item.numInstances, item.baseInstance);
		}
		else
		{
			gGraphics->Draw(item.op->topology, item.op->firstVertex, item.op->count, item.numInstances, item.baseInstance);
		}
	}
}


bool Renderer::_AddObject(void* _object, const AlignedBox& _bbox, bool _withTest, void* _userData)
{
	Renderable* _e = reinterpret_cast<Renderable*>(_object);
	RenderBatch* _batch = reinterpret_cast<RenderBatch*>(_userData);

	if (_e->IsEnabled() && _e->HasRenderTag(_batch->View()->tag) && _e->HasLayer(_batch->View()->layer))
	{
		_batch->AddObject(_e);
	}

	return true;
}

void Renderer::_PrepareBatch(RenderBatch* _batch)
{
	DbvtFrustumCallback _callback;
	_callback.volume = _batch->View()->frustum;
	_callback.callback = { this, &Renderer::_AddObject };
	_callback.userData = _batch;

	_batch->Clear();
	m_dbvt.Enum(&_callback);

	_batch->Build();
}

void Renderer::_PrepareModels(void)
{
	m_models.View()->tag = RenderTag::Model;
	m_models.View()->layer = m_currentLayer;
	_PrepareBatch(&m_models);
}

void Renderer::_Prepare2D(void)
{
	m_2D.View()->tag = RenderTag::Flat;
	m_2D.View()->layer = m_currentLayer;
	_PrepareBatch(&m_2D);
}

void Renderer::_Prepare2DOverlay(void)
{
	m_camera2DOverlay.fov = 0; // ortho
	m_camera2DOverlay.nearClip = 0;
	m_camera2DOverlay.farClip = 1;
	m_camera2DOverlay.zoom = 1;
	m_camera2DOverlay.useRelativeSize = true;
	m_camera2DOverlay.viewport = Rect::Identity;
	m_camera2DOverlay.depthC = 1;
	m_camera2DOverlay.Update(Matrix44::Identity, gDevice->Size());

	m_2DOverlay.View()->frustum = m_camera2DOverlay.frustum;
	m_2DOverlay.View()->tag = RenderTag::FlatOverlay;
	m_2DOverlay.View()->layer = -1;

	_PrepareBatch(&m_2DOverlay);
}
void Renderer::_Draw2DOverlay(void)
{
	gThreadPool->WaitTask(&m_asyncPrepare2DOverlay);


	gGraphics->RasterizerParams->DepthBias = 0.001f;
	gGraphics->Invalidate(ShaderParam::RasterizerParams);
	gGraphics->SetBlendState(BlendState::Translucent);
	gGraphics->SetDepthState(DepthState::Enabled);


	m_camera2DOverlay.Export(gGraphics->Camera);
	gGraphics->Invalidate(ShaderParam::Camera);

	_DrawBatch(&m_2DOverlay, 0);
}


#ifdef _DEBUG_GRAPHICS
//----------------------------------------------------------------------------//
void Renderer::DrawDebugLine(const Vector3& _start, const Vector3& _end, const Vector4& _color)
{
	if (!m_currentDebugRop || m_currentDebugRop->topology != PrimitiveType::Lines)
	{
		m_currentDebugRop = m_debugGeometry.AddRenderOp();
		m_currentDebugRop->firstVertex = m_debugGeometry.TempVertices()->ElementsCount<Vertex>();
		//m_currentDebugRop->firstIndex = m_debugGeometry.TempIndices()->ElementsCount<uint16>();
		m_currentDebugRop->topology = PrimitiveType::Lines;
		m_currentDebugRop->type = RenderOp::StaticModel;
		m_currentDebugRop->matrix = &Matrix44::Identity;
		m_currentDebugRop->instanceParams = &Vector4::Zero;
		m_currentDebugRop->vertexBuffer = m_debugGeometry.TempVertices();
		//m_currentDebugRop->indexBuffer = m_debugGeometry.TempIndices();
	}

	Vertex _v;
	_v.color = _color;
	//_v.SetNormal(Vector3::Zero);
	_v.position = _start;
	m_debugGeometry.TempVertices()->AppendElement(_v);
	_v.position = _end;
	m_debugGeometry.TempVertices()->AppendElement(_v);
	m_currentDebugRop->count += 2;
}
//----------------------------------------------------------------------------//
void Renderer::_DebugDraw(void)
{
	m_debugGeometry.Clear();
	m_currentDebugRop = nullptr;
	// if(m_enableDebugGeometry) return;

	SendEvent(RendererEvent::DebugDraw, &m_debugGeometry);
	
	m_currentDebugRop = nullptr;

	//_DrawDebugGeometry(false, true);

	m_debugGeometry.Build();

	gGraphics->RasterizerParams->DepthBias = 1e-3f;
	gGraphics->RasterizerParams->SilhouetteOffset = 0;
	gGraphics->Invalidate(ShaderParam::RasterizerParams);
	gGraphics->SetDepthState(DepthState::Enabled);
	gGraphics->SetBlendState(BlendState::Default);

	_DrawBatch(&m_debugGeometry, 0);
}
//----------------------------------------------------------------------------//
void Renderer::_DrawDebugGeometry(bool _drawDbvt, bool _drawBBox)
{
	Vertex _v[8];
	uint _baseVertex = 0xffff;
	RenderOp* _op = nullptr;
	Vector4 _color(1, 1, 1, 1);
	int _depth = 1;

	Dbvt::Node* _stackBase[64];
	Dbvt::Node** _stack = _stackBase;
	*_stack++ = m_dbvt.Root();

	if (m_dbvt.Root()) do
	{
		Dbvt::Node* _node = *--_stack;
		_depth -= 1;

		if (!m_debugGeometry.View()->frustum.Intersects(_node->box))
			continue;

		if (_node->child[1])
		{
			*_stack++ = _node->child0;
			*_stack++ = _node->child1;
			_depth += 2;
		}

		if (_drawDbvt && _node->child[1])
		{
			for (uint i = 0; i < 8; ++i)
				_v[i].SetColor(_color * (1 - 1 / Log((float)_depth + 1)));
		}
		else if (_drawBBox && !_node->child[1])
		{
			for (uint i = 0; i < 8; ++i)
				_v[i].SetColor(_color);
		}
		else
			continue;

		_node->box.GetAllCorners(_v, sizeof(Vertex), offsetof(Vertex, position));
		uint _base = m_debugGeometry.TempVertices()->AppendElements(_v, 8);
		_baseVertex += 8;
		if (_baseVertex >= 0xffff)
		{
			_op = m_debugGeometry.AddRenderOp();
			_op->firstVertex = _base;
			_op->firstIndex = m_debugGeometry.TempIndices()->ElementsCount<uint16>();
			_op->topology = PrimitiveType::Lines;
			_op->type = RenderOp::StaticModel;
			_op->matrix = &Matrix44::Identity;
			_op->instanceParams = &Vector4::Zero;
			_op->vertexBuffer = m_debugGeometry.TempVertices();
			_op->indexBuffer = m_debugGeometry.TempIndices();
			_base = 0;
			_baseVertex = 0;
		}
		m_debugGeometry.TempIndices()->AppendElements(Box::LineIndices, 24, _baseVertex);
		_op->count += 24;

	} while (_stack > _stackBase);
}
#endif

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
