#pragma once

#include "Core.hpp"
#include "Math.hpp"
#include "Thread.hpp"
#include <Windows.h>

//----------------------------------------------------------------------------//
// LOG file
//----------------------------------------------------------------------------//

#if defined(_LOG) || defined(_FATAL)
HANDLE g_log = nullptr;
void _WriteLog(const char* _msg, int _length = -1)
{
	if (!g_log)
	{
		//g_log = CreateFile(String(APP_NAME) + ".log", GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	}
	if (g_log)
	{
		_length = String::Length(_msg, _length);
		DWORD _written = 0;
		//WriteFile(g_log, _msg, _length, &_written, nullptr);
		//FlushFileBuffers(g_log);
	}
}
#endif

//----------------------------------------------------------------------------//
// Fatal
//----------------------------------------------------------------------------//

#ifdef _FATAL
void Fatal(const char* _msg)
{
	_WriteLog("FATAL ERROR:\n");
	_WriteLog(_msg);

#ifdef _DEBUG
	printf("FATAL ERROR:\n");
	printf(_msg);
	printf("\n");

	if(IsDebuggerPresent())
		DebugBreak();
	else
		system("pause");
#else
	MessageBoxA(0, _msg, "Fatal error", MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_TOPMOST);
#endif
	ExitProcess(-1);
}
#endif

//----------------------------------------------------------------------------//
// Log
//----------------------------------------------------------------------------//

#ifdef _LOG
SpinLock g_logMutex;
void LogMessage(const char* _msg, ...)
{
	SCOPE_LOCK(g_logMutex);

	String _str;
	for (int i = 0; i < LogNode::s_depth; ++i)
		_str.Append('\t');
	va_list _args;
	va_start(_args, _msg);
	_str += String::FormatV(_msg, _args) + "\n";
	va_end(_args);

#ifdef _DEBUG
	printf("%s", *_str);
#endif
	_WriteLog(_str, _str.Length());
}
#endif

//----------------------------------------------------------------------------//
// LogNode
//----------------------------------------------------------------------------//

#ifdef _LOG
int LogNode::s_depth = 0;

//----------------------------------------------------------------------------//
LogNode::LogNode(const char* _func)
{ 
	LOG(_func); 
	LOG("{"); 
	++s_depth;
}
//----------------------------------------------------------------------------//
LogNode::~LogNode(void)
{ 
	--s_depth;
	LOG("}");
}
//----------------------------------------------------------------------------//
#endif

//----------------------------------------------------------------------------//
// CRT
//----------------------------------------------------------------------------//

#ifndef _DEBUG

extern "C" const int _fltused = 1;

//----------------------------------------------------------------------------//
/*extern "C" __declspec(naked) void __cdecl _chkstk()
{
	// code from chkstk.asm
#define _PAGESIZE_ 4096

	__asm push    ecx;

	// Calculate new TOS.

	__asm lea ecx, [esp] + 8 - 4; // TOS before entering function + size for ret value
	__asm sub ecx, eax; new TOS

	// Handle allocation size that results in wraparound.
	// Wraparound will result in StackOverflow exception.

	__asm sbb eax, eax; // 0 if CF == 0, ~0 if CF == 1
	__asm not eax; // ~0 if TOS did not wrapped around, 0 otherwise
	__asm and ecx, eax; // set to 0 if wraparound

	__asm mov eax, esp; // current TOS
	__asm and eax, not (_PAGESIZE_ - 1); // Round down to current page boundary

cs10:
	__asm cmp ecx, eax; // Is new TOS
	__asm bnd jb short cs20; // in probed page ?
	__asm mov eax, ecx; // yes.
	__asm pop ecx
	__asm xchg esp, eax; // update esp
	__asm mov eax, dword ptr[eax]; // get return address
	__asm mov dword ptr[esp], eax; // and put it at new TOS
	__asm bnd ret;

	// ; Find next lower page and probe
cs20:
	__asm sub eax, _PAGESIZE_; // decrease by PAGESIZE
	__asm test dword ptr[eax], eax; // probe page.
	__asm jmp short cs10
} */
//----------------------------------------------------------------------------//
/*extern "C" int __cdecl _ftol2(float _val)
{
	int _r;
	__asm fistp _r
	return _r;
}
//----------------------------------------------------------------------------//
extern "C" int __cdecl _ftol2_sse(float _val)
{
	int _r;
	__asm fistp _r
	return _r;
}*/
//----------------------------------------------------------------------------//
#pragma function(memset)
#pragma optimize("", off)
void* __cdecl memset(void* _dst, int _val, size_t _size)
{
	for (char* p = (char*)_dst; _size--;)
		*p++ = (char)_val;
	return _dst;
}
#pragma optimize("", on)
//----------------------------------------------------------------------------//
#pragma function(memcpy)
#pragma optimize("", off)
void* memcpy(void* _dst, const void* _src, size_t _size)
{
	for (char *p = (char*)_dst, *s = (char*)_src; _size--;)
		*p++ = *s++;
	return _dst;
}
#pragma optimize("", on)
//----------------------------------------------------------------------------//
#ifndef _EDITOR
#pragma warning(push, 1)
//#pragma warning(disable: 4273) // Inconsistent DLL linkage
extern "C" void* __cdecl malloc(size_t _size)
{
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, _size);
}
//----------------------------------------------------------------------------//
extern "C" void __cdecl free(void* _p)
{
	HeapFree(GetProcessHeap(), 0, _p);
}
#pragma warning(pop)
#endif
//----------------------------------------------------------------------------//
void*  operator new (size_t _size)
{
	return malloc(_size);
}
//----------------------------------------------------------------------------//
void*  operator new [](size_t _size)
{
	return malloc(_size);
}
//----------------------------------------------------------------------------//
void  operator delete (void* _p)
{
	if (_p)
		free(_p);
}
//----------------------------------------------------------------------------//
void  operator delete [](void* _p)
{
	if (_p)
		free(_p);
}
//----------------------------------------------------------------------------//
void  operator delete (void* _p, size_t _size)
{
	if (_p)
		free(_p);
}
//----------------------------------------------------------------------------//
void operator delete [](void* _p, size_t _size)
{
	if (_p)
		free(_p);
}
//----------------------------------------------------------------------------//

