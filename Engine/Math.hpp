#pragma once

#include "Core.hpp"

//----------------------------------------------------------------------------//
// Math
//----------------------------------------------------------------------------//

static const float EPSILON = 1e-6f;
static const float EPSILON2 = 1e-12f;
static const float PI = 3.1415926535897932384626433832795f;
static const float HALF_PI = PI * 0.5f;
static const float QUAD_PI = PI * 0.25f;
static const float DEGREES = 57.295779513082320876798154814105f;
static const float RADIANS = 0.01745329251994329576923690768489f;
static const float BYTE2FLOAT = 1.0f / 255.0f;
static const float FLOAT2BYTE = 255.0f;

typedef uint16 float16;

template <typename T> const T& Min(const T& _a, const T& _b) { return _a < _b ? _a : _b; }
template <typename T> const T& Min(const T& _a, const T& _b, const T& _c) { return _a < _b ? (_a < _c ? _a : _c) : (_b < _c ? _b : _c); }
template <typename T> const T& Max(const T& _a, const T& _b) { return _a > _b ? _a : _b; }
template <typename T> const T& Max(const T& _a, const T& _b, const T& _c) { return _a > _b ? (_a > _c ? _a : _c) : (_b > _c ? _b : _c); }
template <typename T> const T Clamp(T _x, T _l, T _u) { return _x > _l ? (_x < _u ? _x : _u) : _l; }
template <typename T> T Mix(const T& _a, const T& _b, float _t) { return _a + (_b - _a) * _t; }

inline float Radians(float _val) { return _val * RADIANS; }
inline float Degrees(float _val) { return _val * DEGREES; }
template <typename T> T Sqr(T _x) { return _x * _x; }
template <class T> T Abs(T _x) { return _x < 0 ? -_x : _x; }
float Abs(float _x);
float Sqrt(float _x);
float RSqrt(float _x);
float Sin(float _x);
float Cos(float _x);
void SinCos(float _a, float& _s, float& _c);
float Tan(float _x);
float ATan2(float _y, float _x);
float ASin(float _x);
float ACos(float _x);
float Log(float _x);
float Log2(float _x);
int Log2i(int _x);
float Pow(float _a, float _b);
float Exp(float _x);
float Floor(float _x);
float Ceil(float _x);
float Wrap(float _x, float _l, float _u);
float Cerp(float _a, float _b, float _t);

uint32 FirstPow2(uint32 _val);
bool IsPow2(uint32_t _val);

//TODO: FixedToFloat
//TODO: FloatToFixed
//TODO: FixedToFixed
uint8 FloatToByte(float _x);
float ByteToFloat(uint8 _x);
uint8 FloatToSByte(float _x);
float SByteToFloat(uint8 _x);
uint16 FloatToHalf(float _value);
float HalfToFloat(uint16 _value);
void Pack(float16* _dst, const float* _src, uint _num);
void Unpack(float* _dst, const float16* _src, uint _num);
void Pack(uint8* _dst, const float* _src, uint _num);
void Unpack(float* _dst, const uint8* _src, uint _num);
void Pack(int8* _dst, const float* _src, uint _num);
void Unpack(float* _dst, const int8* _src, uint _num);

//----------------------------------------------------------------------------//
// Vector2
//----------------------------------------------------------------------------//

struct Vector2
{
	Vector2(void) = default;
	Vector2(float _s) : x(_s), y(_s) { }
	Vector2(float _x, float _y) : x(_x), y(_y) { }

	Vector2 operator - (void) const { return Vector2(-x, -y); }
	Vector2 operator + (const Vector2& _rhs) const { return Vector2(x + _rhs.x, y + _rhs.y); }
	Vector2 operator - (const Vector2& _rhs) const { return Vector2(x - _rhs.x, y - _rhs.y); }
	Vector2 operator * (const Vector2& _rhs) const { return Vector2(x * _rhs.x, y * _rhs.y); }
	Vector2 operator / (const Vector2& _rhs) const { return Vector2(x / _rhs.x, y / _rhs.y); }
	Vector2 operator * (float _rhs) const { return Vector2(x * _rhs, y * _rhs); }
	Vector2 operator / (float _rhs) const { return Vector2(x / _rhs, y / _rhs); }
	Vector2& operator += (const Vector2& _rhs) { x += _rhs.x, y += _rhs.y; return *this; }
	Vector2& operator -= (const Vector2& _rhs) { x -= _rhs.x, y -= _rhs.y; return *this; }
	Vector2& operator *= (const Vector2& _rhs) { x *= _rhs.x, y *= _rhs.y; return *this; }
	Vector2& operator /= (const Vector2& _rhs) { x /= _rhs.x, y /= _rhs.y; return *this; }
	Vector2& operator *= (float _rhs) { x *= _rhs, y *= _rhs; return *this; }
	Vector2& operator /= (float _rhs) { x /= _rhs, y /= _rhs; return *this; }
	friend Vector2 operator / (float _lhs, const Vector2& _rhs) { return Vector2(_lhs / _rhs.x, _lhs / _rhs.y); }
	friend Vector2 operator * (float _lhs, const Vector2& _rhs) { return Vector2(_lhs * _rhs.x, _lhs * _rhs.y); }

