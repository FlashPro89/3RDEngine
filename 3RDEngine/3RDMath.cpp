#include "../include/3RDMath.h"
#include <string>

const float _m_id_Set[] = { 1.f, 0.f, 0.f, 0.f,
							0.f, 1.f, 0.f, 1.f,
							0.f, 0.f, 1.f, 0.f,
							0.f, 0.f, 0.f, 1.f };

const auto _m4_sz = 16 * sizeof(float);
const auto _q4_sz = 4 * sizeof(float);
const auto _v4_sz = 4 * sizeof(float);
const auto _v3_sz = 3 * sizeof(float);
const auto _v2_sz = 2 * sizeof(float);


// ------------------------------------
//
//		*** struct gVector4 ***
//
// ------------------------------------

gVector4::gVector4()
{
	x = y = z = 0.f;
}

gVector4::gVector4( float value )
{
	x = y = z = w = value;
}

gVector4::gVector4(float _x, float _y, float _z, float _w)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->w = _w;
}

gVector4::gVector4( const gVector4& other )
{ 
	x = other.x; y = other.y;  z = other.z; w = other.w; 
}

void gVector4::operator = (const gVector4& other)
{
	memcpy_s( _v, _v4_sz, other._v, _v4_sz );
}

void gVector4::operator += (const gVector4& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
}

void gVector4::operator -= (const gVector4& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
}

void gVector4::operator *= (float value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;
}

float gVector4::operator * (const gVector4& other) // dot product of two vecs
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

// ------------------------------------
//
//		*** struct gVector3 ***
//
// ------------------------------------

gVector3::gVector3()
{
	x = y = z = 0.f;
}

gVector3::gVector3( float value )
{
	x = y = z = value;
}

gVector3::gVector3(float _x, float _y, float _z)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
}

gVector3::gVector3( const gVector3& other )
{ 
	x = other.x; y = other.y;  z = other.z; 
}

void gVector3::operator = (const gVector3& other)
{
	memcpy_s(_v, _v3_sz, other._v, _v3_sz);
}

void gVector3::operator += (const gVector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

void gVector3::operator -= (const gVector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void gVector3::operator *= (float value)
{
	x *= value;
	y *= value;
	z *= value;
}

void gVector3::operator ^= (const gVector3& other) // cross product with other vec
{
	gVector3 tmp(*this);
	x = tmp.y * other.z - tmp.z * other.y;
	y = tmp.z * other.x - tmp.x * other.z;
	z = tmp.x * other.y - tmp.y * other.x;
}

float gVector3::operator * (const gVector3& other) // dot product of two vecs
{
	return x * other.x + y * other.y + z * other.z;
}

const gVector3 gVector3::operator ^ (const gVector3& other) // dot product of two vecs
{
	gVector3 cross;
	cross.x = y * other.z - z * other.y;
	cross.y = z * other.x - x * other.z;
	cross.z = x * other.y - y * other.x;

	return cross;
}
// ------------------------------------
//
//		*** class gMatrix4 ***
//
// ------------------------------------

gMatrix4::gMatrix4() { identity(); }
gMatrix4::gMatrix4(bool isIdentity) { if (isIdentity) identity(); }
gMatrix4::gMatrix4(const gMatrix4& other) 
{ 
	auto sz = _countof(_p) * sizeof(float);
	memcpy_s(_p, sz, other._p, sz);
}

void gMatrix4::identity()
{
	memcpy_s( _p, _m4_sz, _m_id_Set, _m4_sz );
}

float gMatrix4::determinant()
{
	float determinant = _14 * _23 * _32 * _41 - _13 * _24 * _32 * _41
		- _14 * _22 * _33 * _41 + _12 * _24 * _33 * _41
		+ _13 * _22 * _34 * _41 - _12 * _23 * _34 * _41
		- _14 * _23 * _31 * _42 + _13 * _24 * _31 * _42
		+ _14 * _21 * _33 * _42 - _11 * _24 * _33 * _42
		- _13 * _21 * _34 * _42 + _11 * _23 * _34 * _42
		+ _14 * _22 * _31 * _43 - _12 * _24 * _31 * _43
		- _14 * _21 * _32 * _43 + _11 * _24 * _32 * _43
		+ _12 * _21 * _34 * _43 - _11 * _22 * _34 * _43
		- _13 * _22 * _31 * _44 + _12 * _23 * _31 * _44
		+ _13 * _21 * _32 * _44 - _11 * _23 * _32 * _44
		- _12 * _21 * _33 * _44 + _11 * _22 * _33 * _44;

	return determinant;
}

const gMatrix4& gMatrix4::transpose()
{
	gMatrix4 t = *this;

	_12 = t._21;
	_13 = t._31;
	_14 = t._41;

	_21 = t._12;
	_23 = t._32;
	_24 = t._42;

	_31 = t._13;
	_32 = t._23;
	_34 = t._43;

	_41 = t._14;
	_42 = t._24;
	_43 = t._34;

	return *this;
}

void gMatrix4::translation( float x, float y, float z )
{
	identity();
	_v4 = gVector4( x, y, z, 1.f );
}

void gMatrix4::translation(const gVector4& v)  // use first 3 dim
{
	identity();
	_v4 = gVector4(v.x, v.y, v.z, 1.f);
}

void gMatrix4::operator = (const gMatrix4& other)
{
	auto sz = _countof(_p) * sizeof(float);
	memcpy_s(_p, sz, other._p, sz);
}
