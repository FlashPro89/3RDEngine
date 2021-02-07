#include "../include/3RDMath.h"
#include <math.h>
#include <string>

//
constexpr float _m_id_[] = {	1.f, 0.f, 0.f, 0.f,
							0.f, 1.f, 0.f, 1.f,
							0.f, 0.f, 1.f, 0.f,
							0.f, 0.f, 0.f, 1.f		};

const size_t gPlane::Size = sizeof(gPlane);
const size_t gMatrix4::Size = sizeof(gMatrix4);
const size_t gQuaternion::Size = sizeof(gQuaternion);
const size_t gVector4::Size = sizeof(gVector4);
const size_t gVector3::Size = sizeof(gVector3);
const size_t gVector2::Size = sizeof(gVector2);
const gMatrix4 gMatrix4::Identity = gMatrix4( (const float*)_m_id_ );
const gQuaternion gQuaternion::Identity = gQuaternion( 0.f, 0.f, 0.f, 1.f );

const gVector2 gVector2::Up(0.f, 1.f);
const gVector2 gVector2::Right(1.f, 0.f);

const gVector3 gVector3::Right(1.f, 0.f, 0.f);
const gVector3 gVector3::Up(0.f, 1.f, 0.f);
const gVector3 gVector3::Front(0.f, 0.f, 1.f);

const gVector4 gVector4::Right(1.f, 0.f, 0.f, 1.f);
const gVector4 gVector4::Up(0.f, 1.f, 0.f, 1.f);
const gVector4 gVector4::Front(0.f, 0.f, 1.f, 1.f);

// ------------------------------------
//
//		*** struct gVector4 ***
//
// ------------------------------------

gVector4::gVector4()
{
	//x = y = z = 0.f;
}

gVector4::gVector4(const float* p)
{
	x = p[0];
	y = p[1];
	z = p[2];
	w = p[3];
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
	memcpy_s( _v, gVector4::Size, other._v, gVector4::Size );
}

void gVector4::normalize()
{
	float l = lenght();
	(*this) /= l;
}

float gVector4::lenght()  const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

float gVector4::sqLenght()  const
{
	return x * x + y * y + z * z + w * w;
}


void gVector4::set(float value)
{
	x = y = z = w = value;
}

void gVector4::set(float _x, float _y, float _z, float _w)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->w = _w;
}

gVector4 gVector4::operator + ()  const
{
	return gVector4(x, y, z, w);
}

gVector4 gVector4::operator - ()  const
{
	return gVector4(-x, -y, -z, -w);
}

gVector4 gVector4::operator + (const gVector4& other)  const
{
	gVector4 t( x + other.x, y + other.y, z + other.z, w + other.w );
	return t;
}

