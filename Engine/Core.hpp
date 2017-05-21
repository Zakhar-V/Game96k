#pragma once

//----------------------------------------------------------------------------//
// Common includes
//----------------------------------------------------------------------------//

#pragma warning(disable : 4577)	// noexcept
#include <new>
#include <initializer_list>
#include <stdint.h>
#include <stdarg.h>
#include <Windows.h>
#ifdef _DEBUG
#include <stdio.h>
#endif

//----------------------------------------------------------------------------//
// Preprocessor unitlities
//----------------------------------------------------------------------------//

#define _EMPTY()
#define _QUOTE( x ) #x
#define _QUOTE_IN_PLACE( x ) _QUOTE( x )
#define __FILELINE__ __FILE__"(" _QUOTE_IN_PLACE( __LINE__ ) ")"
#define DEPRECATED __declspec(deprecated)
#define _PRAGMA(x) __pragma(x)
#define _PRAGMA_MESSAGE(x) _PRAGMA(message(__FILELINE__ " : " x))
#define COMPILER_MESSAGE(_prefix, _message) _PRAGMA_MESSAGE(_prefix ": " _message )
#define COMPILER_MESSAGE_EX(_prefix, _source, _message) COMPILER_MESSAGE(_prefix, _source " : " _message)
#define WARNING_EX(_source, _message) COMPILER_MESSAGE_EX("Warning", _source, _message)
#define WARNING(_message) WARNING_EX(__FUNCTION__, _message)
#define FIXME_EX(_source, _message) COMPILER_MESSAGE_EX("FixMe", _source, _message)
#define FIXME(_message) FIXME_EX(__FUNCTION__, _message)
#define TODO_EX(_source, _message) COMPILER_MESSAGE_EX("ToDo", _source, _message)
#define TODO(_message) TODO_EX(__FUNCTION__, _message)
#define NOT_IMPLEMENTED_YET() FIXME("Not implemented yet")
#define NOT_IMPLEMENTED_YET_EX(_source) FIXME_EX(_source, "Not implemented yet")

//----------------------------------------------------------------------------//
// Settings
//----------------------------------------------------------------------------//

#ifdef _EDITOR
#	define APP_NAME "Editor"
#else
#	define APP_NAME "Game"
#endif

#ifdef _DEBUG // debug settings

#	define _FATAL // enable fatal errors handling
#	define _LOG // enable logging
#	define _DEBUG_RC // debug refcounting
#	define _DEBUG_GRAPHICS // enable OpenGL debug layer
#	define _STATISTICS // enable statistics

#else // release settings

#	define _FATAL // enable fatal errors handling
#	define _LOG // enable logging
//#	define _DEBUG_RC // debug refcounting
#	define _DEBUG_GRAPHICS // enable OpenGL debug layer
#	define _STATISTICS // enable statistics

#endif

#define _GL_CORE_PROFILE_CONTEXT // Create core profile context. Compatible context will be created otherwise.
#define _GL_ARB_CONTEXT // use wglCreateContextAttribsARB


//#define _HAVE_TYPE_NAMES // Enable name of type for Object

#if !defined(_HAVE_TYPE_NAMES) || defined(_LOG)
#	define _HAVE_TYPE_NAMES
#endif

//----------------------------------------------------------------------------//
// Fatal
//----------------------------------------------------------------------------//

#ifdef _FATAL
#	define CHECK(cond, ...) ((cond) ? ((void)0) : (Fatal("Assertion failed:\n" #cond " (" ##__VA_ARGS__ ")\n" __FILELINE__)))
void Fatal(const char* _msg);
#else
#	define CHECK(cond, ...)
#endif

//----------------------------------------------------------------------------//
// Log (TODO: write file)
//----------------------------------------------------------------------------//

#ifdef _LOG
void LogMessage(const char* _msg, ...);
struct LogNode
{
	LogNode(const char* _func);
	~LogNode(void);

	static int s_depth;
};
#	define LOG(msg, ...) LogMessage(msg, ##__VA_ARGS__)
#	define LOG_NODE(name) LogNode _logNode_(name)
#	define LOG_NODE_F(name, ...) LogNode _logNode_(String::Format(name, ##__VA_ARGS__))
#else
#	define LOG(msg, ...)
#	define LOG_NODE(name) 
#	define LOG_NODE_F(name, ...)
#endif

//----------------------------------------------------------------------------//
// Assert
//----------------------------------------------------------------------------//

#ifdef _DEBUG
#	define ASSERT(cond, ...) CHECK(cond, ##__VA_ARGS__)
#else
#	define ASSERT(cond, ...)
#endif

//----------------------------------------------------------------------------//
// DEBUG_CODE
//----------------------------------------------------------------------------//

#ifdef _DEBUG
#	define DEBUG_CODE(...) _EMPTY() ##__VA_ARGS__
#else
#	define DEBUG_CODE(...)
#endif

//----------------------------------------------------------------------------//
// Types
//----------------------------------------------------------------------------//

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef unsigned int uint;

//----------------------------------------------------------------------------//
// Memory
//----------------------------------------------------------------------------//

/*void* MemorySet(void* _dst, int _val, size_t _size);
void* MemoryCopy(void* _dst, const void* _src, size_t _size);
void* MemoryAlloc(size_t _size);
void MemoryFree(void* _p);*/

//----------------------------------------------------------------------------//
// NonCopyable
//----------------------------------------------------------------------------//

class NonCopyable
{
public:
	NonCopyable(void) { }
	~NonCopyable(void) { }

private:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator = (const NonCopyable&) = delete;
};

//----------------------------------------------------------------------------//
// Move semantics
//----------------------------------------------------------------------------//

template <class T> struct TypeWithoutRef { typedef T Type; };
template <class T> struct TypeWithoutRef<T&> { typedef T Type; };
template <class T> struct TypeWithoutRef<T&&> { typedef T Type; };

//!
template <class T> inline constexpr typename T&& Forward(typename TypeWithoutRef<T>::Type& _ref) { return static_cast<T&&>(_ref); }
//!
template <class T> inline constexpr typename T&& Forward(typename TypeWithoutRef<T>::Type&& _ref) { return static_cast<T&&>(_ref); }
//!
template <class T> inline typename TypeWithoutRef<T>::Type&& Move(T&& _ref) { return static_cast<TypeWithoutRef<T>::Type&&>(_ref); }

//!
template <typename T> void Swap(T& _a, T& _b)
{
	T _c = Move(_a);
	_a = Move(_b);
	_b = Move(_c);
}

//!
template <class T, class... Args> static void Construct(T* _dst, Args&&... _args) { new(_dst) T(Forward<Args>(_args)...); }
//!
template <class T> void Destroy(T* _dst) { _dst->~T(); }