	bool operator == (const Vector2& _rhs) const { return x == _rhs.x && y == _rhs.y; }
	bool operator != (const Vector2& _rhs) const { return x != _rhs.x || y != _rhs.y; }

	Vector2& Set(const Vector2& _v) { return *this = _v; }
	Vector2& Set(float _s) { x = _s, y = _s; return *this; }
	Vector2& Set(float _x, float _y) { x = _x, y = _y; return *this; }

	template <class T> Vector2& Unpack(const T* _src) { ::Unpack(v, _src, 2); return *this; }
	template <class T> Vector2& Pack(T* _dst) { ::Pack(_dst, v, 2); return *this; }

	float Dot(const Vector2& _v) const { return x * _v.x + y * _v.y; }
	float LengthSq(void) const { return Dot(*this); }

	union
	{
		struct { float x, y; };
		float v[2];
	};

	static const Vector2 Zero;
	static const Vector2 One;
};

//----------------------------------------------------------------------------//
// Vector2i
//----------------------------------------------------------------------------//

struct Vector2i
{
	Vector2i(void) = default;
	Vector2i(int _s) : x(_s), y(_s) { }
	Vector2i(int _x, int _y) : x(_x), y(_y) { }
	Vector2i(const Vector2& _v) : x((int)_v.x), y((int)_v.y) { }
	operator Vector2 (void) const { return Vector2((float)x, (float)y); }

	Vector2i operator - (void) const { return Vector2i(-x, -y); }
	Vector2i operator + (const Vector2i& _rhs) const { return Vector2i(x + _rhs.x, y + _rhs.y); }
	Vector2i operator - (const Vector2i& _rhs) const { return Vector2i(x - _rhs.x, y - _rhs.y); }
	Vector2i operator * (const Vector2i& _rhs) const { return Vector2i(x * _rhs.x, y * _rhs.y); }
	Vector2i operator / (const Vector2i& _rhs) const { return Vector2i(x / _rhs.x, y / _rhs.y); }
	Vector2i operator * (int _rhs) const { return Vector2i(x * _rhs, y * _rhs); }
	Vector2i operator / (int _rhs) const { return Vector2i(x / _rhs, y / _rhs); }
	Vector2i& operator += (const Vector2i& _rhs) { x += _rhs.x, y += _rhs.y; return *this; }
	Vector2i& operator -= (const Vector2i& _rhs) { x -= _rhs.x, y -= _rhs.y; return *this; }
	Vector2i& operator *= (const Vector2i& _rhs) { x *= _rhs.x, y *= _rhs.y; return *this; }
	Vector2i& operator /= (const Vector2i& _rhs) { x /= _rhs.x, y /= _rhs.y; return *this; }
	Vector2i& operator *= (int _rhs) { x *= _rhs, y *= _rhs; return *this; }
	Vector2i& operator /= (int _rhs) { x /= _rhs, y /= _rhs; return *this; }
	friend Vector2i operator / (int _lhs, const Vector2i& _rhs) { return Vector2i(_lhs / _rhs.x, _lhs / _rhs.y); }
	friend Vector2i operator * (int _lhs, const Vector2i& _rhs) { return Vector2i(_lhs * _rhs.x, _lhs * _rhs.y); }

	bool operator == (const Vector2i& _rhs) const { return x == _rhs.x && y == _rhs.y; }
	bool operator != (const Vector2i& _rhs) const { return x != _rhs.x || y != _rhs.y; }

	Vector2i& Set(const Vector2i& _v) { return *this = _v; }
	Vector2i& Set(int _s) { x = _s, y = _s; return *this; }
	Vector2i& Set(int _x, int _y) { x = _x, y = _y; return *this; }

	union
	{
		struct { int x, y; };
		int v[2];
	};
};

//----------------------------------------------------------------------------//
// Vector3
//----------------------------------------------------------------------------//

struct Vector3
{
	Vector3(void) = default;
	Vector3(float _s) : x(_s), y(_s), z(_s) { }
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
	Vector3 Copy(void) const { return *this; }

	const float operator [] (uint _index) const { return v[_index]; }
	float& operator [] (uint _index) { return v[_index]; }
	const float* operator * (void) const { return v; }
	float* operator * (void) { return v; }

