#pragma once

#include "Core.hpp"

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
#include "Math.hpp"

class File : public NonCopyable
{

public:

	File(void) { }
	~File(void)
	{
		if (m_ownData)
			delete[] m_data;
	}

	void SetData(void* _data, uint _size, bool _readOnly)
	{
		if (m_ownData)
			delete[] m_data;

		m_data = (uint8*)_data;
		m_size = _size;
		m_reserved = !_readOnly ? _size : 0;
		m_ownData = !_readOnly;
		m_pos = 0;
	}

	bool Eof(void) { return m_pos == m_size; }

	File& Read(void* _dst, uint _size)
	{
		_size = Min(_size, m_size - m_pos);

		memcpy(_dst, m_data + m_pos, _size);
		m_pos += _size;

		return *this;
	}

	File& Write(const void* _src, uint _size)
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

	template <class T> T Read(void)
	{
		T _val = static_cast<T>(0);
		Read(&_val, sizeof(T));
		return _val;
	}

	template <class T> File& Read(T& _dst)
	{
		return Read(&_dst, sizeof(T)) ;
	}

	template <> String Read(void)
	{
		const char* _start = (const char*)(m_data + m_pos);
		while (m_pos < m_size && m_data[m_pos])
			++m_pos;
		if (m_pos < m_size)
			++m_pos;

		return String(_start, (const char*)(m_data + m_pos));
	}
	File& Read(String& _dst)
	{
		_dst = Read<String>();
		return *this;
	}

	File& Read(char* _dst)
	{
		while (m_pos < m_size && m_data[m_pos])
			*_dst++ = m_data[m_pos++];

		*_dst = m_pos < m_size ? m_data[m_pos++] : 0;

		return *this;
	}

protected:
	void _Upsize(uint _size)
	{
		ASSERT(m_ownData == true);

		uint _newSize = m_size ? (_size + (_size >> 1)) : _size;
		uint8* _newData = new uint8[_newSize];
		
		memcpy(_newData, m_data, m_size);
		delete[] m_data;
		m_data = _newData;
		m_reserved = _newSize;
	}

	uint8* m_data = nullptr;
	uint m_size = 0;
	uint m_pos = 0;
	uint m_reserved = 0;
	bool m_ownData = true;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

class FileSystem : public Singleton<FileSystem>
{

};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