//!
template <class T> void ConstructRange(T* _dst, const T* _end, const T* _src) { while (_dst < _end) Construct(_dst++, *_src++); }
//!
template <class T> void ConstructRange(T* _dst, const T* _end, const T& _src) { while (_dst < _end) Construct(_dst++, _src); }
//!
template <class T> void MoveRange(T* _dst, const T* _end, T* _src) { while (_dst < _end) Construct(_dst++, Move(*_src++)); }

//!
template <class T> void DestroyRange(T* _dst, const T* _end) { while (_dst < _end) Destroy(_dst++); }
														  
//!
template <class T> void MoveAndDestruct(T* _dst, T* _src) { *_dst = Move(*_src); Destroy(_dst); }
//!
template <class T> void Assign(T* _dst, const T* _end, const T& _src) { while (_dst < _end) *_dst++ = _src; }
//!
template <class T> void Copy(T* _dst, const T* _end, const T* _src) { while (_dst < _end) *_dst++ = *_src++; }

//----------------------------------------------------------------------------//
// Allocator utilities
//----------------------------------------------------------------------------//

//!
template <class T> T* AllocateBlock(uint _size) { return reinterpret_cast<T*>(new uint8[_size]); }
//!
template <class T> T* Allocate(uint _count) { return reinterpret_cast<T*>(new uint8[_count * sizeof(T)]); }
//!
template <class T> void Deallocate(T* _ptr) { delete[] reinterpret_cast<uint8*>(_ptr); }
//!
uint GrowTo(uint _currentSize, uint _newSize);

//----------------------------------------------------------------------------//
// InitializerList
//----------------------------------------------------------------------------//

//!
template <class T> using InitializerList = std::initializer_list<T>;

//----------------------------------------------------------------------------//
// Array
//----------------------------------------------------------------------------//

template <class T> class Array
{
public:
	typedef T* Iterator;
	typedef const T* ConstIterator;

	//!
	Array(void) = default;
	//!
	~Array(void)
	{
		Clear();
		delete[] reinterpret_cast<uint8*>(m_data);
	}
	//!
	Array(const Array& _rhs) { Push(_rhs.m_data, _rhs.m_size); }
	//!
	Array(Array&& _rhs) : m_data(_rhs.m_data), m_size(_rhs.m_size), m_capacity(_rhs.m_capacity)
	{
		_rhs.m_data = nullptr;
		_rhs.m_size = 0;
		_rhs.m_capacity = 0;
	}
	//!
	Array(const T* _e, uint _size) { Push(_e, _size); }
	//!
	Array(uint _size) { Push(T(), _size); }
	//!
	Array(InitializerList<T> _list) { Push(_list); }

	//!
	Array& operator = (const Array& _rhs)
	{
		if (m_data != _rhs.m_data)
			Clear().Push(_rhs.m_data, _rhs.m_size);
		return *this;
	}
	//!
	Array& operator = (Array&& _rhs)
	{
		Swap(m_data, _rhs.m_data);
		Swap(m_size, _rhs.m_size);
		Swap(m_capacity, _rhs.m_capacity);
		return *this;
	}
	//!
	Array& operator = (InitializerList<T> _list) { return Clear().Push(_list); }

	// [capacity]
	
	//!
	uint Size(void) const { return m_size; }
	//!
	Array& Reserve(uint _size) { _Realloc(_size); return *this; }
	//!
	T* Upsize(uint _append)
	{
		uint _offset = m_size;
		Resize(m_size + _append, T());
		return m_data + _offset;
	}
	//!
	Array& Resize(uint _newSize, const T& _sample = T())
	{
		if (_newSize > m_size)
			Push(_sample, _newSize - m_size);
		else if (_newSize < m_size)
			Pop(m_size - _newSize);
		return *this;
	}
	//!
	Array& Clear(void) { return Pop(m_size); }

	// [access]

	//!
	T& operator [] (uint _index) { ASSERT(_index < m_size); return m_data[_index]; }
	//!
	const T& operator [] (uint _index) const { ASSERT(_index < m_size); return m_data[_index]; }
	//!
	T* operator* (void) { return m_data; }
	//!
	const T* operator* (void) const { return m_data; }
	//!
	T* Data(void) { return m_data; }
	//!
	const T* Data(void) const { return m_data; }
	//!
	T& Front(void) { ASSERT(m_size > 0); return m_data[0]; }
	//!
	const T& Front(void) const { ASSERT(m_size > 0); return m_data[0]; }
	//!
	T& Back(void) { ASSERT(m_size > 0); return m_data[m_size - 1]; }
	//!
	const T& Back(void) const { ASSERT(m_size > 0); return m_data[m_size - 1]; }
	//!
	T& Top(void) { ASSERT(m_size > 0); return m_data[m_size - 1]; }
	//!
	const T& Top(void) const { ASSERT(m_size > 0); return m_data[m_size - 1]; }

	// [iterators]

	//!	
	Iterator Begin(void) { return Iterator(m_data); }
	//!	
	ConstIterator Begin(void) const { return ConstIterator(m_data); }
	//!	
	Iterator End(void) { return Iterator(m_data + m_size); }
	//!	
	ConstIterator End(void) const { return ConstIterator(m_data + m_size); }

	// [modifiers]

	//!
	Array& Push(const T* _e, uint _count)
	{
		uint _newUsed = m_size + _count;
		_Realloc(_newUsed);
		ConstructRange(m_data + m_size, m_data + _newUsed, _e);
		m_size = _newUsed;
		return *this;
	}
	//!
	Array& Push(const T& _e, uint _count = 1)
	{
		uint _newUsed = m_size + _count;
		_Realloc(_newUsed);
		ConstructRange(m_data + m_size, m_data + _newUsed, _e);
		m_size = _newUsed;
		return *this;
	}
	//!
	Array& Push(const T* _start, const T* _end) { return Push(_start, (uint)(_end - _start)); }
	//!
	Array& Push(InitializerList<T> _list) { return Push(_list.begin(), _list.end()); }
	//!
	Array& Push(T&& _e) 
	{ 
		uint _newUsed = m_size + 1;
		_Realloc(_newUsed);
		Construct(m_data + m_size, Forward<T>(_e));
		m_size = _newUsed;
		return *this;
	}

	//!
	Array& Pop(uint _count = 1)
	{
		if (_count > m_size)
			_count = m_size;
		uint _newUsed = m_size - _count;
		DestroyRange(m_data + _newUsed, m_data + m_size);
		m_size = _newUsed;
		return *this;
	}

	//!
	Array& Erase(uint _index)
	{
		if (_index < m_size)
		{
			--m_size;
			MoveAndDestruct(m_data + _index, m_data + m_size);
		}
		return *this;
	}
	//!
	Array& Remove(const T& _e) { return Erase(Find(_e)); }

	// [operations]

	//!
	uint Find(const T& _e, uint _startIndex = 0)
	{
		for (T *_p = m_data + _startIndex, *_end = m_data + m_size; _p < _end;)
			if (*_p == _e)
				return _p - m_data;
		return (uint)-1;
	}

protected:
	//!
	void _Realloc(uint _newSize)
	{
		if (m_capacity < _newSize)
		{
			_newSize = GrowTo(m_capacity, _newSize); // quantize memory
			T* _newData = Allocate<T>(_newSize);

			MoveRange(_newData, _newData + m_size, m_data);
			DestroyRange(m_data, m_data + m_size);

			Deallocate(m_data);
			m_data = _newData;
			m_capacity = _newSize;
		}
	}

	uint m_size = 0;
	uint m_capacity = 0;
	T* m_data = nullptr;
};