	Vector3 operator - (void) const { return Vector3(-x, -y, -z); }
	Vector3 operator + (const Vector3& _rhs) const { return Vector3(x + _rhs.x, y + _rhs.y, z + _rhs.z); }
	Vector3 operator - (const Vector3& _rhs) const { return Vector3(x - _rhs.x, y - _rhs.y, z - _rhs.z); }
	Vector3 operator * (const Vector3& _rhs) const { return Vector3(x * _rhs.x, y * _rhs.y, z * _rhs.z); }
	Vector3 operator / (const Vector3& _rhs) const { return Vector3(x / _rhs.x, y / _rhs.y, z / _rhs.z); }
	Vector3 operator * (float _rhs) const { return Vector3(x * _rhs, y * _rhs, z * _rhs); }
	Vector3 operator / (float _rhs) const { return Vector3(x / _rhs, y / _rhs, z / _rhs); }
	Vector3& operator += (const Vector3& _rhs) { x += _rhs.x, y += _rhs.y, z += _rhs.z; return *this; }
	Vector3& operator -= (const Vector3& _rhs) { x -= _rhs.x, y -= _rhs.y, z -= _rhs.z; return *this; }
	Vector3& operator *= (const Vector3& _rhs) { x *= _rhs.x, y *= _rhs.y, z *= _rhs.z; return *this; }
	Vector3& operator /= (const Vector3& _rhs) { x /= _rhs.x, y /= _rhs.y, z /= _rhs.z; return *this; }
	Vector3& operator *= (float _rhs) { x *= _rhs, y *= _rhs, z *= _rhs; return *this; }
	Vector3& operator /= (float _rhs) { x /= _rhs, y /= _rhs, z /= _rhs; return *this; }
	friend Vector3 operator / (float _lhs, const Vector3& _rhs) { return Vector3(_lhs / _rhs.x, _lhs / _rhs.y, _lhs / _rhs.z); }
	friend Vector3 operator * (float _lhs, const Vector3& _rhs) { return Vector3(_lhs * _rhs.x, _lhs * _rhs.y, _lhs * _rhs.z); }

	bool operator == (const Vector3& _rhs) const { return x == _rhs.x && y == _rhs.y && z == _rhs.z; }
	bool operator != (const Vector3& _rhs) const { return !(*this == _rhs); }

	Vector3& Set(float _s) { x = _s, y = _s, z = _s; return *this; }
	Vector3& Set(float _x, float _y, float _z) { x = _x, y = _y, z = _z; return *this; }
	Vector3& Set(const Vector3& _other) { return *this = _other; }

	template <class T> Vector3& Unpack(const T* _src) { ::Unpack(v, _src, 3); return *this; }
	template <class T> Vector3& Pack(T* _dst) { ::Pack(_dst, v, 3); return *this; }

	Vector3& SetMin(const Vector3& _a, const Vector3& _b) { x = Min(_a.x, _b.x), y = Min(_a.y, _b.y), z = Min(_a.z, _b.z); return *this; }
	Vector3& SetMin(const Vector3& _other) { return SetMin(*this, _other); }
	Vector3& SetMax(const Vector3& _a, const Vector3& _b) { x = Max(_a.x, _b.x), y = Max(_a.y, _b.y), z = Max(_a.z, _b.z); return *this; }
	Vector3& SetMax(const Vector3& _other) { return SetMax(*this, _other); }
	Vector3& Normalize(void)
	{
		float _l = LengthSq();
		if (_l > EPSILON2)
			*this *= RSqrt(_l);
		return *this;
	}
	Vector3& NormalizeFast(void) { return *this *= LengthInv(); }
	float LengthSq(void) const { return x * x + y * y + z * z; }
	float Length(void) const { return Sqrt(x * x + y * y + z * z); }
	float LengthInv(void) const { return Sqrt(x * x + y * y + z * z); }
	float Dot(const Vector3& _rhs) const { return x * _rhs.x + y * _rhs.y + z * _rhs.z; }
	float AbsDot(const Vector3& _rhs) const { return Abs(x * _rhs.x) + Abs(y * _rhs.y) + Abs(z * _rhs.z); }
	Vector3 Cross(const Vector3& _rhs) const { return Vector3(y * _rhs.z - z * _rhs.y, z * _rhs.x - x * _rhs.z, x * _rhs.y - y * _rhs.x); }
	float Distance(const Vector3& _v) const { return (*this - _v).Length(); }
	float DistanceSq(const Vector3& _v) const { return (*this - _v).LengthSq(); }
	Vector3 MidPoint(const Vector3& _v) const { return (*this + _v) * 0.5f; }
	Vector3 Reflect(const Vector3& _n) const { return Vector3(*this - (2 * Dot(_n) * _n)); }
	Vector3 Perpendicular(void) const
	{
		Vector3 _perp = Cross({ 1, 0, 0 });
		if (_perp.LengthSq() <= EPSILON2)
			_perp = Cross({ 0, 1, 0 });
		return _perp.Normalize();
	}
	float Angle(const Vector3& _v) const
	{
		float _lp = LengthSq() * _v.LengthSq();
		if (_lp > EPSILON2)
			_lp = RSqrt(_lp);
		return ACos(Clamp<float>(Dot(_v) * _lp, -1, 1));
	}
	float AbsSum(void) const { return Abs(x) + Abs(y) + Abs(z); }

	bool operator < (const Vector3& _rhs) const { return x < _rhs.x && y < _rhs.y && z < _rhs.z; }
	bool operator <= (const Vector3& _rhs) const { return x <= _rhs.x && y <= _rhs.y && z <= _rhs.z; }
	bool operator > (const Vector3& _rhs) const { return x > _rhs.x && y > _rhs.y && z > _rhs.z; }
	bool operator >= (const Vector3& _rhs) const { return x >= _rhs.x && y >= _rhs.y && z >= _rhs.z; }

	union
	{
		struct { float x, y, z; };
		float v[3];
	};

	static const Vector3 Vector3::Zero;
	static const Vector3 Vector3::One;
	static const Vector3 Vector3::UnitX;
	static const Vector3 Vector3::UnitY;
	static const Vector3 Vector3::UnitZ;
};

