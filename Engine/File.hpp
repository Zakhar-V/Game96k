#pragma once

#include "Core.hpp"

//----------------------------------------------------------------------------//
// File
//----------------------------------------------------------------------------//

class File : public NonCopyable
{
public:
	File(void);
	~File(void);

	void SetName(const String& _name) { m_name = _name; }
	const String& GetName(void) { return m_name; }

	void SetData(void* _data, uint _size, bool _readOnly);
	void Clear(void) { SetData(nullptr, 0, false); }

	bool Load(const char* _path = nullptr);
	bool Save(const char* _path = nullptr);

	bool Eof(void) { return m_pos == m_size; }
	File& Skip(uint _num);
	uint Pos(void) { return m_pos; }
	File& SetPos(uint _pos);

	File& Write(const void* _src, uint _size);
	File& Write(const char* _src, int _length = -1, bool _nullTerminated = false);

	template <class T> File& Write(const T& _src)
	{
		return Write(&_src, sizeof(T));
	}


	File& Read(void* _dst, uint _size);
	File& Read(String& _dst);
	File& Read(char* _dst);
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

protected:
	void _Upsize(uint _size);

	uint8* m_data = nullptr;
	uint m_size = 0;
	uint m_pos = 0;
	uint m_reserved = 0;
	bool m_ownData = true;
	String m_name;
};

//----------------------------------------------------------------------------//
// BitStream
//----------------------------------------------------------------------------//

template <class T> struct BitStream
{
	BitStream(T* _stream = nullptr) : stream(_stream) { }

	uint Read(uint _bits)
	{
		uint _value = 0;
		while (_bits--)
		{
			if (offset == 0)
			{
				current = 0;
				stream->Read(&current, 1);
			}
			_value |= ((current >> (7 - offset)) & 0x1) << _bits;
			offset++;

			if (offset == 8)
				offset = 0;
		}
		return _value;
	}

	void Write(uint _value, uint _bits)
	{
		while (_bits--)
		{
			uint _bit = ((_value >> _bits) & 0x1);
			current |= _bit << (7 - offset);
			offset++;
			if (offset == 8)
			{
				stream->Write(&current, 1);
				current = 0;
				offset = 0;
			}
		}
	}

	void WriteLastByte(void)
	{
		if (offset > 0)
		{
			stream->Write(&current, 1);
			current = 0;
			offset = 0;
		}
	}

	T* stream = nullptr;
	uint8 current = 0;
	uint offset = 0;
};

//----------------------------------------------------------------------------//
// FileSystem
//----------------------------------------------------------------------------//

class FileSystem : public Singleton<FileSystem>
{

};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