//!
template <class T> auto begin(Array<T>& _array)->decltype(_array.Begin()) { return _array.Begin(); }
//!
template <class T> auto begin(const Array<T>& _array)->decltype(_array.Begin()) { return _array.Begin(); }
//!
template <class T> auto end(Array<T>& _array)->decltype(_array.End()) { return _array.End(); }
//!
template <class T> auto end(const Array<T>& _array)->decltype(_array.End()) { return _array.End(); }

//----------------------------------------------------------------------------//
// Sort 
//----------------------------------------------------------------------------//

template <class T, class C> void _Sort(T* _data, int _first, int _last, C _cmp)
{
	int i = _first;	// lhs
	int j = _last;	// rhs
	T x = _data[(_first + _last) >> 1];	// pivot

	if (_cmp(_data[i], x) && _cmp(_data[j], _data[i]))
		x = _data[i];
	else if (_cmp(_data[j], x) && _cmp(_data[i], _data[j]))
		x = _data[j];

	do
	{
		while (_cmp(_data[i], x))
			i++;
		while (_cmp(x, _data[j]))
			j--;

		if (i <= j)
		{
			if (_cmp(_data[j], _data[i]))
				Swap(_data[i], _data[j]);
			i++;
			j--;
		}

	} while (i <= j);

	if (i < _last)
		_Sort(_data, i, _last, _cmp);
	if (_first < j)
		_Sort(_data, _first, j, _cmp);
}

template <class T, class C> void Sort(T _start, T _end, C _cmp)
{
	if (_start < _end)
		_Sort(_start, 0, uint(_end - _start) - 1, _cmp);
}

//----------------------------------------------------------------------------//
// List 
//----------------------------------------------------------------------------//

void _LL_Link(void* _head, void* _this, void* _node);
void _LL_Unlink(void* _head, void* _this, void* _node);
void _LL_LinkFirst(void* _first, void* _last, void* _this, void* _node);
void _LL_LinkLast(void* _first, void* _last, void* _this, void* _node);
void _LL_Unlink(void* _first, void* _last, void* _this, void* _node);

template <class T> void Link(T*& _head, T* _this, T*& _node) { _LL_Link(&_head, _this, &_node); }
template <class T> void Unlink(T*& _head, T* _this, T*& _node) { _LL_Unlink(&_head, _this, &_node); }
template <class T> void LinkFirst(T*& _first, T*& _last, T* _this, T*& _node) { _LL_LinkFirst(&_first, &_last, _this, &_node); }
template <class T> void LinkLast(T*& _first, T*& _last, T* _this, T*& _node) { _LL_LinkLast(&_first, &_last, _this, &_node); }
template <class T> void Unlink(T*& _first, T*& _last, T* _this, T*& _node) { _LL_Unlink(&_first, &_last, _this, &_node); }

//----------------------------------------------------------------------------//
// ListIterator
//----------------------------------------------------------------------------//

//! Iterator for double-linked nodes.
template <class T> class ListIterator
{
public:
	typedef typename TypeWithoutRef<decltype(reinterpret_cast<T*>(0)->Value())>::Type ValueType;
	//!
	ListIterator(T* _ptr = nullptr) : m_ptr(_ptr) { }

	//!	Implicit conversion from non-const iterator to const iterator.
	operator const typename ListIterator<const T>(void) const { return ListIterator<const T>(m_ptr); }

	//!	Get address.
	ValueType* operator & (void) { ASSERT(m_ptr != nullptr); return &(m_ptr->Value()); }
	//!	Get address.
	ValueType* operator -> (void) { ASSERT(m_ptr != nullptr); return &(m_ptr->Value()); }
	//!	Get reference.
	ValueType& operator * (void) { ASSERT(m_ptr != nullptr); return m_ptr->Value(); }
	//!	Get node.
	T* Node(void) { return m_ptr; }
	//!
	operator T* (void) { return m_ptr; }

	//!	Go to next element.
	ListIterator& operator ++ (void) { ASSERT(m_ptr != nullptr); m_ptr = m_ptr->Next(); return *this; }
	//!	Go to next element.
	ListIterator operator ++ (int) { ASSERT(m_ptr != nullptr); ListIterator _next(m_ptr); m_ptr = m_ptr->Next(); return _next; }
	//!	Go to prev element.
	ListIterator& operator -- (void) { ASSERT(m_ptr != nullptr); m_ptr = m_ptr->Prev(); return *this; }
	//!	Go to prev element.
	ListIterator operator -- (int) { ASSERT(m_ptr != nullptr); ListIterator _prev(m_ptr); m_ptr = m_ptr->Prev(); return _prev; }

	//!
	bool operator == (const ListIterator& _rhs) const { return m_ptr == _rhs.m_ptr; }
	//!
	bool operator != (const ListIterator& _rhs) const { return m_ptr != _rhs.m_ptr; }

protected:

	T* m_ptr;
};

//----------------------------------------------------------------------------//
// PageAllocator
//----------------------------------------------------------------------------//