//----------------------------------------------------------------------------//
// Vector4
//----------------------------------------------------------------------------//

struct Vector4
{
	Vector4(void) = default;
	Vector4(float _s) : x(_s), y(_s), z(_s), w(_s) { }
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }
	Vector4(const Vector3& _v, float _w = 1) : x(_v.x), y(_v.y), z(_v.z), w(_w) { }

	Vector4 Copy(void) const { return *this; }

	const float operator [] (uint _index) const { return v[_index]; }
	float& operator [] (uint _index) { return v[_index]; }
	const float* operator * (void) const { return v; }
	float* operator * (void) { return v; }

	operator Vector3& (void) { return *(Vector3*)v; }
	operator const Vector3& (void) const { return *(const Vector3*)v; }

	Vector4& Set(float _x, float _y, float _z, float _w = 1) { x = _x, y = _y, z = _z, w = _w; return *this; }
	Vector4& Set(float _s) { x = _s, y = _s, z = _s, w = _s; return *this; }
	Vector4& Set(const Vector3& _xyz, float _w = 1) { return Set(_xyz.x, _xyz.y, _xyz.z, _w); }
	Vector4& Set(const Vector4& _other) { return *this = _other; }

	template <class T> Vector4& Unpack(const T* _src) { ::Unpack(v, _src, 4); return *this; }
	template <class T> Vector4& Pack(T* _dst) { ::Pack(_dst, v, 4); return *this; }

	union
	{
		struct { float r, g, b, a; };
		struct { float x, y, z, w; };
		float v[4];
	};

	static const Vector4 Vector4::Zero;
	static const Vector4 Vector4::One;
	static const Vector4 Vector4::Identity;
};

//----------------------------------------------------------------------------//
// Quaternion
//----------------------------------------------------------------------------//

struct Quaternion
{
	Quaternion(void) = default;

	Quaternion(float _x, float _z, float _y, float _w) : x(_x), y(_y), z(_z), w(_w) { }
	Quaternion(float _w) : x(0), y(0), z(0), w(_w) { }
	Quaternion Copy(void) const { return *this; }

	Quaternion& Set(const Quaternion& _other) { return *this = _other; }
	Quaternion& Set(float _x, float _y, float _z, float _w) { x = _x, y = _y, z = _z, w = _w; return *this; }
	Quaternion& Set(float _w) { x = 0, y = 0, z = 0, w = _w; return *this; }

	const float operator [] (uint _index) const { return v[_index]; }
	float& operator [] (uint _index) { return v[_index]; }
	const float* operator * (void) const { return v; }
	float* operator * (void) { return v; }

	Quaternion operator - (void) const { return Quaternion(-x, -y, -z, -w); }

	Quaternion operator + (const Quaternion& _rhs) const { return Quaternion(x + _rhs.x, y + _rhs.y, z + _rhs.z, w + _rhs.w); }
	Quaternion operator - (const Quaternion& _rhs) const { return Quaternion(x - _rhs.x, y - _rhs.y, z - _rhs.z, w - _rhs.w); }
	Quaternion operator * (const Quaternion& _rhs) const { return Copy().Mul(_rhs); }
	Quaternion operator * (float _rhs) const { return Quaternion(x * _rhs, y * _rhs, z * _rhs, w * _rhs); }
	Quaternion operator / (float _rhs) const { return Quaternion(x / _rhs, y / _rhs, z / _rhs, w / _rhs); }
	Quaternion& operator += (const Quaternion& _rhs) { x += _rhs.x, y += _rhs.y, z += _rhs.z, w += _rhs.w; return *this; }
	Quaternion& operator -= (const Quaternion& _rhs) { x -= _rhs.x, y -= _rhs.y, z -= _rhs.z, w -= _rhs.w; return *this; }
	Quaternion& operator *= (const Quaternion& _rhs) { return Mul(_rhs); }
	Quaternion& operator *= (float _rhs) { x += _rhs, y += _rhs, z += _rhs, w += _rhs; return *this; }
	Quaternion& operator /= (float _rhs) { x += _rhs, y += _rhs, z += _rhs, w += _rhs; return *this; }
	friend Quaternion operator * (float _lhs, const Quaternion& _rhs) { return Quaternion(_lhs * _rhs.x, _lhs * _rhs.y, _lhs * _rhs.z, _lhs * _rhs.w); }
	friend Vector3 operator * (const Vector3& _lhs, const Quaternion& _rhs) { return _rhs.Transform(_lhs); }

	Quaternion& Mul(const Quaternion& _rhs);
	Vector3 Transform(const Vector3& _v) const;
	float Dot(const Quaternion& _q) const { return x * _q.x + y * _q.y + z * _q.z + w * _q.w; }
	Quaternion& Normalize(void);
	Quaternion& Inverse(void);
	Quaternion& UnitInverse(void) { x = -x, y = -y, z = -z; return *this; }
	Quaternion Nlerp(const Quaternion& _q, float _t, bool _shortestPath = true) const;
	Quaternion Slerp(const Quaternion& _q, float _t, bool _shortestPath = true) const;
	void ToRotationMatrix(float* _r0, float* _r1, float* _r2) const;
	Quaternion& FromRotationMatrix(const float* _r0, const float* _r1, const float* _r2);
	Quaternion& FromAxisAngle(const Vector3& _axis, float _angle);