#endif // _DEBUG

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// String
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
String& String::operator = (const String& _rhs)
{
	if (m_data != _rhs.m_data)
		Clear().Append(_rhs);
	return *this;
}
//----------------------------------------------------------------------------//
String& String::operator = (String&& _rhs)
{
	Swap(m_length, _rhs.m_length);
	Swap(m_capacity, _rhs.m_capacity);
	Swap(m_data, _rhs.m_data);
	return *this;
}
//----------------------------------------------------------------------------//
String& String::Clear(void)
{
	if (m_length)
	{
		m_length = 0;
		m_data[0] = 0;
	}
	return *this;
}
//----------------------------------------------------------------------------//
String& String::Reserve(uint _maxLength)
{
	if (m_capacity < _maxLength)
	{
		_maxLength = GrowTo(m_capacity, _maxLength) | 15;
		char* _newData = Allocate<char>(_maxLength + 1);
		memcpy(_newData, m_data, m_length);
		_newData[m_length] = 0;
		Deallocate(m_data);
		m_data = _newData;
		m_capacity = _maxLength;
	}
	return *this;
}
//----------------------------------------------------------------------------//
String& String::Resize(uint _newLength, char _ch)
{
	if (_newLength > m_length)
	{
		Reserve(_newLength);
		uint _append = _newLength - m_length;
		memset(m_data + m_length, _ch, _newLength - m_length);
	}
	m_length = _newLength;
	m_data[_newLength] = 0;
	return *this;
}
//----------------------------------------------------------------------------//
String& String::Append(const char* _str, int _length)
{
	_length = Length(_str, _length);
	if (_length)
	{
		uint _newLength = m_length + _length;
		Reserve(_newLength);
		memcpy(m_data + m_length, _str, _length);
		m_length = _newLength;
		m_data[m_length] = 0;
	}
	return *this;
}
//----------------------------------------------------------------------------//
int String::Compare(const char* _lhs, const char* _rhs)
{
	if (!_lhs)
		_lhs = "";
	if (!_rhs)
		_rhs = "";

	while (*_lhs && *_lhs++ == *_rhs++);

	return *_lhs - *_rhs;
}
//----------------------------------------------------------------------------//
uint String::Length(const char* _str, int _length)
{
	if (!_str)
		_str = "";

	if (_length < 0)
	{
		_length = 0;
		while (*_str++)
			_length++;
	}

	return (uint)_length;
}
//----------------------------------------------------------------------------//
uint String::Hash(const char* _str, uint _hash)
{
	if (!_str)
		_str = "";
	while (*_str)
		_hash = ((_hash >> 1) + ((_hash & 1) << 15) + Lower(*_str++)) & 0xffff;
	return _hash;
}
//----------------------------------------------------------------------------//
char* String::FromInt(char* _dst, int _val, uint _digits)
{
	//+4294967295
	char _buff[16];
	uint _written = 0;
	bool _neg = _val < 0;
	uint _x = _neg ? -_val : _val;
	do
	{
		_buff[_written++] = '0' + _x % 10;
		_x = _x / 10;

	} while (_x);

	if (_digits > 10)
		_digits = 10;

	while (_written < _digits)
		_buff[_written++] = '0';

	if (!_digits)
		_digits = _written;

	if (_neg)
		_buff[_written++] = '-', ++_digits;

	for (char* _src = _buff + _written; _digits--;)
		*_dst++ = *--_src;
	*_dst = 0;

	return _dst;
}
//----------------------------------------------------------------------------//
#include <math.h>
char* String::FromFloat(char* _dst, float _val, uint _digits)
{
	int _int = (int)_val;
	float _flt = _val - (float)_int;

	if (_val < 0 && !_int)
		*_dst++ = '-';

	_dst = FromInt(_dst, _int, 0);

	if (_digits)
	{
		const char* _pt = _dst;
		*_dst++ = '.';
		_dst = FromInt(_dst, (uint)Floor(Abs(_flt * Pow(10, (float)_digits)) + .5f), _digits);
		while(_dst-- > _pt)
		{
			if (*_dst == '0' || *_dst == '.')
				*_dst = 0;
			else
				break;
		}
	}

	return _dst;
}
//----------------------------------------------------------------------------//
String String::PrintSize(uint _size, float _div, const char* _prefix, const char* _suffix)
{
	if (_size < 1000)
		return FromInt(_size).Append(_prefix).Append(_suffix);
	if (_size < 1000000)
		return FromFloat(_size / _div, 1).Append(_prefix).Append("K").Append(_suffix);
	if (_size < 1000000000)
		return FromFloat(_size / _div / _div, 2).Append(_prefix).Append("M").Append(_suffix);

	return FromFloat(_size / _div / _div / _div, 2).Append(_prefix).Append("G").Append(_suffix);
}
//----------------------------------------------------------------------------//
String String::Format(const char* _fmt, ...)
{
	va_list _args;
	va_start(_args, _fmt);
	String _str = FormatV(_fmt, _args);
	va_end(_args);
	return _str;
}
//----------------------------------------------------------------------------//
int(_cdecl*_vsnprintf_pfn)(char* _dst, size_t _size, const char* _fmt, va_list _args) = nullptr;