template <class T, uint S = 4096 / sizeof(T)> class PageAllocator : public NonCopyable
{
	struct Block
	{
		Block* next;
	};

	struct Page
	{
		Page* prev = nullptr;
		Page* next = nullptr;
		uint8* Data(void) { return reinterpret_cast<uint8*>(this) + sizeof(*this); }
		Block* At(uint _idx) { return reinterpret_cast<Block*>(Data() + _idx * sizeof(T)); }
	};

public:
	enum : uint { Size = S }; // num elements

	PageAllocator(void) = default;
	~PageAllocator(void) { FreeMemory(); }

	void FreeMemory(void)
	{
		while (m_root)
		{
			Page* _page = m_root;
			Unlink(m_root, _page, _page->prev);
			Deallocate(_page);
		}
	}

	T* Alloc(void)
	{
		if (!m_free)
			_AddPage();

		Block* _block = m_free;
		m_free = m_free->next;
		return reinterpret_cast<T*>(_block);
	}
	void Free(T* _e)
	{
		if (_e)
		{
			Block* _block = reinterpret_cast<Block*>(_e);
			_block->next = m_free;
			m_free = _block;
		}
	}

	template <class... A> T* New(A&&... _args)
	{
		T* _new = Alloc();
		Construct(_new, Forward<A>(_args)...);
		return _new;
	}
	void Delete(T* _e)
	{
		if (_e)
		{
			Destroy(_e);
			Free(_e);
		}
	}

protected:
	void _AddPage(void)
	{
		Page* _page = AllocateBlock<Page>(sizeof(Page) + Size * sizeof(T));
		_page->prev = nullptr;
		_page->next = nullptr;
		for (uint i = 0; i < Size; ++i)
			_page->At(i)->next = _page->At(i + 1);
		_page->At(Size - 1)->next = m_free;
		m_free = _page->At(0);

		Link(m_root, _page, _page->prev);
	}

	Page* m_root = nullptr;
	Block* m_free = nullptr;
};

//----------------------------------------------------------------------------//
// Pair
//----------------------------------------------------------------------------//

//! Pair. 
template <class T, class U> struct Pair
{
	T first;
	U second;

	Pair(void) = default;
	Pair(const T& _first, const U& _second) : first(_first), second(_second) { }
	Pair(T&& _first, U&& _second) : first(Forward<T>(_first)), second(Forward<U>(_second)) { }
};

//----------------------------------------------------------------------------//
// Hash 
//----------------------------------------------------------------------------//

//! SDMB hash
inline uint Hash(const void* _data, uint _size, uint _hash = 0)
{
	const uint8* _start = reinterpret_cast<const uint8*>(_data);
	const uint8* _end = _start + _size;
	while (_start < _end)
		_hash = *_start++ + +(_hash << 6) + (_hash << 16) - _hash;
	return _hash;
}

//! Generic hash function.
template <class T> inline uint MakeHash(const T& _value) { return Hash(&_value, sizeof(_value)); }
//!
template <> inline uint MakeHash(const char& value) { return (uint)value; }
//!
template <> inline uint MakeHash(const unsigned char& value) { return (uint)value; }
//!
template <> inline uint MakeHash(const int16& _value) { return (uint)_value; }
//!
template <> inline uint MakeHash(const uint16& _value) { return (uint)_value; }
//!
template <> inline uint MakeHash(const uint& _value) { return (uint)_value; }
//!
template <> inline uint MakeHash(const int& _value) { return (uint)_value; }
//!
template <> inline uint MakeHash(const int64& _value) { return (uint)((_value >> 32) | (_value & 0xffffffff)); }
//!
template <> inline uint MakeHash(const uint64& _value) { return (uint)((_value >> 32) | (_value & 0xffffffff)); }
//!
inline uint MakeHash(void* _value) { return (uint)(size_t)_value; }
//!
inline uint MakeHash(const void* _value) { return (uint)(size_t)_value; }
//!
template <class T> inline uint MakeHash(T* _value) { return (uint)((size_t)_value / sizeof(T)); }
//!
template <class T> inline uint MakeHash(const T* _value) { return (uint)((size_t)_value / sizeof(T)); }

//----------------------------------------------------------------------------//
// HashMap
//----------------------------------------------------------------------------//