	Quaternion& FromLookRotation(const Vector3& _dir, const Vector3& _up = Vector3::UnitY)
	{
		Vector3 _z = _dir.Copy().Normalize();
		Vector3 _y = _z.Cross(_up).Copy().Normalize().Cross(_z);
		Vector3 _x = _y.Cross(_z);
		return FromRotationMatrix(*_x, *_y, *_z);
	}

	Quaternion& FromRotationTo(const Vector3& _start, const Vector3& _end)
	{
		Vector3 _ns = _start.Copy().Normalize();
		Vector3 _ne = _end.Copy().Normalize();
		float d = _ns.Dot(_ne);

		if (d > -1 + EPSILON)
		{
			Vector3 _c = _ns.Cross(_ne);
			float _s = Sqrt((1 + d) * 2);
			float _invS = 1 / _s;
			x = _c.x * _invS;
			y = _c.y * _invS;
			z = _c.z * _invS;
			w = 0.5f * _s;
		}
		else
		{
			Vector3 _axis = Vector3::UnitX.Cross(_ns);
			if (_axis.LengthSq() < EPSILON2)
				_axis = Vector3::UnitY.Cross(_ns);

			FromAxisAngle(_axis, HALF_PI);
		}
		return *this;
	}


	union
	{
		struct { float x, y, z, w; };
		float v[4];
	};

	static const Quaternion Quaternion::Zero;
	static const Quaternion Quaternion::Identity;
};

inline Quaternion Mix(const Quaternion& _a, const Quaternion& _b, float _t)
{
	return _a.Slerp(_b, _t);
}

//----------------------------------------------------------------------------//
// Matrix24
//----------------------------------------------------------------------------//

/// Row-major matrix 2x4 (for OpenGL 2x2, 2x3 and 2x4 matrices in uniform blocks)
struct Matrix24
{
	Vector4 r[2];
};

//----------------------------------------------------------------------------//
// Matrix34
//----------------------------------------------------------------------------//

/// Row-major matrix 3x4 (for OpenGL 3x3 and 3x4 matrices in uniform blocks)
struct Matrix34
{
	Vector4 r[3];
};

//----------------------------------------------------------------------------//
// Matrix44
//----------------------------------------------------------------------------//

/// Row-major matrix 4x4
struct Matrix44
{
	Matrix44(void) { }
	explicit Matrix44(float _val) { SetIdentity(_val); }
	explicit Matrix44(const float* _m44) { FromPtr(_m44); }
	Matrix44(float _00, float _01, float _02, float _03, float _10, float _11, float _12, float _13, float _20, float _21, float _22, float _23, float _30, float _31, float _32, float _33);

	operator Matrix34& (void) { return *(Matrix34*)v; }
	operator const Matrix34& (void) const { return *(Matrix34*)v; }

	Matrix44 Copy(void) const { return *this; }
	Matrix44& FromPtr(const float* _m44);
	Matrix44& Set(const Matrix44& _other) { return *this = _other; }
	Matrix44& Set(float _00, float _01, float _02, float _03, float _10, float _11, float _12, float _13, float _20, float _21, float _22, float _23, float _30, float _31, float _32, float _33);
	Matrix44& SetDiagonal(float _val) { m00 = _val, m11 = _val, m22 = _val, m33 = _val; return *this; }
	Matrix44& SetZero(void) { for (uint i = 0; i < 16; ++i) v[i] = 0; return *this; }
	Matrix44& SetIdentity(float _val = 1) { return SetZero().SetDiagonal(_val); }

	Vector3& Row(uint _r) { return *(Vector3*)(m[_r]); }
	const Vector3& Row(uint _r) const { return *(Vector3*)(m[_r]); }

	float* operator [] (uint _r) { return m[_r]; }
	const float* operator [] (uint _r) const { return m[_r]; }
	const float* operator * (void) const { return v; }
	float* operator * (void) { return v; }

	Vector3 Translate(const Vector3& _v) const { return Vector3(_v.x + m03, _v.y + m13, _v.z + m23); }
	Vector3 Transform(const Vector3& _v) const;
	Vector3 TransformVectorAbs(const Vector3& _v) const;
	Vector3 TransformVector(const Vector3& _v) const;
	Vector3 TransformProj(const Vector3& _v) const;
	Vector4 Transform(const Vector4& _v) const;
	friend Vector3 operator * (const Vector3&_lhs, const Matrix44& _rhs) { return _rhs.TransformProj(_lhs); }
	friend Vector4 operator * (const Vector4&_lhs, const Matrix44& _rhs) { return _rhs.Transform(_lhs); }

	Matrix44& Add(const Matrix44& _rhs) { for (uint i = 0; i < 16; ++i) v[i] += _rhs.v[i]; return *this; }
	Matrix44 operator + (const Matrix44& _rhs) const { return Copy().Add(_rhs); }
	Matrix44& operator += (const Matrix44& _rhs) { return Add(_rhs); }

