#pragma once

#include "Math.hpp"
#include "File.hpp"

//----------------------------------------------------------------------------//
// Image
//----------------------------------------------------------------------------//

class Image	: public NonCopyable
{
public:

	Image(uint _name = 0);
	~Image(void);

	static Image* GetAsset(uint _name);

protected:

	uint m_name = 0;
	uint m_width = 0;
	uint m_height = 0;
	uint m_channels = 0;
	Vector2 m_invSize = Vector2::Zero;
	float* m_data = nullptr;
	Image* m_prev = nullptr;
	Image* m_next = nullptr;
	static Image* s_images;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class ImageNode
{
public:


protected:

	ImageNode** m_inputs;
	ImageNode** m_outputs;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class ImageGenerator
{
public:

	enum Flags
	{
		F_Temp,
	};

	enum OutputType
	{
		OT_None,
		OT_Image,
		OT_Texture,
	};

	uint16 width;
	uint16 height;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class ImageLibrary
{
public:

protected:
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class ImageContext
{
public:

	//Array<Image*> m_temp;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class ImageScript
{
public:

	virtual void Load(File& _file) = 0;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

struct ImgGen_Font : public ImageScript
{
	uint16 width;
	uint16 height;
	char name[32];
	int8 size;
	uint8 flags;

	void Load(File& _file)
	{
		_file.Read(width).Read(height).Read(name).Read(size).Read(flags);
	}
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class ImageModifier
{
public:

	enum Type
	{

	};
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

