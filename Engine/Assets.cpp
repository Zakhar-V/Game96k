#pragma once

#include "Assets.hpp"

//----------------------------------------------------------------------------//
// Data
//----------------------------------------------------------------------------//

#ifdef EDITOR
#include "../Data/EditorData.inl"
#else
#include "../Data/GameData.inl"
#endif

//----------------------------------------------------------------------------//
// LZ4 decoder
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void LZ4Decompress(const void* _src, void* _dst, size_t _srcSize, size_t _dstSize)
{
	// specification: http://fastcompression.blogspot.in/2011/05/lz4-explained.html

	if (/*!_src ||*/ !_srcSize)	// empty source
		return;

	const uint8*_ref;
	const uint8* _srcp = (const uint8*)_src;
	const uint8* _srce = _srcp + _srcSize;
	const uint8* _srce6 = _srce - 6; // end - (lastliterals + marker)
	const uint8* _srce8 = _srce - 8; // end - (offset + lastliterals + marker)
	uint8* _dstp = (uint8*)_dst;
	const uint8* _dste = _dstp + _dstSize;
	const uint8* _dste8 = _dste - 8; // end - (copylength)
	const uint8* _dste12 = _dste - 12; // end - (copylength + minmatch)
	uint8 _marker;
	size_t _length;

	for (;;)
	{
		// marker
		_marker = *_srcp++;
		_length = (_marker >> 4);

		// length
		if (_length == 0xf)
		{
			for (uint8 s = 0xff; _srcp < _srce && s == 0xff;)
				_length += (s = *_srcp++);
		}

		// literals
		if (_dstp + _length > _dste /*|| _srcp + _length > _srce*/)
			break; // out of range

		MemoryCopy(_dstp, _srcp, _length);
		_srcp += _length;
		_dstp += _length;

		if (_dstp > _dste8 /*|| _srcp > _srce8*/) // EoF or overflow destination buffer
			break;

		// offset
		_ref = _dstp - *(uint16*)_srcp;
		_srcp += 2;
		if (_ref > _dste) // overflow destination buffer
			break;

		// matchlength
		_length = _marker & 0xf;
		if (_length == 0xf)
		{
			for (uint8 s = 0xff; _srcp < _srce8 && s == 0xff;)
				_length += (s = *_srcp++);
		}

		// copy 
		if (_dstp - _ref < 4)
		{
			_dstp[0] = _ref[0];
			_dstp[1] = _ref[1];
			_dstp[2] = _ref[2];
			_dstp[3] = _ref[3];
			_dstp += 4;
		}
		else
			_length += 4;

		if (_dstp + _length > _dste)
			_length = _dste - _dstp; // EoF

		while (_length--)
			*_dstp++ = *_ref++;

		if (_dstp == _dste)
			break;
	}
	ASSERT(_srcp <= _srce);
	ASSERT(_dstp <= _dste);

	//return _srcp <= _srce ? (_dstp - (uint8*)_dst) : (_srcp - _srce);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Asset
//----------------------------------------------------------------------------//

struct Asset
{
	uint name; // 2 byte
	uint size; // 2 byte
	uint8* data;
	Asset* prev = nullptr;
	Asset* next = nullptr;
};

//----------------------------------------------------------------------------//
// Assets
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Assets::Assets(void)
{
	LOG_NODE("Unpack assets");

	uint16 _size = *(const uint16*)gPackedData;
	m_data = new uint8[_size];

	LOG("%d bytes x%.2f --> %d bytes", sizeof(gPackedData) - 2, sizeof(gPackedData) > 2 ? ((float)_size / (float)(sizeof(gPackedData) - 2)) : 1.f, _size);

	LZ4Decompress(gPackedData + 2, m_data, sizeof(gPackedData) - 2, _size);

	int _num = 0;
	for (uint8* p = m_data, *e = m_data + _size; p < e;)
	{
		Asset* _asset = new Asset;
		_asset->name = *(const uint16*)(p + 0);
		_asset->size = *(const uint16*)(p + 2);
		_asset->data = p + 4;
		p += _asset->size + 4;
		_num++;
		LOG("#%d 0x%04x at %d, %d bytes", _num, _asset->name, (int)(_asset->data - m_data), _asset->size);

		Link(m_assets, _asset, _asset->prev);
	}
}
//----------------------------------------------------------------------------//
Assets::~Assets(void)
{
	LOG_NODE("Delete assets");

	while (m_assets)
	{
		Asset* _asset = m_assets;
		Unlink(m_assets, _asset, _asset->prev);
		delete _asset;
	}
	delete[] m_data;
}
//----------------------------------------------------------------------------//
uint8* Assets::GetAsset(uint _name, uint& _size)
{
	for (Asset* i = m_assets; i; i = i->next)
	{
		if (i->name == _name)
		{
			_size = i->size;
			return i->data;
		}
	}
	return nullptr;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