	Matrix44& Mul(float _rhs) { for (uint i = 0; i < 16; ++i) v[i] *= _rhs; return *this; }
	Matrix44 operator * (float _rhs) const { return Copy().Mul(_rhs); }
	Matrix44& operator *= (float _rhs) { return Mul(_rhs); }

	Matrix44& Mul(const Matrix44& _rhs);
	Matrix44 operator * (const Matrix44& _rhs) const { return Copy().Mul(_rhs); }
	Matrix44& operator *= (const Matrix44& _rhs) { return Mul(_rhs); }

	bool IsAffine(void) const { return m30 == 0 && m31 == 0 && m32 == 0 && m33 == 1; }

	Vector3 GetTranslation(void) const { return Vector3(m03, m13, m23); }
	Matrix44& SetTranslation(const Vector3& _t) { m03 = _t.x, m13 = _t.y, m23 = _t.z; return *this; }
	Matrix44& CreateTranslation(const Vector3& _t) { return SetIdentity().SetTranslation(_t); }

	Vector3 GetScale(void) const;
	Matrix44& SetScale(const Vector3& _s) { return SetRotationScale(GetRotation(), _s); }
	Matrix44& CreateScale(const Vector3& _s);

	Quaternion GetRotation(void) const;
	Matrix44& SetRotation(const Quaternion& _r) { return SetRotationScale(_r, GetScale()); }
	Matrix44& CreateRotation(const Quaternion& _r) { return SetIdentity().SetRotationScale(_r, 1); }

	Matrix44& SetRotationScale(const Quaternion& _r, const Vector3& _s);
	Matrix44& CreateRotationScale(const Quaternion& _r, const Vector3& _s) { return SetIdentity().SetRotationScale(_r, _s); }
	Matrix44& CreateTransform(const Vector3& _t, const Quaternion& _r, const Vector3& _s = 1);
	Matrix44& CreateInverseTransform(const Vector3& _t, const Quaternion& _r, const Vector3& _s = 1);

	Matrix44& Inverse(void);
	Matrix44& Transpose(void);

	/*Matrix44& CreateProjection(float _left, float _right, float _bottom, float _top, float _znear, float _zfar, uint _flags)
	{
	return *this;
	}*/

	/*Matrix44& CreateLookAt()
	{

	}*/

	Matrix44& CreatePerspective(float _fov, float _aspect, float _near, float _far);
	Matrix44& CreateOrtho(float _left, float _right, float _bottom, float _top, float _znear, float _zfar);
	Matrix44& CreateOrtho2D(float _width, float _height);

	union
	{
		float m[4][4]; // [row][column]
		float v[16];
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
	};

	static const Matrix44 Matrix44::Zero;
	static const Matrix44 Matrix44::Identity;
};

//----------------------------------------------------------------------------//
// Ray 
//----------------------------------------------------------------------------//

struct Ray
{
	Ray(void) : origin(Vector3::Zero), dir(Vector3::UnitZ) { }
	Ray(const Vector3& _origin, const Vector3& _dir) : origin(_origin), dir(_dir) { }
	Vector3 Point(float _t) const { return origin + dir * _t; }
	Vector3 operator * (float _t) const { return Point(_t); }
	friend Vector3 operator * (float _t, const Ray& _ray) { return _ray.Point(_t); }

	Vector3 origin, dir;
};

//----------------------------------------------------------------------------//
// Triangle
//----------------------------------------------------------------------------//

struct Triangle
{
	// TODO

	static Vector3 Normal(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2) { return (_v1 - _v0).Cross(_v2 - _v0); }

	static bool Intersects(const Vector3& _t0, const Vector3& _t1, const Vector3& _t2, const Ray& _ray, float* _dist = nullptr);
};


//----------------------------------------------------------------------------//
// Plane
//----------------------------------------------------------------------------//

struct Plane
{
	Plane(void) : normal(Vector3::Zero), dist(0) { }
	Plane(float _nx, float _ny, float _nz, float _d) : normal(_nx, _ny, _nz), dist(_d) { }
	Plane(const Vector3& _n, float _d) : normal(_n), dist(_d) { }

	Plane Copy(void) const { return *this; }

	Plane& Set(const Plane& _other) { return *this = _other; }
	Plane& Set(const Vector4& _v) { return Set(_v.x, _v.y, _v.z, Abs(_v.w)); }
	Plane& Set(float _nx, float _ny, float _nz, float _d) { normal.Set(_nx, _ny, _nz); dist = _d; return *this; }
	Plane& Set(const Vector3& _normal, float _distance) { normal = _normal, dist = _distance; return *this; }

	Plane& FromNormalPoint(const Vector3& _normal, const Vector3& _point) { return Set(_normal, -_normal.AbsDot(_point)); }
	Plane& FromTriangle(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2) { return FromNormalPoint(Triangle::Normal(_v0, _v1, _v2), _v0); }

	Vector4& AsVec4(void) { return *(Vector4*)&normal; }
	const Vector4& AsVec4(void) const { return *(const Vector4*)&normal; }

	Plane& Normalize(void);
	Plane& Transform(const Matrix44& _m) { return Set(AsVec4() * _m.Copy().Inverse().Transpose()); }

	Plane operator * (const Matrix44& _m) const { return Copy().Transform(_m); }
	Plane& operator *= (const Matrix44& _m) { return Transform(_m); }

