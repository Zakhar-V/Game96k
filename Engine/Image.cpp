#pragma once

#include "Image.hpp"

//----------------------------------------------------------------------------//
// Image
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Image::Image(void) 
{
	ResetAtlas();
}
//----------------------------------------------------------------------------//
Image::Image(const Image& _other) :
	m_size(_other.m_size), m_channels(_other.m_channels), m_invSize(_other.m_invSize), m_atlas(_other.m_atlas)
{
	uint _size = m_size.x * m_size.y * m_channels;
	m_data = Allocate<float>(_size);
	memcpy(m_data, _other.m_data, _size * sizeof(float));
}
//----------------------------------------------------------------------------//
Image::~Image(void)
{
	Deallocate(m_data);
}
//----------------------------------------------------------------------------//
void Image::Realloc(uint _width, uint _height, uint _channels, bool _resetAtlas)
{
	ASSERT(_channels > 0 && _channels <= 4);

	if (m_size.x != _width || m_size.y != _height || m_channels != _channels)
	{
		Deallocate(m_data);
		uint _size = _width * _height * _channels;
		m_data = Allocate<float>(_size);
		m_size.Set(_width, _height);
		m_channels = _channels;
		m_invSize = _size > 0 ? (1 / Vector2(m_size)) : 0;
	}
	if(_resetAtlas)
		ResetAtlas();
}
//----------------------------------------------------------------------------//
void Image::Clear(const Vector4& _color, const IntRect& _region)
{
	IntRect _r = IntRect(0, 0, m_size.x, m_size.y).Overlay(_region);
	for (int y = _r.top; y < _r.bottom; ++y)
	{
		int _base = y * m_size.x;
		for (int x = _r.left; x < _r.right; ++x)
		{
			float* _dst = m_data + (_base + x) * m_channels;
			for (uint i = 0; i < m_channels; ++i)
				_dst[i] = _color[i];
		}
	}
}
//----------------------------------------------------------------------------//
void Image::ResetAtlas(void)
{
	m_atlas.Clear().Push(IntRect(0, 0, m_size.x, m_size.y));
}
//----------------------------------------------------------------------------//
IntVector2 Image::PlaceRects(IntRect* _srcRects, uint _num, uint _border)
{
	Array<IntRect*> _rects;
	for (uint i = 0; i < _num; ++i)
	{
		IntRect* _r = _srcRects + i;
		ASSERT(_r->IsFinite());
		_rects.Push(_r);
		_r->right += _border;
		_r->bottom += _border;
	}

	IntVector2 _texSize = IntVector2::Zero;

	Sort(_rects.Begin(), _rects.End(), [](IntRect* _lhs, IntRect* _rhs)
	{
		if (_lhs->Height() == _rhs->Height())
			return _lhs->Width() > _rhs->Width();
		return _lhs->Height() > _rhs->Height();
	});

#if 0 // test of sorting
	int _height = 0;
	for (uint i = 0; i < _rects.Size(); ++i)
	{
		IntRect* _rect = _rects[i];
		_rect->SetOrigin(_texSize);
		_height = Max(_height, _rect->Height());
		_texSize.x = Max(_rect->left, _rect->right);
	}
	_texSize.y = _height;
#else
	if (_rects.Size() > 0) for (;;)
	{
		bool _placed = true;
		IntVector2 _pos = IntVector2::Zero;
		IntRect* _lineStart = _rects[0];
		int _maxWidth = 0;
		for (uint i = 0; i < _rects.Size(); ++i)
		{
			IntRect* _r = _rects[i];
			IntVector2 _size = _r->Size();
			if (_pos.x + _size.x > _texSize.x)
			{
				if (_pos.y + _size.y > _texSize.y)
				{
					if (_texSize.x < _texSize.y)
						_texSize.x += _size.x;
					else
						_texSize.y += _size.y;

					_placed = false;
				}
				_pos.x = 0;
				_pos.y += _lineStart->Height();
				_lineStart = _r;

				if (!_placed)
					break;
			}
			_r->SetOrigin(_pos);
			_pos.x += _r->Width();
			_maxWidth = Max(_maxWidth, _r->right);
		}

		if (_placed)
		{
			_texSize.x = _maxWidth;
			_texSize.y = _lineStart->bottom;
			break;
		}
	}
#endif

#if defined(_STATISTICS) && 1
	{
		float _area = 0;
		for (uint i = 0; i < _num; ++i)
			_area += _rects[i]->Width() * _rects[i]->Height();
		float _effectivity = (_area / (_texSize.x * _texSize.y)) * 100;
		LOG("Pack %d sprites, Size: %dx%d, Effectivity: %.2f%%", _rects.Size(), _texSize.x, _texSize.y, _effectivity);
	}
#endif

	for (uint i = 0; i < _num; ++i)
	{
		IntRect& _r = _srcRects[i];
		_r.right -= _border;
		_r.bottom -= _border;
	}

	return _texSize;
}
//----------------------------------------------------------------------------//
float* Image::GetPtr(uint _x, uint _y)
{
	ASSERT((int)_x < m_size.x);
	ASSERT((int)_y < m_size.y);
	return &m_data[(_x + _y * m_size.x) * m_channels];
}
//----------------------------------------------------------------------------//
void Image::_Wrap(int& _x, int& _y, bool _wrap)
{
	ASSERT(m_size.x > 0);
	ASSERT(m_size.y > 0);

	if (_wrap)
	{
		_x %= m_size.x;
		_y %= m_size.y;

		if (_x < 0)
			_x = m_size.x + _x;
		if (_y < 0)
			_y = m_size.y + _y;
	}
	else
	{
		_x = Clamp(_x, 0, m_size.x - 1);
		_y = Clamp(_y, 0, m_size.y - 1);
	}
}
//----------------------------------------------------------------------------//
Vector4 Image::GetPixel(int _x, int _y, bool _wrap)
{
	_Wrap(_x, _y, _wrap);

	Vector4 _r = Vector4::Zero;
	memcpy(_r.v, GetPtr(_x, _y), m_channels * sizeof(float));
	return _r;
}
//----------------------------------------------------------------------------//
void Image::SetPixel(int _x, int _y, const Vector4& _pixel, bool _wrap)
{
	_Wrap(_x, _y, _wrap);
	memcpy(GetPtr(_x, _y), _pixel.v, m_channels * sizeof(float));
}
//----------------------------------------------------------------------------//
Vector4 Image::GetPixelSmooth(int _x, int _y, bool _wrap)
{
	Vector4 _p = Vector4::Zero;
	for (int y = -1; y < 2; ++y)
	{
		for (int x = -1; x < 2; ++x)
			_p += GetPixel(x + _x, y + _y, _wrap);
	}
	_p *= 1.f / 9;
	return _p;
}
//----------------------------------------------------------------------------//
Vector4 Image::Sample(float _x, float _y, bool _wrap, bool _linear)
{
	if (_wrap)
	{
		_x -= Floor(_x);
		_y -= Floor(_y);
		if (_x < 0)
			_x = 1 + _x;
		if (_y < 0)
			_y = 1 + _y;
	}
	else
	{
		_x = Clamp01(_x);
		_y = Clamp01(_y);
	}

	_x *= m_invSize.x;
	_y *= m_invSize.y;

	Vector4 _r = Vector4::Zero;
	if (_linear)
	{
		float _fx = _x - Floor(_x);
		float _fy = _y - Floor(_y);
		float _x2 = (_x + 1) * m_invSize.x;
		float _y2 = (_y + 1) * m_invSize.y;
		_x *= m_invSize.x;
		_y *= m_invSize.y;

		_r = Mix(
			Mix(Sample(_x, _y, _wrap, false), Sample(_x2, _y, _wrap, false), _fx),
			Mix(Sample(_x, _y2, _wrap, false), Sample(_x2, _y2, _wrap, false), _fx),
			_fy);
	}
	else
	{
		memcpy(_r.v, GetPtr((uint)_x, (uint)_y), m_channels * sizeof(float));
	}

	return _r;
}
//----------------------------------------------------------------------------//
void Image::ExportAtlas(Texture* _dst)
{
	ASSERT(_dst != nullptr);
	_dst->Atlas().Clear();
	for (IntRect& i : m_atlas)
	{
		Rect _r;
		_r.left = i.left * m_invSize.x;
		_r.right = i.right * m_invSize.x;
		_r.top = i.top * m_invSize.y;
		_r.bottom = i.bottom * m_invSize.y;
		_dst->Atlas().Push(_r);
	}
}
//----------------------------------------------------------------------------//
void Image::ExporImage(Texture* _dst, uint _layer)
{
	ASSERT(_dst != nullptr);
	//ASSERT(_dst->IsInitialized() != nullptr);

	PixelFormat::Enum _srcFormat;
	switch (m_channels)
	{
	case 1:
		_srcFormat = PixelFormat::R32F;
		break;
	case 2:
		_srcFormat = PixelFormat::RG32F;
		break;
	case 3:
		_srcFormat = PixelFormat::RGB32F;
		break;
	default:
		_srcFormat = PixelFormat::RGBA32F;
		break;
	}

	_dst->Write(_srcFormat, 0, 0, _layer, m_size.x, m_size.y, 1, m_data);
}
//----------------------------------------------------------------------------//
TexturePtr Image::Export2D(PixelFormat::Enum _format)
{
	TexturePtr _tex = new Texture;
	_tex->Init(TextureType::Quad, _format);
	_tex->Realloc(m_size.x, m_size.y);
	ExporImage(_tex);
	ExportAtlas(_tex);
	_tex->GenerateMipmap();
	return _tex;
}
//----------------------------------------------------------------------------//
void Image::SaveBMP(File& _dst)
{
	BITMAPFILEHEADER _header;
	BITMAPINFOHEADER _info;

	memset(&_header, 0, sizeof(_header));
	_header.bfType = 0x4d42;
	_header.bfSize = m_size.x * m_size.y * 4 + sizeof(_header) + sizeof(_info);
	_header.bfOffBits = sizeof(_header) + sizeof(_info);

	memset(&_info, 0, sizeof(_info));
	_info.biSize = sizeof(_info);
	_info.biWidth = m_size.x;
	_info.biHeight = m_size.y;
	_info.biPlanes = 1;
	_info.biBitCount = 32;
	_info.biSizeImage = m_size.x * m_size.y * 32;

	_dst.Write(_header);
	_dst.Write(_info);

	for (int y = 0; y < m_size.y; ++y)
	{
		for (int x = 0; x < m_size.x; ++x)
		{
			PackedColor _color = GetPixel(x, m_size.y - (y + 1), false);
			_dst.Write(_color);
		}
	}
}
//----------------------------------------------------------------------------//
void Image::CreateFont(const char* _name, uint _height, float _width, bool _italic)
{
	int _tw = int(_width * 1000) / 100;
	HFONT _fnt = CreateFontA(-int(_height), 0, 0, 0, _tw * 100, _italic, 0, 0, RUSSIAN_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_TT_ALWAYS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _name);
	if (!_fnt)
	{
		LOG("Unable to create \"%s\" font", _name);
		CreateFont("Tahoma", _height, _width, _italic);
		return;
	}
	HDC _dc = CreateCompatibleDC(0);
	SetTextColor(_dc, RGB(255, 255, 255));
	SetBkColor(_dc, RGB(0, 0, 0));
	SetBkMode(_dc, TRANSPARENT);
	SetMapMode(_dc, MM_TEXT);
	SetTextAlign(_dc, TA_TOP);
	SelectObject(_dc, _fnt);

	ABC _abc[256];
	TEXTMETRICA	_tm;
	GetTextMetricsA(_dc, &_tm);
	GetCharABCWidthsA(_dc, 0, 0xff, _abc);
	m_atlas.Clear().Resize(256, IntRect::Zero);

	_height = _tm.tmHeight; //+ _tm.tmInternalLeading + _tm.tmExternalLeading;
	MAT2 _mat = { { 0, 1 },{ 0, 0 },{ 0, 0 },{ 0, 1 } };
	for (uint i = 0; i < m_atlas.Size(); ++i)
	{
		GLYPHMETRICS _metrics;
		GetGlyphOutlineA(_dc, i, GGO_METRICS, &_metrics, 0, nullptr, &_mat);
		m_atlas[i].SetSize(_metrics.gmCellIncX + _abc[i].abcA, _height);
	}
					
	IntVector2 _size = PlaceRects(m_atlas.Data(), m_atlas.Size(), 4);
	Realloc(_size.x, _size.y, 4, false);

	BITMAPINFO _bmi;
	memset(&_bmi, 0, sizeof(_bmi));
	_bmi.bmiHeader.biSize = sizeof(_bmi);
	_bmi.bmiHeader.biWidth = _size.x;
	_bmi.bmiHeader.biHeight = -_size.y;
	_bmi.bmiHeader.biBitCount = 32;
	_bmi.bmiHeader.biCompression = BI_RGB;
	_bmi.bmiHeader.biPlanes = 1;

	PackedColor* _bmp;
	HBITMAP _bm = CreateDIBSection(_dc, &_bmi, DIB_RGB_COLORS, (void**)&_bmp, 0, 0);

	SelectObject(_dc, _bm);
	for (uint i = 0; i < m_atlas.Size(); ++i)
	{
		uint8 c = (uint8)i;
		IntRect& _ch = m_atlas[i];
		TextOutA(_dc, _ch.left + _abc[i].abcA, _ch.top, (const char*)&c, 1);
	}
	GdiFlush();

	Vector4 _color = Vector4::One;
	for (int y = 0; y < m_size.y; ++y)
	{
		uint _base = y * m_size.x;
		for (int x = 0; x < m_size.x; ++x)
		{
			uint _offset = _base + x;
			float* _dst = m_data + _offset * m_channels;
			_color = Vector4::One;
			_color.a = ByteToFloat(_bmp[_offset].r);
			_color.r *= _color.a;
			_color.g *= _color.a;
			_color.b *= _color.a;
			for (uint i = 0; i < m_channels; ++i)
				_dst[i] = _color[i];
		}
	}

	DeleteObject(_bm);
	DeleteDC(_dc);
	DeleteObject(_fnt);
}
//----------------------------------------------------------------------------//
void Image::CreatePerlinNoise(uint _width, uint _height, float _scale, int _rseed, uint _numOctaves)
{
	Realloc(_width, _height, 1);

	for (int y = 0; y < m_size.y; ++y)
	{
		for (int x = 0; x < m_size.x; ++x)
		{
			m_data[x + y * m_size.x] = Perlin2d(x * m_invSize.x * _scale, y *  m_invSize.y * _scale, _numOctaves);
			//printf("%f\n", m_data[x + y * m_size.x]);
		}
	}
}
//----------------------------------------------------------------------------//
ImagePtr Image::CreateNormalmapFromHeightmap(float _scale, bool _wrap)
{
	ImagePtr _nmap = new Image;
	_nmap->Realloc(m_size.x, m_size.y, 3);

	// http://steps3d.narod.ru/tutorials/bump-maps-tutorial.html
	for (int y = 0; y < m_size.y; ++y)
	{
		for (int x = 0; x < m_size.x; ++x)
		{
			float _c = GetPixel(x, y, false)[0];
			float _cx = GetPixel(x + 1, y, false)[0];
			float _cy = GetPixel(x, y + 1, false)[0];

			float _dx = (_c - _cx) * _scale;
			float _dy = (_c - _cy) * _scale;

			float _invLen = RSqrt(_dx*_dx + _dy*_dy + 1);

			float* _dst = _nmap->GetPtr(x, y);
			_dst[0] = _dy * _invLen * .5f + .5f;
			_dst[1] = -_dx * _invLen * .5f + .5f;
			_dst[2] = _invLen * .5f + .5f;
		}
	}

	return _nmap;
}
//----------------------------------------------------------------------------//
void Image::CreateBlurCoeff(float* _dst, uint _width, float _r)
{
	float _i2rr = -1 / (2 * _r * _r);
	float _ip2rr = 1 / (PI * 2 * _r * _r);
	int _hw = _width >> 1;
	float _sum = 0;
	for (int x = 0; x < (int)_width; ++x)
	{
		_dst[x] = Exp(Sqr((float)_hw - x) * _i2rr) * _ip2rr;
		_sum += _dst[x];
	}

	_sum = 1 / _sum;
	for (int x = 0; x < (int)_width; ++x)
	{
		_dst[x] *= _sum;
		//printf("%f ", _dst[x]);
	}
	//printf("\n");
}
//----------------------------------------------------------------------------//
void Image::_Blur(Image* _dst, float _x, float _y, uint _size, float _r, bool _wrap)
{
	ASSERT(_dst && _dst != this);
	_dst->Realloc(m_size.x, m_size.y, m_channels, false);

	Array<float> _filter;
	_filter.Resize(_size);
	CreateBlurCoeff(_filter.Data(), _size, _r);

	int _hSize = _size >> 1;
	for (int y = 0; y < m_size.y; ++y)
	{
		for (int x = 0; x < m_size.x; ++x)
		{
			int _sx = x - _hSize;
			int _sy = y - _hSize;
			Vector4 _sum = Vector4::Zero;
			for (int i = 0; i < (int)_size; ++i)
			{
				_sum += GetPixel((int)(_sx + i * _x), (int)(_sy + i * _y), _wrap) *_filter[i];
			}
			_dst->SetPixel(x, y, _sum, _wrap);
		}
	}
}
//----------------------------------------------------------------------------//
void Image::Blur(uint _width, uint _height, float _r, bool _wrap)
{
	ImagePtr _copy = new Image;

	_Blur(_copy, 1, 0, _width, _r, _wrap);
	_copy->_Blur(this, 0, 1, _height, _r, _wrap);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// ImageManager
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
ImageManager::ImageManager(void)
{
	LOG_NODE("Create ImageManager");
}
//----------------------------------------------------------------------------//
ImageManager::~ImageManager(void)
{
	LOG_NODE("Destroy ImageManager");
}
//----------------------------------------------------------------------------//
EventResult ImageManager::_OnEvent(int _type, void* _data)
{
	return ER_Pass;
}
//----------------------------------------------------------------------------//
Texture* ImageManager::GetTexture(uint _name)
{
	auto _exists = m_textures.Find(_name);
	if (_exists != m_textures.End())
		return _exists->second;

	LOG("Texture 0x%04x not found", _name);
	return nullptr;
}
//----------------------------------------------------------------------------//
void ImageManager::ExecuteScript(const ImageScript* _prog)
{
	ImagePtr _img;
	HashMap<int, ImagePtr> _temp;

	while (_prog->op != ImageScript::End)
	{
		switch (_prog[0].op)
		{
		case ImageScript::Font:
		{
			LOG("CreateFont \"%s\", Height: %d, Width: %.1f, Italic: %d", _prog[1].sArg, _prog[2].iArg, _prog[3].fArg, _prog[4].bArg);
			
			_img = new Image;
			_img->CreateFont(_prog[1].sArg, _prog[2].iArg, _prog[3].fArg, _prog[4].bArg);
			_prog += 5;

		} break;

		case ImageScript::PerlinNoise:
		{
			LOG("CreatePerlinNoise %d x %d, Scale: %f, Seed: %d, %d Octaves", _prog[1].iArg, _prog[2].iArg, _prog[3].fArg, _prog[4].iArg, _prog[5].iArg);
		
			_img = new Image;
			_img->CreatePerlinNoise(_prog[1].iArg, _prog[2].iArg, _prog[3].fArg, _prog[4].iArg, _prog[5].iArg);
			_prog += 6;

		} break;

		case ImageScript::Blur:
		{
			LOG("Blur %d x %d, R: %f, Wrap: %d", _prog[1].iArg, _prog[2].iArg, _prog[3].fArg, _prog[4].bArg);
			ASSERT(_img != nullptr, "No image");
			
			_img->Blur(_prog[1].iArg, _prog[2].iArg, _prog[3].fArg, _prog[4].bArg);
			_prog += 5;

		} break;

		case ImageScript::Temp:
		{
			LOG("Temp %d", _prog[1].iArg);
			ASSERT(_img != nullptr, "No image");
			
			_temp[_prog[1].iArg] = _img;
			_prog += 2;

		} break;

		case ImageScript::ExportImage:
		{
			LOG("Export Image 0x%04x", _prog[1].iArg);
			DEBUG_CODE(if (m_images.Find(_prog[1].iArg) != m_images.End()) LOG("Image 0x%04x already exists", _prog[1].iArg));
			ASSERT(_img != nullptr, "No image");
			
			m_images[_prog[1].iArg] = _img;
			_prog += 2;

		} break;

		case ImageScript::Export2D:
		{
			LOG("Export 2D 0x%04x, Format: %d", _prog[1].iArg, _prog[2].iArg);
			DEBUG_CODE(if (m_textures.Find(_prog[1].iArg) != m_textures.End()) LOG("Texture 0x%04x already exists", _prog[1].iArg));
			ASSERT(_img != nullptr, "No image");

			m_textures[_prog[1].iArg] = _img->Export2D((PixelFormat::Enum)_prog[2].iArg);
			_prog += 3;

		} break;

		default:
		{
			LOG("Unknown command %d", _prog[0].iArg);
			CHECK(false, "Unknown command");
		} break;

		}
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