//! Unordered associative array.
template <class T, class U> class HashMap
{
public:
	//! Max size of bucket = 2^(MAX_BUCKET_SIZE_POW2) (0=1, 1=2, 2=4, 3=8 ...)
	static const uint MAX_BUCKET_SIZE_POW2 = 0;
	//!
	typedef Pair<const T, U> KeyValue;

	//!
	struct Node;

	//!
	struct NodeBase
	{
		//!
		Node* Next(void) { return m_next; }
		//!
		const Node* Next(void) const { return m_next; }
		//!
		Node* Prev(void) { return m_prev; }
		//!
		const Node* Prev(void) const { return m_prev; }

	protected:
		friend class HashMap;
		//!
		Node* m_prev;
		//!
		Node* m_next;
	};

	//!
	struct Node : public NodeBase
	{
		//!
		KeyValue& Value(void) { return m_value; }
		//!
		const KeyValue& Value(void) const { return m_value; }
		//!
		Node* Down(void) { return m_down; }
		//!
		const Node* Down(void) const { return m_down; }

	protected:
		friend class HashMap;
		//!
		Node* m_down;
		//!
		uint m_hash;
		//!
		KeyValue m_value;
	};

	//!
	typedef ListIterator<Node> Iterator;
	//!
	typedef ListIterator<const Node> ConstIterator;

	//!
	HashMap(void) { _ResetList(); }
	//!
	~HashMap(void)
	{
		Clear();
		delete[] m_buckets;
	}
	//!
	HashMap(const HashMap& _other)
	{
		_ResetList();
		Insert(_other.Begin(), _other.End());
	}
	//!
	HashMap(HashMap&& _temp) : m_size(_temp.m_size)
	{
		Node* _first = _temp.m_tail.m_next;
		Node* _last = _temp.m_tail.m_prev;
		if (_first != _last)
		{
			_BindList(_first, _last);
			_temp._ResetList();
			Swap(m_buckets, _temp.m_buckets);
			Swap(m_numBuckets, _temp.m_numBuckets);
		}
		else
			_ResetList();
	}
	//!
	HashMap(ConstIterator _start, ConstIterator _end) : HashMap() { Insert(_start, _end); }
	//!
	HashMap(InitializerList<KeyValue> _list) : HashMap() { Insert(_list); }

	//!
	HashMap& operator = (const HashMap& _rhs)
	{
		if (m_buckets != _rhs.m_buckets)
			Clear().Insert(_rhs.Begin(), _rhs.End());
		return *this;
	}
	//!
	HashMap& operator = (HashMap&& _rhs)
	{
		Node* _first = _rhs.m_tail.m_next;
		Node* _last = _rhs.m_tail.m_prev;
		uint _size = _rhs.m_size;

		if (m_size)
		{
			_rhs._BindList(m_tail.m_next, m_tail.m_prev);
			_rhs.m_size = m_size;
		}
		else
			_rhs._ResetList();

		if (_first != _last)
		{
			_BindList(_first, _last);
			m_size = _size;
		}
		else
			_ResetList();

		Swap(m_buckets, _rhs.m_buckets);
		Swap(m_numBuckets, _rhs.m_numBuckets);

		return *this;
	}
	//!
	HashMap& operator = (InitializerList<KeyValue> _list) { return Clear().Insert(_list); }


	//!
	const T& Front(void) const { ASSERT(m_size != 0); return m_tail.m_next->m_value.first; }
	//!
	const T& Back(void) const { ASSERT(m_size != 0); return m_tail.m_prev->m_value.first; }

	//!
	Iterator Begin(void) { return m_tail.m_next; }
	//!
	ConstIterator Begin(void) const { return m_tail.m_next; }
	//!
	Iterator End(void) { return static_cast<Node*>(&m_tail); }
	//!
	ConstIterator End(void) const { return static_cast<const Node*>(&m_tail); }


	//!
	uint Size(void) const { return m_size; }
	//!
	bool IsEmpty(void) const { return m_size == 0; }
	//!
	bool NonEmpty(void) const { return m_size != 0; }
	//!
	HashMap& Clear(void)
	{
		while (m_size)
		{
			Node* _head = m_tail.m_next;
			Node* _next = _head->m_next;
			_head->m_next->m_prev = _head->m_prev;
			_head->m_prev->m_next = _head->m_next;
			--m_size;
		}
		memset(m_buckets, 0, m_numBuckets * sizeof(m_buckets[0]));
		return *this;
	}

	//!
	uint BucketCount(void) const { return m_numBuckets; }

	//!
	U& operator[] (const T& _key)
	{
		uint _hash = MakeHash(_key);
		Node* _node = _Find(_key, _hash);
		if (!_node)
		{
			_node = _Insert(_hash);
			Construct(const_cast<T*>(&_node->m_value.first), _key);
			Construct(&_node->m_value.second);
		}
		return _node->m_value.second;
	}
	//!
	U& operator[] (T&& _key)
	{
		uint _hash = MakeHash(_key);
		Node* _node = _Find(_key, _hash);
		if (!_node)
		{
			_node = _Insert(_hash);
			Construct(const_cast<T*>(&_node->m_value.first), Forward<T>(_key));
			Construct(&_node->m_value.second);
		}
		return _node->m_value.second;
	}

	//!
	Iterator Insert(const KeyValue& _value)
	{
		uint _hash = MakeHash(_value.first);
		Node* _node = _Find(_value.first, _hash);
		if (!_node)
		{
			_node = _Insert(_hash);
			Construct(const_cast<T*>(&_node->m_value.first), _value.first);
			Construct(&_node->m_value.second, _value.second);
		}
		else
			_node->m_value.second = _value.second;
		return _node;
	}
	//!
	Iterator Insert(KeyValue&& _value)
	{
		uint _hash = MakeHash(_value.first);
		Node* _node = _Find(_value.first, _hash);
		if (!_node)
		{
			_node = _Insert(_hash);
			//Construct(const_cast<T*>(&_node->m_value.first), Forward<T>(_value.first));
			//Construct(&_node->m_value.second, Forward<U>(_value.second));
			Construct(&_node->m_value, Forward<KeyValue>(_value));
		}
		else
			_node->m_value.second = _value.second;
		return _node;
	}
	//!
	Iterator Insert(ConstIterator _iter)
	{
		ASSERT(_iter.Node() != nullptr);
		Node* _insert = _iter.Node();
		uint _hash = _node->m_hash;
		Node* _node = _Find(_insert->m_value.first, _hash);
		if (!_node)
		{
			_node = _Insert(_hash);
			Construct(const_cast<T*>(&_newNode->m_value.first), _insert->m_value.first);
			Construct(&_newNode->m_value.second, _insert->m_value.second);
		}
		else
			_node->m_value.second = _insert->m_value.second;

		return _node;
	}
	//!
	Iterator Insert(ConstIterator _start, ConstIterator _end)
	{
		if (_start != _end)
		{
			Iterator _r = Insert(_start++);
			for (; _start != _end; ++_start)
				Insert(_start);
			return _r;
		}
		return End();
	}
	//!
	Iterator Insert(InitializerList<KeyValue> _list)
	{
		if (_list.size())
		{
			const KeyValue* _start = _list.begin();
			Iterator _r = Insert(*_start++);
			for (; _start != _list.end(); ++_start)
				Insert(*_start);
			return _r;
		}
		return End();
	}

	//!
	Iterator Erase(const T& _key)
	{
		uint _hash = MakeHash(_key);
		uint _index = _hash & (m_numBuckets - 1);
		Node* _prev = nullptr;
		Node* _node = _Find(_key, _index, _prev);
		if (_node)
			return _Erase(_node, _index, _prev);
		return End();
	}
	//!
	Iterator Erase(ConstIterator _pos)
	{
		Node* _node = _pos.Node();
		ASSERT(_node != nullptr);
		if (_pos != &m_tail)
		{
			uint _index = _node->m_hash & (m_numBuckets - 1);
			return _Erase(_node, _index, _Prev(m_buckets[_index], _node));
		}
		return End();
	}

	//!
	Iterator Find(const T& _key)
	{
		uint _hash = MakeHash(_key);
		Node* _node = _Find(_key, _hash);
		return _node ? _node : End();
	}
	//!
	ConstIterator Find(const T& _key) const
	{
		uint _hash = MakeHash(_key);
		Node* _node = _Find(_key, _hash);
		return _node ? _node : End();
	}
	//!
	bool Contains(const T& Key) const
	{
		return _Find(_key, MakeHash(_key)) != nullptr;
	}

protected:
	//!
	void _ResetList(void)
	{
		m_size = 0;
		m_tail.m_next = static_cast<Node*>(&m_tail); // head --> tail
		m_tail.m_prev = static_cast<Node*>(&m_tail); // tail <-- head
	}
	//!
	void _BindList(Node* _first, Node* _last)
	{
		ASSERT(_first != _last);
		ASSERT(_first != nullptr);
		ASSERT(_last != nullptr);

		m_tail.m_next = _first;
		_first->m_prev = static_cast<Node*>(&m_tail);
		m_tail.m_prev = _last;
		_last->m_next = static_cast<Node*>(&m_tail);
	}
	//!
	Node* _Find(const T& _key, uint _hash) const
	{
		for (Node* _node = m_buckets ? m_buckets[_hash & (m_numBuckets - 1)] : nullptr; _node; _node = _node->m_down)
		{
			if (_node->m_value.first == _key)
				return _node;
		}
		return nullptr;
	}
	//!
	Node* _Find(const T& _key, uint _index, Node*& _prev) const
	{
		for (Node* _node = m_buckets ? m_buckets[_index] : nullptr; _node; _prev = _node, _node = _node->m_down)
		{
			if (_node->m_value.first == _key)
				return _node;
		}
		return nullptr;
	}
	//!
	Node* _Prev(Node* _bucket, Node* _node)
	{
		for (Node* _prev = nullptr; _bucket; _prev = _bucket, _bucket = _bucket->m_down)
		{
			if (_bucket == _node)
				return _prev;
		}
		return nullptr;
	}
	//!
	void _Rehash(uint _newSize)
	{
		Node** _newData = new Node*[_newSize];
		memset(_newData, 0, _newSize * sizeof(Node*));

		delete[] m_buckets;
		m_buckets = _newData;
		m_numBuckets = _newSize;

		uint _mask = m_numBuckets - 1;
		for (Node* i = m_tail.m_next; i != &m_tail; i = i->m_next)
		{
			uint _index = i->m_hash & _mask;
			i->m_down = m_buckets[_index];
			m_buckets[_index] = i;
		}
	}
	//!
	Node* _Insert(uint _hash)
	{
		if (!m_buckets)
		{
			m_numBuckets = 4; // min num buckets
			m_buckets = new Node*[4];
			memset(m_buckets, 0, m_numBuckets * sizeof(Node*));
		}

		uint _index = _hash & (m_numBuckets - 1);
		Node* _newNode = Allocate<Node>(1);
		_newNode->m_hash = _hash;

		_newNode->m_next = static_cast<Node*>(&m_tail);
		_newNode->m_prev = m_tail.m_prev;
		m_tail.m_prev->m_next = _newNode;
		m_tail.m_prev = _newNode;

		_newNode->m_down = m_buckets[_index];
		m_buckets[_index] = _newNode;

		++m_size;

		if (m_size > m_numBuckets << MAX_BUCKET_SIZE_POW2)
			_Rehash(m_numBuckets << 1);

		return _newNode;
	}
	//!
	Node* _Erase(Node* _node, uint _index, Node* _prev)
	{
		ASSERT(_node != nullptr);
		ASSERT(_node != &m_tail);

		if (_prev)
			_prev->m_down = _node->m_down;
		else
			m_buckets[_index] = _node->m_down;

		Node* _next = _node->m_next;
		_node->m_next->m_prev = _node->m_prev;
		_node->m_prev->m_next = _node->m_next;
		--m_size;

		Destroy(&_node->m_value);
		Deallocate(_node);

		return _next;
	}


	NodeBase m_tail;
	Node** m_buckets = nullptr;
	uint m_numBuckets = 0;
	uint m_size;
};