	float Distance(const Vector3& _point) const { return normal.Dot(_point) + dist; }
	float Distance(const Vector3& _center, float _radius) const;
	float Distance(const Ray& _ray) const;
	Vector3 Point(const Ray& _ray)	const { return _ray * Distance(_ray); }

	Vector3 Reflect(const Vector3& _dir) const { return _dir - (2.f * normal.Dot(_dir) * normal); }
	Matrix44 GetReflectionMatrix(void) const;

	Vector3 normal;
	float dist;
};

//----------------------------------------------------------------------------//
// Box
//----------------------------------------------------------------------------//

struct Box
{
	enum Corner : uint8
	{
		LeftBottomFar = 0,// min
		RightBottomFar,
		RightBottomNear,
		LeftBottomNear,
		RightTopNear, // max
		LeftTopNear,
		LeftTopFar,
		RightTopFar,
	};

	static const uint16 LineIndices[24];
	static const uint16 QuadIndices[24];
	static const uint16 TriangleIndices[36];
};

//----------------------------------------------------------------------------//
// AlignedBox
//----------------------------------------------------------------------------//

struct AlignedBox
{
	AlignedBox(void) : mn(999999.9f), mx(-999999.9f) { }
	AlignedBox(const Vector3& _min, const Vector3& _max) : mn(_min), mx(_max) { }

	AlignedBox Copy(void) const { return *this; }

	AlignedBox& Set(const AlignedBox& _b) { return *this = _b; }
	AlignedBox& Set(const Vector3& _min, const Vector3& _max) { mn.Set(_min), mx.Set(_max); return *this; }
	AlignedBox& SetMinMax(const Vector3& _a, const Vector3& _b) { mn.SetMin(_a, _b), mx.SetMax(_a, _b); return *this; }
	AlignedBox& SetZero(void) { mn = Vector3::Zero, mx = Vector3::Zero; return *this; }
	AlignedBox& FromCenterExtends(const Vector3& _center, const Vector3& _extends) { return Set(_center - _extends, _center + _extends); }
	AlignedBox& FromViewProjMatrix(const Matrix44& _m) { return *this = AlignedBox(-1, 1).TransformProj(_m); }

	AlignedBox& Reset(const Vector3& _pt) { return Set(_pt, _pt); }
	AlignedBox& Reset(void) { mn = 999999.9f, mx = -999999.9f; return *this; }
	AlignedBox& AddPoint(const Vector3& _pt) { mn.SetMin(_pt), mx.SetMax(_pt); return *this; }
	AlignedBox& AddVertices(const void* _src, uint _count, size_t _stride = 0, size_t _offset = 0);

	bool IsZero(void) const { return mn == mx && mn == Vector3::Zero; }
	bool IsFinite(void) const { return mn.x <= mx.x && mn.y <= mx.y && mn.z <= mx.z; }

	Vector3 Size(void) const { return mx - mn; }
	Vector3 Extends(void) const { return (mx - mn) * 0.5f; }
	Vector3 Center(void) const { return (mx + mn) * 0.5f; }
	//operator Sphere (void) const { return Sphere(Center(), Radius()); }
	float Diagonal(void) const { return (mx - mn).Length(); }
	float DiagonalSq(void) const { return (mx - mn).LengthSq(); }
	float Radius(void) const { return Diagonal() * 0.5f; }
	float Width(void) const { return mx.x - mn.x; }
	float Height(void) const { return mx.y - mn.y; }
	float Depth(void) const { return mx.z - mn.z; }
	float Volume(void) const { return (mx - mn).LengthSq(); }
	void GetAllCorners(const void* _dst, size_t _stride = 0, size_t _offset = 0) const;

	// for dbvt
	float Proximity(const AlignedBox& _other) const { return ((mn + mx) - (_other.mn + _other.mx)).AbsSum(); }
	int Select(const AlignedBox& _a, const AlignedBox& _b) const { return Proximity(_a) < Proximity(_b) ? 0 : 1; }

	AlignedBox TransformAffine(const Matrix44& _rhs) { return FromCenterExtends(Center() * _rhs, _rhs.TransformVectorAbs(Extends())); }
	AlignedBox TransformProj(const Matrix44& _rhs);

	AlignedBox operator + (const Vector3& _point) const { return Copy().AddPoint(_point); }
	AlignedBox operator + (const AlignedBox& _box) const { return Copy().AddPoint(_box.mn).AddPoint(_box.mx); }
	AlignedBox& operator += (const Vector3& _point) { return AddPoint(_point); }
	AlignedBox& operator += (const AlignedBox& _box) { return AddPoint(_box.mn).AddPoint(_box.mx); }
	AlignedBox& operator *= (const Matrix44& _rhs) { return TransformAffine(_rhs); }
	AlignedBox operator * (const Matrix44& _rhs) const { return Copy().TransformAffine(_rhs); }

	bool operator == (const AlignedBox& _rhs) const { return mn == _rhs.mn && mx == _rhs.mx; }
	bool operator != (const AlignedBox& _rhs) const { return !(*this == _rhs); }