gVector4 gVector4::operator - (const gVector4& other)  const
{
	gVector4 t(x - other.x, y - other.y, z - other.z, w - other.w);
	return t;
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

void gVector4::operator /= (float value)
{
	if (value == 0)
		return;

	x /= value;
	y /= value;
	z /= value;
	w /= value;
}

float gVector4::operator * (const gVector4& other)  const // dot product of two vecs
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

gVector4 gVector4::operator * (float value) const // scale vec
{
	gVector4 result = *this;
	result *= value;
	return result;
}

// ------------------------------------
//
//		*** struct gVector3 ***
//
// ------------------------------------

gVector3::gVector3()
{
	//x = y = z = 0.f;
}

gVector3::gVector3( const float* p )
{
	x = p[0];
	y = p[1];
	z = p[2];
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

void gVector3::normalize()
{
	float l = lenght();
	(*this) /= l;
}

float gVector3::lenght() const
{
	return sqrtf(x * x + y * y + z * z);
}

float gVector3::sqLenght() const
{
	return x * x + y * y + z * z;
}

float  gVector3::distanceTo(const gVector3& other) const
{
	gVector3 dst = other - *this;
	return dst.lenght();
}

void gVector3::set(float value)
{
	x = y = z = value;
}

void gVector3::set(float _x, float _y, float _z)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
}

void gVector3::operator = (const gVector3& other)
{
	x = other.x; y = other.y; z = other.z;
}

gVector3 gVector3::operator + () const
{
	return gVector3(x, y, z);
}

gVector3 gVector3::operator - () const
{
	return gVector3(-x, -y, -z);
}

gVector3 gVector3::operator + (const gVector3& other) const
{
	gVector3 t( x + other.x, y + other.y, z + other.z );
	return t;
}

gVector3 gVector3::operator - (const gVector3& other) const
{
	gVector3 t(x - other.x, y - other.y, z - other.z);
	return t;
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

void gVector3::operator /= (float value)
{
	if (value == 0)
		return;

	x /= value;
	y /= value;
	z /= value;
}

void gVector3::operator ^= (const gVector3& other) // cross product with other vec
{
	gVector3 tmp(*this);
	x = tmp.y * other.z - tmp.z * other.y;
	y = tmp.z * other.x - tmp.x * other.z;
	z = tmp.x * other.y - tmp.y * other.x;
}

float gVector3::operator * (const gVector3& other) const // dot product of two vecs
{
	return x * other.x + y * other.y + z * other.z;
}

gVector3 gVector3::operator * (float value) const // scale vec
{
	gVector3 result = *this;
	result *= value;
	return result;
}

gVector3 gVector3::operator ^ (const gVector3& other) const // dot product of two vecs
{
	gVector3 cross;
	cross.x = y * other.z - z * other.y;
	cross.y = z * other.x - x * other.z;
	cross.z = x * other.y - y * other.x;

	return cross;
}


// ------------------------------------
//
//		*** class gVector2 ***
//
// ------------------------------------

gVector2::gVector2()
{
	//x = y = 0.f;
}

gVector2::gVector2( const float* p )
{
	x = p[0];
	y = p[1];
}

gVector2::gVector2(float value)
{
	x = y = value;
}

gVector2::gVector2(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
}

gVector2::gVector2(const gVector2& other)
{
	x = other.x; y = other.y;
}

void gVector2::normalize()
{
	float l = lenght();
	(*this) /= l;
}

float gVector2::lenght() const
{
	return  sqrtf(x * x + y * y);
}

float gVector2::sqLenght() const
{
	return  x * x + y * y;
}

void gVector2::set(float value)
{
	x = y = value;
}

void gVector2::set(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
}

void gVector2::operator = (const gVector2& other)
{
	x = other.x; y = other.y;
}

gVector2 gVector2::operator + () const
{
	return gVector2(x, y);
}

gVector2 gVector2::operator - () const
{
	return gVector2( -x, -y );
}

gVector2 gVector2::operator + (const gVector2& other) const
{
	return gVector2 (x + other.x, y + other.y);
}

gVector2 gVector2::operator - (const gVector2& other) const
{
	return gVector2 (x - other.x, y - other.y);
}

void gVector2::operator += (const gVector2& other)
{
	x += other.x;
	y += other.y;
}

void gVector2::operator -= (const gVector2& other)
{
	x -= other.x;
	y -= other.y;
}

void gVector2::operator *= (float value)
{
	x *= value;
	y *= value;
}

void gVector2::operator /= (float value)
{
	if (value == 0)
		return;

	x /= value;
	y /= value;
}

float gVector2::operator * (const gVector2& other) const // dot product of two vecs
{
	return x * other.x + y * other.y;
}

gVector2 gVector2::operator * (float value) const // scale vec
{
	gVector2 result = *this;
	result *= value;
	return result;
}

// ------------------------------------
//
//		*** class gPlane ***
//
// ------------------------------------

gPlane::gPlane()
{
	//a = b = c = d = 0.f; // надо ли инициализировать?
}

gPlane::gPlane(float _a, float _b, float _c, float _d)
{
	a = _a; b = _b; c = _c; d = _d;
}

gPlane::gPlane(const gVector3& p0, const gVector3& p1, const gVector3& p2)
{
	fromPoints( p0, p1, p2 );
}

void gPlane::fromEquationConstants(float _a, float _b, float _c, float _d)
{
	a = _a; b = _b; c = _c; d = _d;
}

void gPlane::fromPointNormal(const gVector3& _p, const gVector3& _n)
{
	this->n = _n;
	n.normalize();
	this->d = -( _p * n );
}

void gPlane::fromPointNormalizedNormal(const gVector3& _p, const gVector3& _n)
{
	this->n = _n;
	this->d = -(_p * n);
}

void gPlane::fromPointVectors(const gVector3& p, const gVector3& v0, const gVector3& v1)
{
	gVector3 normal = v0 ^ v1;
	fromPointNormal( p, normal );
}

void gPlane::fromPoints(const gVector3& p0, const gVector3& p1, const gVector3& p2)
{
	gVector3 normal = (p1-p0) ^ (p2-p0);
	fromPointNormal( p0, normal );
}

void gPlane::normalize()
{
	/*
	Plane Result;
	float Distance = sqrtf(a * a + b * b + c * c);
	Result.a = a / Distance;
	Result.b = b / Distance;
	Result.c = c / Distance;
	Result.d = d / Distance;
	return Result; */
	float l = n.lenght();
	if (l == 0) return;
	a /= l; b /= l; c /= l; d /= l;
}

gVector3  gPlane::projectPointToPlane( const gVector3& p) const
{
	float dist = a * p.x + b * p.y + c * p.z + d;
	gVector3 vToPlane = (n * dist);
	vToPlane = p - vToPlane;
	return vToPlane;
}

float gPlane::getDistanceTo(const gVector3& p ) const
{
	return  a * p.x + b * p.y + c * p.z + d;
}

// ------------------------------------
//
//		*** class gQuaternion ***
//
// ------------------------------------

gQuaternion::gQuaternion() 
{
	//identity();
}

gQuaternion::gQuaternion(const float* v) // 4 floats
{
	x = v[0]; y = v[1]; z = v[2]; w = v[3];
}

gQuaternion::gQuaternion(float _x, float _y, float _z, float _w)
{
	x = _x; y = _y; z = _z; w = _w;
}

gQuaternion::gQuaternion(gVector3 axis, float angle)
{
	fromAxisAngle(axis, angle);
}

gQuaternion::gQuaternion(float pitch, float yaw, float roll)
{
	fromPitchYawRoll(pitch, yaw, roll);
}

void gQuaternion::identity()  // make identity quat
{
	memcpy_s( v, gQuaternion::Size, gQuaternion::Identity.v, gQuaternion::Size );
}

void gQuaternion::fromEquationConstants(float _x, float _y, float _z, float _w) //build quat from constants
{
	x = _x; y = _y; z = _z; w = _w;
}

void gQuaternion::fromAxisAngle(gVector3 axis, float angle)  // build quat from axis & angle in radians
{
	float ha = angle / 2.f;
	float sha = sinf(ha);
	x = axis.x * sha;
	y = axis.y * sha;
	z = axis.z * sha;
	w = cosf(ha);
}

//===========================================================
//	FROM GPUGEMS IV:
//===========================================================

/**** EulerAngles.c - Convert Euler angles to/from matrix or quat ****/
/* Ken Shoemake, 1993 */
void gQuaternion::fromPitchYawRoll( float pitch, float yaw, float roll ) // build quat pitch, yaw, roll angles in radians
{
	float a[3], ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;

	ti = yaw * 0.5f; tj = pitch * 0.5f; th = roll * 0.5f;
	ci = cosf(ti);  cj = cosf(tj);  ch = cosf(th);
	si = sinf(ti);  sj = sinf(tj);  sh = sinf(th);
	cc = ci * ch; cs = ci * sh; sc = si * ch; ss = si * sh;

	a[1] = cj * sc - sj * cs;
	a[0] = cj * ss + sj * cc;
	a[2] = sj * sc - cj * cs;

	x = a[0]; y = a[1]; z = -a[2]; // "-" added
	w = cj * cc + sj * ss;
}

//===========================================================
//	FROM GPUGEMS IV:
//===========================================================

/**** EulerAngles.c - Convert Euler angles to/from matrix or quat ****/
/* Ken Shoemake, 1993 */
gVector3 gQuaternion::getPitchYawRoll() const // get pitch, yaw, roll angles equivalent to quat
{
	gVector3 angles;

	float Nq = x * x + y * y + z * z + w * w;
	float _s = (Nq > 0.f) ? (2.f / Nq) : 0.f;

	float xs = x * _s, ys = y * _s, zs = z * _s;
	float wx = w * xs, wy = w * ys, wz = w * zs;
	float xx = x * xs, xy = x * ys, xz = x * zs;
	float yy = y * ys, yz = y * zs, zz = z * zs;

	float m[3][3] = { {1.f - (yy + zz),	xy - wz,					xz + wy},
					  { xy + wz,		1.f - (xx + zz),			yz - wx},
					  {xz - wy,			yz + wx,					1.f - (xx + yy)} };

	float cy = sqrtf(m[1][1] * m[1][1] + m[0][1] * m[0][1]);
	if (cy > 16 * FLT_EPSILON) {
		angles.y = -atan2f(m[2][0], m[2][2]);
		angles.x = -atan2f(-m[2][1], cy);
		angles.z = -atan2f(m[0][1], m[1][1]); 
	}
	else {
		angles.y = -atan2f(-m[0][2], m[0][0]);
		angles.x = -atan2f(-m[2][1], cy);
		angles.z = 0;
	}

	return angles;
}

// ------------------------------------
//
//		*** class gMatrix4 ***
//
// ------------------------------------

gMatrix4::gMatrix4() 
{ 
	identity(); 
}

gMatrix4::gMatrix4( bool isIdentity ) 
{ 
	if (isIdentity) 
		identity(); 
}

gMatrix4::gMatrix4( const float* m )
{
	memcpy_s(p, gMatrix4::Size, m, gMatrix4::Size);
}

gMatrix4::gMatrix4(const gMatrix4& other) 
{ 
	memcpy_s( p, gMatrix4::Size, other.p, gMatrix4::Size );
}

void gMatrix4::identity()
{
	memcpy_s( p, gMatrix4::Size, gMatrix4::Identity.p, gMatrix4::Size );
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
	v4 = gVector4( x, y, z, 1.f );
}

void gMatrix4::translation(const gVector4& v)  // use first 3 dim
{
	identity();
	v4 = gVector4(v.x, v.y, v.z, 1.f);
}

void gMatrix4::operator = (const gMatrix4& other)
{
	memcpy_s( p, gMatrix4::Size, other.p, gMatrix4::Size );
}