//!
template <class T, class U> auto begin(HashMap<T, U>& _map)->decltype(_map.Begin()) { return _map.Begin(); }
//!
template <class T, class U> auto begin(const HashMap<T, U>& _map)->decltype(_map.Begin()) { return _map.Begin(); }
//!
template <class T, class U> auto end(HashMap<T, U>& _map)->decltype(_map.End()) { return _map.End(); }
//!
template <class T, class U> auto end(const HashMap<T, U>& _map)->decltype(_map.End()) { return _map.End(); }

//----------------------------------------------------------------------------//
// String
//----------------------------------------------------------------------------//

class String
{
public:
	typedef char* Iterator;
	typedef const char* ConstIterator;

	//!
	String(void) = default;
	//!
	~String(void) { if (m_data) Deallocate(m_data); }
	//!
	String(const String& _other) { Append(_other); }
	//!
	String(String&& _temp) : m_length(_temp.m_length), m_capacity(_temp.m_capacity), m_data(_temp.m_data) { _temp.m_data = nullptr; }
	//!
	String(const char* _str, int _length = -1) { Append(_str, _length); }
	//!
	String(const char* _str, const char* _end) : String(_str, (int)(_end - _str)) { }
	//!
	String(const char _ch) : String(&_ch, 1) { }

	//!
	String& operator = (const String& _rhs);
	//!
	String& operator = (String&& _rhs);
	//!
	String& operator = (const char* _rhs) { return Clear().Append(_rhs, -1); }
	//!
	String& operator = (char _rhs) { return Clear().Append(&_rhs, 1); }

	//!
	String& operator += (const String& _str) { return Append(_str); }
	//!
	String& operator += (const char* _str) { return Append(_str); }
	//!
	String& operator += (char _ch) { return Append(_ch); }

	//!
	String operator + (const String& _str) { return String(*this).Append(_str); }
	//!
	String operator + (const char* _str) { return String(*this).Append(_str); }
	//!
	String operator + (char _ch) { return String(*this).Append(_ch); }

	//!
	friend String operator + (const char* _lhs, const String& _rhs) { return String(_lhs).Append(_rhs); }
	//!
	friend String operator + (char _lhs, const String& _rhs) { return String(_lhs).Append(_rhs); }

	//!
	char& operator [] (int _index) { ASSERT((uint)_index <= m_length); return m_data[_index]; }
	//!
	char operator [] (int _index) const { ASSERT((uint)_index <= m_length); return m_data[_index]; }
	//!
	operator const char* (void) const { return m_data; }
	//!
	char* operator* (void) { return m_data; }
	//!
	const char* operator* (void) const { return m_data; }
	//!
	const char* CStr(void) const { return m_data; }
	//!
	char* Data(void) { return m_data; }

	//!	
	Iterator Begin(void) { return Iterator(m_data); }
	//!	
	ConstIterator Begin(void) const { return ConstIterator(m_data); }
	//!	
	Iterator End(void) { return Iterator(m_data + m_length); }
	//!	
	ConstIterator End(void) const { return ConstIterator(m_data + m_length); }

	//!
	bool operator == (const char* _rhs) const { return Compare(m_data, _rhs) == 0; }
	//!
	bool operator != (const char* _rhs) const { return Compare(m_data, _rhs) != 0; }

	//!
	uint Length(void) const { return m_length; }
	//!
	String& Clear(void);
	//!
	String& Reserve(uint _maxLength);
	//!
	String& Resize(uint _newLength, char _ch = 0);

	//!
	String& Append(const String& _str) { return Append(_str.m_data, _str.Length()); }
	//!
	String& Append(const char* _str, int _length = -1);
	//!
	String& Append(const char* _s, const char* _e) { ASSERT(_s <= _e); return Append(_s, (uint)(_e - _s)); }
	//!
	String& Append(char _ch) { return Append(&_ch, 1); }

	//!
	uint Hash(void) const { return Hash(m_data); }

	//!
	static int Compare(const char* _lhs, const char* _rhs);
	//!
	static uint Length(const char* _str, int _length = -1);

	//!
	static constexpr bool IsAlpha(char _ch) { return (_ch >= 'A' && _ch <= 'Z') || (_ch >= 'a' && _ch <= 'z') || ((uint8)_ch >= 0xc0); }
	//!
	static constexpr char Lower(char _ch) { return IsAlpha(_ch) ? (_ch | 0x20) : _ch; }
	//!
	static constexpr char Upper(char _ch) { return IsAlpha(_ch) ? (_ch & ~0x20) : _ch; }

