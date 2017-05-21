#pragma once

#include "File.hpp"
#include "Math.hpp"	 // for Min/Max 

//----------------------------------------------------------------------------//
// File
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
File::File(void) 
{ 
}
//----------------------------------------------------------------------------//
File::~File(void)
{
	if (m_ownData)
		delete[] m_data;
}
//----------------------------------------------------------------------------//
void File::SetData(void* _data, uint _size, bool _readOnly)
{
	if (m_ownData)
		delete[] m_data;

	m_data = (uint8*)_data;
	m_size = _size;
	m_reserved = !_readOnly ? _size : 0;
	m_ownData = !_readOnly;
	m_pos = 0;
}
//----------------------------------------------------------------------------//
bool File::Load(const char* _path)
{
	if (!_path)
		_path = m_name;

	HANDLE _file = CreateFile(_path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, nullptr);
	if (!_file)
	{
		LOG("Couldn't open file \"%s\"", _path);
		SetData(nullptr, 0, false);
		return false;
	}

	DWORD _size = 0;
	GetFileSize(_file, &_size);
	uint8* _data = new uint8[_size];
	SetData(_data, _size, false);

	DWORD _readed = 0;
	ReadFile(_file, _data, _size, &_readed, nullptr);

	if (_readed != _size)
	{
		LOG("incomplete reading from \"%s\" (size = %d, readed = %d)", _path, _size, _readed);
	}

	CloseHandle(_file);
	return true;
}
//----------------------------------------------------------------------------//
bool File::Save(const char* _path)
{
	if (!_path)
		_path = m_name;

	HANDLE _file = CreateFile(_path, GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (!_file)
	{
		LOG("Couldn't create file \"%s\"", _path);
		return false;
	}

	DWORD _written = 0;
	WriteFile(_file, m_data, m_size, &_written, nullptr);
	
	if (_written != m_size)
	{
		LOG("Incomplete writing from \"%s\" (size = %d, written = %d)", _path, m_size, _written);
	}

	FlushFileBuffers(_file);
	CloseHandle(_file);

	return true;
}
//----------------------------------------------------------------------------//
File& File::Skip(uint _num)
{
	m_pos += Min(_num, m_size - m_pos);
	return *this;
}
//----------------------------------------------------------------------------//
File& File::SetPos(uint _pos)
{
	m_pos = Min(_pos, m_size);
	return *this;
}
//----------------------------------------------------------------------------//
File& File::Write(const void* _src, uint _size)
{
	if (m_ownData)
	{
		if (m_reserved - m_pos < _size)
			_Upsize(m_pos + _size);

		memcpy(m_data + m_pos, _src, _size);
		m_pos += _size;
		m_size = Max(m_size, m_pos);
	}
	return *this;
}
//----------------------------------------------------------------------------//
File& File::Write(const char* _src, int _length, bool _nullTerminated)
{
	_length = String::Length(_src, _length);
	Write((const void*)_src, _length);
	if (_nullTerminated)
	{
		uint8 _null = 0;
		Write(&_null, 1);
	}
	return *this;
}
//----------------------------------------------------------------------------//
File& File::Read(void* _dst, uint _size)
{
	_size = Min(_size, m_size - m_pos);

	//LOG("%d", *(uint16*)(m_data + m_pos));
	memcpy(_dst, m_data + m_pos, _size);

	//vertices
	//texcoords
	//colors
	//faces

	m_pos += _size;

	return *this;
}
//----------------------------------------------------------------------------//
File& File::Read(String& _dst)
{
	_dst = Read<String>();
	return *this;
}
//----------------------------------------------------------------------------//
File& File::Read(char* _dst)
{
	while (m_pos < m_size && m_data[m_pos])
		*_dst++ = m_data[m_pos++];

	*_dst = m_pos < m_size ? m_data[m_pos++] : 0;

	return *this;
}
//----------------------------------------------------------------------------//
void File::_Upsize(uint _size)
{
	ASSERT(m_ownData == true);

	uint _newSize = m_size ? (_size + (_size >> 1)) : _size;
	uint8* _newData = new uint8[_newSize];

	memcpy(_newData, m_data, m_size);
	delete[] m_data;
	m_data = _newData;
	m_reserved = _newSize;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