	bool Contains(const Vector3& _point) const { return _point >= mn && _point <= mx; }
	bool Contains(const AlignedBox& _box) const { return _box.mn >= mn && _box.mx <= mx; }
	bool Intersects(const AlignedBox& _box) const { return !(_box.mx < mn || _box.mn > mx); }

	Vector3 mn, mx;
};

//----------------------------------------------------------------------------//
// OrientedBox
//----------------------------------------------------------------------------//

struct OrientedBox
{

};

//----------------------------------------------------------------------------//
// Sphere
//----------------------------------------------------------------------------//

struct Sphere
{

};

//----------------------------------------------------------------------------//
// Frustum
//----------------------------------------------------------------------------//

enum FrustumPlane : uint16
{
	FP_Left,
	FP_Right,
	FP_Bottom,
	FP_Top,
	FP_Near,
	FP_Far,
};

struct Frustum
{
	Frustum(void) { }

	Frustum& FromCameraMatrices(const Matrix44& _view, const Matrix44& _proj);

	bool Intersects(const Vector3& _point) const;
	bool Intersects(const Vector3& _center, float _radius) const;
	bool Intersects(const AlignedBox& _box, bool* _contains = nullptr) const;
	bool Intersects(const Frustum& _frustum, bool* _contains = nullptr) const;

	float Distance(const Vector3& _point) const { return origin.Distance(_point); }
	float Distance(const Vector3& _center, float _radius) const { float _d = origin.Distance(_center); return _d < _radius ? 0 : _d - _radius; }
	float Distance(const AlignedBox& _box) const { return Distance(_box.Center(), _box.Radius()); }


	static void GetPlanes(Plane* _planes, const Matrix44& _m);

	Plane planes[6];
	Vector3 corners[8];
	Vector3 origin;
	AlignedBox box;
};

//----------------------------------------------------------------------------//
// DbvtNode
//----------------------------------------------------------------------------//

struct DbvtNode
{
	DbvtNode(void) : child0(nullptr), child1(nullptr) { }

	bool IsLeaf(void) const { return child[1] == 0; }
	bool IsNode(void) const { return child[1] != 0; }
	bool IsValidLeaf(void) const { return IsLeaf() && object && box.IsFinite(); }

	AlignedBox box;
	DbvtNode* parent = nullptr;
	union
	{
		DbvtNode* child[2];
		struct
		{
			DbvtNode* child0;
			DbvtNode* child1;
		};
		void* object;
		int value;
	};

#ifdef _DEBUG
	AlignedBox _GetChildBounds(void)
	{
		if (IsLeaf())
			return box;
		return child0->_GetChildBounds() + child1->_GetChildBounds();
	}
#endif
};

//----------------------------------------------------------------------------//
// DbvtCallback
//----------------------------------------------------------------------------//

struct DbvtCallback
{
	enum TestResult
	{
		TR_Stop = 0, // stop callback
		TR_Skip, // skip node
		TR_WithTest, // add all leafs in node with test
		TR_WithoutTest, // add all leafs in node without test
	};

	virtual TestResult TestNode(DbvtNode* _node) { return TR_WithoutTest; }
	virtual void AddLeaf(DbvtNode* _leaf, TestResult _testResult) { }
};

//----------------------------------------------------------------------------//
// Dbvt
//----------------------------------------------------------------------------//

//! Dynamic Bounding-Volumes Tree (BVH)
class Dbvt : public NonCopyable
{
public:
	typedef DbvtNode Node;
	typedef DbvtCallback Callback;

	Dbvt(void) = default;
	~Dbvt(void) { _Clear(); }

	void Insert(Node* _leaf);
	void Remove(Node* _leaf);
	void Update(Node* _leaf);

	Node* Root(void) { return m_root; }
	void Enum(Callback* _cb, bool _withTest = true);

	// TODO: enum directional

	uint GetMaxDepth(void);

protected:
	void _Insert(Node* _leaf, Node* _root);
	Node* _Remove(Node* _leaf);
	void _Delete(Node* _node);
	void _Clear(void);

	Node* m_root = nullptr;
	Node* m_free = nullptr; // last deleted node
};

//----------------------------------------------------------------------------//
// Dbvt utils
//----------------------------------------------------------------------------//

struct DbvtContainer
{
	virtual void AddObject(void* _object, const AlignedBox& _bbox, bool _withTest) { }

	bool stop = false; // if true, callback will be stopped
};

struct DbvtCallbackContainer : DbvtCallback
{
	//DbvtCallbackContainer(DbvtContainer* _container = nullptr) : container(_container) { }

	void AddLeaf(DbvtNode* _leaf, TestResult _testResult) override
	{
		ASSERT(container != nullptr);
		ASSERT(_leaf != nullptr);
		container->AddObject(_leaf->object, _leaf->box, _testResult == TR_WithTest);
	}

	DbvtContainer* container = nullptr;
};

struct DbvtFrustumCallback : DbvtCallbackContainer
{

	TestResult TestNode(DbvtNode* _node) override
	{
		if (container->stop)
			return TR_Stop;

		bool _contains;
		if (volume.Intersects(_node->box, &_contains))
			return _contains ? TR_WithoutTest : TR_WithTest;
		return TR_Skip;
	}

	Frustum volume;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