	//!\return BSD checksum
	static constexpr uint ConstHash(const char* _str, uint _hash = 0) { return *_str ? ConstHash(_str + 1, ((_hash >> 1) + ((_hash & 1) << 15) + Lower(*_str)) & 0xffff) : _hash; }
	//!\return BSD checksum
	static uint Hash(const char* _str, uint _hash = 0);

	//!
	static char* FromInt(char* _dst, int _val, uint _digits);
	//!
	static char* FromFloat(char* _dst, float _val, uint _digits);

	//!
	static String FromInt(int _val, uint _digits = 0) { char _buff[16]; FromInt(_buff, _val, _digits); return _buff; }
	//!
	static String FromFloat(float _val, uint _digits = 6) { char _buff[64]; FromFloat(_buff, _val, _digits); return _buff; }
	//!
	static String PrintSize(uint _size, float _div, const char* _prefix, const char* _suffix);
	//!
	static String Format(const char* _fmt, ...);
	//!
	static String FormatV(const char* _fmt, va_list _args);

protected:
	uint m_length = 0;
	uint m_capacity = 0;
	char* m_data = nullptr;
};

//!
template <class T> inline uint MakeHash(const String& _value) { return _value.Hash(); }

//!
inline auto begin(String& _string)->decltype(_string.Begin()) { return _string.Begin(); }
//!
inline auto begin(const String& _string)->decltype(_string.Begin()) { return _string.Begin(); }
//!
inline auto end(String& _string)->decltype(_string.End()) { return _string.End(); }
//!
inline auto end(const String& _string)->decltype(_string.End()) { return _string.End(); }

//----------------------------------------------------------------------------//
// StringHash 
//----------------------------------------------------------------------------//

//!
struct StringHash
{
	StringHash(uint _value) : value(_value) { }
	StringHash(const char* _str) : value(String::Hash(_str)) { }

	operator uint (void) const { return value; }

	uint value = 0;
};

//----------------------------------------------------------------------------//
// Function
//----------------------------------------------------------------------------//

template <class F> void* FuncPtr(F _func) { union { F f; void* p; }_fp = { _func }; return _fp.p; }
template <class F> F FuncCast(void* _func) { union { void* p; F f; }_fp = { _func }; return _fp.f; }

// example:
// auto func = Function<void(int)>(Func); // c-func
// func = Function<void(int)>(&var, &MyClass::Func); // method
// func(0);	// call

template <class F> struct Function;
template <class R, class... A> struct Function<R(A...)>
{
	// TODO: calling convention
	typedef R(*Invoke)(void*, void*, A&&...);

	typedef R(*Ptr)(A...);
	typedef R(Type)(A...);

	Invoke invoke;
	void* func;
	void* self;

	Function(void) : invoke(nullptr), func(nullptr), self(nullptr) { }
	Function(R(*_func)(A...)) : invoke(InvokeFunc), func(FuncPtr(_func)), self(nullptr) { }
	template <class C> Function(C* _self, R(C::*_func)(A...)) : invoke(InvokeMethod<C>), func(FuncPtr(_func)), self(_self) { ASSERT(_self != nullptr); }
	template <class C> Function(const C* _self, R(C::*_func)(A...) const) : invoke(InvokeConstMethod<C>), func(FuncPtr(_func)), self(const_cast<C*>(_self)) { ASSERT(_self != nullptr); }
	operator bool(void) const { return func != nullptr; }

	R operator () (A... _args) const
	{
		ASSERT(func != nullptr);
		return invoke(self, func, Forward<A>(_args)...);
	}

	static R InvokeFunc(void* _self, void* _func, A&&... _args)
	{
		typedef R(*Func)(A...);
		return FuncCast<Func>(_func)(Forward<A>(_args)...);
	}

	template <class C> static R InvokeMethod(void* _self, void* _func, A&&... _args)
	{
		ASSERT(_self != nullptr);
		typedef R(C::*Func)(A...);
		return (*((C*)_self).*FuncCast<Func>(_func))(Move(_args)...);
	}

	template <class C> static R InvokeConstMethod(void* _self, void* _func, A&&... _args)
	{
		ASSERT(_self != nullptr);
		typedef R(C::*Func)(A...) const;
		return (*((const C*)_self).*FuncCast<Func>(_func))(Move(_args)...);
	}
};

template <class R, class... A> Function<R(A...)> MakeFunction(R(*_func)(A...))
{
	return Function<R(A...)>(_func);
}
template <class C, class R, class... A> Function<R(A...)> MakeFunction(C* _self, R(C::*_func)(A...))
{
	return Function<R(A...)>(_self, _func);
}
template <class C, class R, class... A> Function<R(A...)> MakeFunction(const C* _self, R(C::*_func)(A...) const)
{
	return Function<R(A...)>(_self, _func);
}

//----------------------------------------------------------------------------//
// ArgsHolder
//----------------------------------------------------------------------------//

template <class T> struct TArgType { typedef T Type; };
template <class T> struct TArgType<const T> { typedef T Type; };
template <class T> struct TArgType<T&> { typedef T& Type; };
template <class T> struct TArgType<const T&> { typedef T Type; };
template <class T> struct TArgType<T&&> { typedef T Type; };

template <int I, typename T> struct TArgHolder
{
	typename TypeWithoutRef<T>::Type arg;

	TArgHolder(void) = default;
	TArgHolder(typename TypeWithoutRef<T>::Type&& _arg) : arg(Forward<T>(_arg)) { }
	
	//typename TypeWithoutRef<T>::Type* arg;
	//TArgHolder(typename TypeWithoutRef<T>::Type* _arg) : arg(_arg) { }
	//~TArgHolder(void) { delete arg; }
};

template <int... I> struct TIndicesTuple { };

template <int N, typename I = TIndicesTuple<>> struct TArgIndexer;
template <int N, int... I> struct TArgIndexer<N, TIndicesTuple<I...>> : TArgIndexer <N - 1, TIndicesTuple<I..., sizeof...(I)>> { };
template <int... I> struct TArgIndexer<0, TIndicesTuple<I...>>
{
	typedef TIndicesTuple<I...> Tuple;
};

template <typename I, typename... A> struct TArgsHolder;
template <int... I, typename... A> struct TArgsHolder <TIndicesTuple<I...>, A...> : TArgHolder<I, A>...
{
	//TArgsHolder(A&&... _args = A...()) : TArgHolder<I, A>(new TypeWithoutRef<A>::Type(_args))... {}
	//TArgsHolder(void) = default;
	TArgsHolder(A&&... _args = A()...) : TArgHolder<I, A>(Forward<A>(_args))... {}

	template <class R, class F, int... I> R Invoke(const Function<F>& _func, const TIndicesTuple<I...>&)
	{
		//return _func(static_cast<typename TypeWithoutRef<A>::Type&>(*TArgHolder<I, A>::arg)...);
		return _func(TArgHolder<I, A>::arg...);
	}
};