String String::FormatV(const char* _fmt, va_list _args)
{
	if (!_vsnprintf_pfn)
	{
		HMODULE _h = LoadLibraryA("msvcrt.dll");
		_vsnprintf_pfn = reinterpret_cast<decltype(_vsnprintf_pfn)>(GetProcAddress(_h, "_vsnprintf"));
		CHECK(_vsnprintf_pfn != nullptr, "_vsnprintf not found");
		//_vsnprintf_pfn = &__vsnprintf;
	} 
	char _buff[4096];
	int _r = _vsnprintf_pfn(_buff, sizeof(_buff), _fmt, _args);
	return _buff;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Allocator utilities
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
uint GrowTo(uint _currentSize, uint _newSize)
{
	if (_currentSize)
	{
		uint _size = _currentSize;
		while (_size < _newSize)
			_size += (_size + 1) >> 1;
		return _size;
	}
	return _newSize;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// List 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void** _LL_Field(void* _ptr, size_t _offset)
{
	return (void**)((uint8*)_ptr + _offset);
}
//----------------------------------------------------------------------------//
void _LL_Link(void* _head, void* _this, void* _node)
{
	/*
	next = head;
	if(next) next->prev = this;
	head = this;
	*/

	size_t _offset = (uint8*)_node - (uint8*)_this;
	void** _next = 1 + (void**)_node;
	*_next = *(void**)_head;
	if (*_next)
		_LL_Field(*_next, _offset)[0] = _this;
	*(void**)_head = _this;
}
//----------------------------------------------------------------------------//
void _LL_Unlink(void* _head, void* _this, void* _node)
{
	/*
	if (m_next) m_next->m_prev = m_prev;
	if (m_prev) m_prev->m_next = m_next;
	else if(head == this) head = m_next;
	*/

	size_t _offset = (uint8*)_node - (uint8*)_this;
	void** _prev = 0 + (void**)_node;
	void** _next = 1 + (void**)_node;

	if (*_next)
		_LL_Field(*_next, _offset)[0] = *_prev;
	if (*_prev)
		_LL_Field(*_prev, _offset)[1] = *_next;
	else if (*(void**)_head == _this)
		*(void**)_head = *_next;

	*_prev = nullptr;
	*_next = nullptr;
}
//----------------------------------------------------------------------------//
void _LL_LinkFirst(void* _first, void* _last, void* _this, void* _node)
{
	/*
	next = first;
	if(next) next->prev = this;
	first = this;
	if(!last) last = this;

	*/

	size_t _offset = (uint8*)_node - (uint8*)_this;
	void** _prev = 0 + (void**)_node;
	void** _next = 1 + (void**)_node;

	*_next = *(void**)_first;
	if (*_next)
		_LL_Field(*_next, _offset)[0] = _this;
	*(void**)_first = _this;
	if (!*(void**)_last)
		*(void**)_last = _this;
}
//----------------------------------------------------------------------------//
void _LL_LinkLast(void* _first, void* _last, void* _this, void* _node)
{
	/*
	prev = last;
	if(prev) prev->next = this;
	last = this;
	if(!first) first = this;

	*/

	size_t _offset = (uint8*)_node - (uint8*)_this;
	void** _prev = 0 + (void**)_node;
	void** _next = 1 + (void**)_node;

	*_prev = *(void**)_last;
	if (*_prev)
		_LL_Field(*_prev, _offset)[1] = _this;
	*(void**)_last = _this;
	if (!*(void**)_first)
		*(void**)_first = _this;
}
//----------------------------------------------------------------------------//
void _LL_Unlink(void* _first, void* _last, void* _this, void* _node)
{
	/*
	if(m_next) m_next->m_prev = m_prev;
	else last = m_prev;
	if (m_prev) m_prev->m_next = m_next;
	else first = m_next;
	*/

	size_t _offset = (uint8*)_node - (uint8*)_this;
	void** _prev = 0 + (void**)_node;
	void** _next = 1 + (void**)_node;

	if (*_next)
		_LL_Field(*_next, _offset)[0] = *_prev;
	else 
		*(void**)_last = *_prev;

	if (*_prev)
		_LL_Field(*_prev, _offset)[1] = *_next;
	else 
		*(void**)_first = *_next;

	*_prev = nullptr;
	*_next = nullptr;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// RefCounted
//----------------------------------------------------------------------------//

#ifdef _DEBUG_RC
int RefCounted::s_numObjects = 0;
#endif

//----------------------------------------------------------------------------//
RefCounted::RefCounted(void)
{
#ifdef _DEBUG_RC
	++s_numObjects;
#endif
}
//----------------------------------------------------------------------------//
RefCounted::~RefCounted(void)
{ 
	ASSERT(m_refCount == 0); 
#ifdef _DEBUG_RC
	--s_numObjects;
#endif
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Reflection
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Reflection::Reflection(void)
{
	LOG_NODE("Create Reflection");
}
//----------------------------------------------------------------------------//
Reflection::~Reflection(void)
{
	LOG_NODE("Destroy Reflection");
}
//----------------------------------------------------------------------------//
#ifdef _HAVE_TYPE_NAMES
TypeInfo* Reflection::GetOrCreateTypeInfo(const char* _name)
#else
Reflection::TypeInfo* Reflection::GetOrCreateTypeInfo(uint _type)
#endif
{
#ifdef _HAVE_TYPE_NAMES
	uint _type = String::Hash(_name);
#endif

	auto _iter = m_types.Find(_type);
	if (_iter != m_types.End())
		return &_iter->second;

	LOG("Register %s(0x%04x) typeinfo", _name, _type);

	auto& _typeInfo = m_types[_type];
	_typeInfo.type = _type;
#ifdef _HAVE_TYPE_NAMES
	_typeInfo.name = _name;
#endif

	return &_typeInfo;
}
//----------------------------------------------------------------------------//
TypeInfo* Reflection::GetTypeInfo(uint _type)
{
	auto _iter = m_types.Find(_type);
	if (_iter != m_types.End())
		return &_iter->second;
	return nullptr;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
