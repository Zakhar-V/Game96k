#pragma once

#include "Math.hpp"
#include "File.hpp"
#include "Graphics.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gImageManager ImageManager::Get()

#undef CreateFont

typedef SharedPtr<class Image> ImagePtr;

//----------------------------------------------------------------------------//
// Image
//----------------------------------------------------------------------------//

class Image	: public Object
{
public:

	Image(void);
	Image(const Image& _other);
	~Image(void);

	void Realloc(uint _width, uint _height, uint _channels = 4, bool _resetAtlas = true);
	void Clear(const Vector4& _color, const IntRect& _region);

	void ResetAtlas(void);
	static IntVector2 PlaceRects(IntRect* _srcRects, uint _num, uint _border = 0);

	uint Width(void) { return m_size.x; }
	uint Height(void) { return m_size.y; }
	float* Data(void) { return m_data; }

	ImagePtr Copy(void) { return new Image(*this); }

	float* GetPtr(uint _x, uint _y);
	void _Wrap(int& _x, int& _y, bool _wrap);

	Vector4 GetPixel(int _x, int _y, bool _wrap);
	void SetPixel(int _x, int _y, const Vector4& _pixel, bool _wrap);
	Vector4 GetPixelSmooth(int _x, int _y, bool _wrap);

	Vector4 Sample(float _x, float _y, bool _wrap, bool _linear);


	void ExportAtlas(Texture* _dst);
	void ExporImage(Texture* _dst, uint _layer = 0);
	TexturePtr Export2D(PixelFormat::Enum _format = PixelFormat::DXT5);

	void SaveBMP(File& _dst);

	void CreateFont(const char* _name, uint _height, float _width, bool _italic);

	void CreatePerlinNoise(uint _width, uint _height, float _scale, int _rseed, uint _numOctaves = 4);

	ImagePtr CreateNormalmapFromHeightmap(float _scale, bool _wrap);


	static void CreateBlurCoeff(float* _dst, uint _width, float _r);
	void _Blur(Image* _dst, float _x, float _y, uint _size, float _r, bool _wrap = true);
	void Blur(uint _width, uint _height, float _r = 1, bool _wrap = true);

protected:

	IntVector2 m_size = 0;
	uint m_channels = 0;
	Vector2 m_invSize = Vector2::Zero;
	float* m_data = nullptr;

	Array<IntRect> m_atlas;
};

//----------------------------------------------------------------------------//
// ImageProgram
//----------------------------------------------------------------------------//

struct ImageScript	// opcode
{
	enum Op
	{
		End,
		Font, // create font: {op, string name, int height, float width, bool italic} 
		PerlinNoise, // create perlin noize: {op, int width, int height, float scale, int seed, int num_octaves} 
		Blur, // blur: {op, int width, int height, float r, bool wrap}
		Temp, //  save as temp image: {op, int id}
		ExportImage, // save in cache: {op, int name}
		Export2D, // export to 2D texture: {op, int name, int format}	
	};

	constexpr ImageScript(void) : op(End) { }
	constexpr ImageScript(Op _op) : op(_op) { }
	constexpr ImageScript(int _iArg) : iArg(_iArg) { }
	constexpr ImageScript(uint _iArg) : iArg(_iArg) { }
	constexpr ImageScript(float _fArg) : fArg(_fArg) { }
	constexpr ImageScript(bool _bArg) : bArg(_bArg) { }
	constexpr ImageScript(const char* _sArg) : sArg(_sArg) { }

	union
	{
		Op op;
		int iArg;
		float fArg;
		bool bArg;
		const char* sArg;
	};

};

//----------------------------------------------------------------------------//
// ImageManager
//----------------------------------------------------------------------------//

class ImageManager : public System, public Singleton<ImageManager>
{
public:
	ImageManager(void);
	~ImageManager(void);

	Texture* GetTexture(uint _name);

	void ExecuteScript(const ImageScript* _prog);

protected:
	EventResult _OnEvent(int _type, void* _data) override;

	HashMap<uint, TexturePtr> m_textures;
	HashMap<uint, ImagePtr> m_images;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