template <class... A> struct ArgsHolder : TArgsHolder<typename TArgIndexer<sizeof...(A)>::Tuple, A...>
{
	typedef typename TArgsHolder<typename TArgIndexer<sizeof...(A)>::Tuple, A...> Type;
	typedef typename TArgIndexer<sizeof...(A)> Indexer;
	static typename const Indexer::Tuple Indices;

	template <class R> static R InvokeFunc(const Function<R(A...)>& _func, Type& _args)
	{
		return _args.Invoke<R, R(A...)>(_func, Indices);
	}
};
template <class... A> const typename ArgsHolder<A...>::Indexer::Tuple ArgsHolder<A...>::Indices;

//----------------------------------------------------------------------------//
// Singleton
//----------------------------------------------------------------------------//

template <class T> class Singleton : public NonCopyable
{
public:
	Singleton(void)
	{
		ASSERT(s_instance == nullptr);
		s_instance = static_cast<T*>(this);
	}
	~Singleton(void)
	{
		s_instance = nullptr;
	}

	static T* Get(void) { return s_instance; }
	template<class X> static X* Get(void) { return static_cast<X*>(s_instance); }

protected:

	static T* s_instance;
};

template <class T> T* Singleton<T>::s_instance = nullptr;

//----------------------------------------------------------------------------//
// RefCounted
//----------------------------------------------------------------------------//

class RefCounted : public NonCopyable
{
public:
	RefCounted(void);
	virtual ~RefCounted(void);

	void AddRef(void) const { ++m_refCount; }
	void Release(void) const { if (!--m_refCount) delete this; }
	int GetRefCount(void) const { return m_refCount; }
#ifdef _DEBUG_RC
	static int GetNumObjects(void) { return s_numObjects; }
#endif

protected:
	mutable int m_refCount = 0;
#ifdef _DEBUG_RC
	static int s_numObjects;
#endif
};

//----------------------------------------------------------------------------//
// SharedPtr
//----------------------------------------------------------------------------//

template <class T> class SharedPtr
{
public:
	SharedPtr(void) : p(nullptr) { }
	~SharedPtr(void) { if (p) p->Release(); }
	SharedPtr(const T* _p) : p(const_cast<T*>(_p)) { if (p) p->AddRef(); }
	SharedPtr(const SharedPtr& _p) : SharedPtr(_p.p) {}
	SharedPtr& operator = (const T* _p) { if (_p) _p->AddRef(); if (p) p->Release(); p = const_cast<T*>(_p); return *this; }
	SharedPtr& operator = (const SharedPtr& _p) { return *this = _p.p; }
	operator T* (void) const { return const_cast<T*>(p); }
	//T* operator & (void) const { return const_cast<T*>(p); }
	T* operator -> (void) const { return const_cast<T*>(p); }
	T& operator * (void) const { return *const_cast<T*>(p); }
	template <class X> X* Cast(void) const { return static_cast<X*>(const_cast<T*>(p)); }

protected:
	T* p;
};

//----------------------------------------------------------------------------//
// Object
//----------------------------------------------------------------------------//

#ifdef _HAVE_TYPE_NAMES
#define RTTI(TYPE) \
	enum : uint { TypeID = String::ConstHash(TYPE) }; \
	uint GetTypeID(void) override { return TypeID; } \
	bool IsTypeOf(uint _type) override { return _type == TypeID || __super::IsTypeOf(_type); } \
	template <class T> bool IsTypeOf(void) { return IsTypeOf(T::TypeID); } \
	static constexpr const char* TypeName = TYPE; \
	const char* GetTypeName(void) override { return TypeName; }
#else
#define RTTI(TYPE) \
	enum : uint { TypeID = String::ConstHash(TYPE) }; \
	uint GetTypeID(void) override { return TypeID; } \
	bool IsTypeOf(uint _type) override { return _type == TypeID || __super::IsTypeOf(_type); } \
	template <class T> bool IsTypeOf(void) { return IsTypeOf(T::TypeID); }
#endif

typedef SharedPtr<class Object> ObjectPtr;

class Object : public RefCounted
{
public:
	enum : uint { TypeID = String::ConstHash("Object") };
	virtual uint GetTypeID(void) { return TypeID; }
	virtual bool IsTypeOf(uint _type) { return _type == TypeID; }
	template <class T> bool IsTypeOf(void) { return IsTypeOf(T::TypeID); }

#ifdef _HAVE_TYPE_NAMES
	static constexpr const char* TypeName = "Object";
	virtual const char* GetTypeName(void) { return TypeName; }
#endif

	typedef SharedPtr<Object> (*Factory)(void);
	template <class T> static SharedPtr<Object> Create() { return new T; }

protected:
};

//----------------------------------------------------------------------------//
// TypeInfo
//----------------------------------------------------------------------------//

struct TypeInfo
{
	typedef ObjectPtr(*FactoryPfn)(void);

	uint type;
#ifdef _HAVE_TYPE_NAMES
	const char* name;
#endif
	FactoryPfn Factory = nullptr;
	uint flags = 0; //!< type-specific flags

	TypeInfo* SetFactory(FactoryPfn _factory) { Factory = _factory; return this; }
	TypeInfo* SetFlags(uint _flags) { flags = _flags; return this; }
	TypeInfo* AddFlags(uint _flags) { flags |= _flags; return this; }
	bool HasAnyOfFlags(uint _flags) { return (flags & _flags) != 0; }
};

//----------------------------------------------------------------------------//
// Reflection
//----------------------------------------------------------------------------//

#define gReflection Reflection::Get()

//! Reflection system. Created by Application
class Reflection : public Singleton<Reflection>
{
public:
	Reflection(void);
	~Reflection(void);

#ifdef _HAVE_TYPE_NAMES
	TypeInfo* GetOrCreateTypeInfo(const char* _name);
#else
	TypeInfo* GetOrCreateTypeInfo(uint _type);
#endif
	TypeInfo* GetTypeInfo(uint _type);

	template <class T> TypeInfo* GetOrCreateTypeInfo(void)
	{
#ifdef _HAVE_TYPE_NAMES
		return GetOrCreateTypeInfo(T::TypeName);
#else
		return GetOrCreateTypeInfo(T::TypeID);
#endif
	}

	//ObjectPtr Create(uint _type);

	template <class T> SharedPtr<T> Create(void)
	{
		auto _iter = m_types.Find(T::TypeID);
		if (_iter != m_types.End())
			return _iter->second.Factory().Cast<T>();

		LOG("Error: Factory for %s not found", T::TypeName); // or better use of FATAL?
		return nullptr;
	}

protected:
	HashMap<uint, TypeInfo> m_types;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
