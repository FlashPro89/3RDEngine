#include "../include/3RDMath.h"
#include <math.h>
#include <STRing>
#include <xmmintrin.h>


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

static gMatrix4 s_temp;

const gVector4 gMatrix4::V0_Id = gVector4(1.f, 0.f, 0.f, 0.f);
const gVector4 gMatrix4::V1_Id = gVector4(0.f, 1.f, 0.f, 0.f);
const gVector4 gMatrix4::V2_Id = gVector4(0.f, 0.f, 1.f, 0.f);
const gVector4 gMatrix4::V3_Id = gVector4(0.f, 0.f, 0.f, 1.f);

const gMatrix4 gMatrix4::Identity = gMatrix4( (const float*)_m_id_ );
const gQuaternion gQuaternion::Identity = gQuaternion( 0.f, 0.f, 0.f, 1.f );

const gVector2 gVector2::Up(0.f, 1.f);
const gVector2 gVector2::Right(1.f, 0.f);
const gVector2 gVector2::Zero(0.f, 0.f);


const gVector3 gVector3::Right(1.f, 0.f, 0.f);
const gVector3 gVector3::Up(0.f, 1.f, 0.f);
const gVector3 gVector3::Front(0.f, 0.f, 1.f);
const gVector3 gVector3::Zero(0.f, 0.f, 0.f);


const gVector4 gVector4::Right(1.f, 0.f, 0.f, 1.f);
const gVector4 gVector4::Up(0.f, 1.f, 0.f, 1.f);
const gVector4 gVector4::Front(0.f, 0.f, 1.f, 1.f);
const gVector4 gVector4::Zero(0.f, 0.f, 0.f, 0.f);

#define _BUILD_WITH_SSE_

// from GitHub free code
inline void m_invert_noopt(const float* src, float* dst)
{
	float det;

	/* Compute adjoint: */

	dst[0] =
		+src[5] * src[10] * src[15]
		- src[5] * src[11] * src[14]
		- src[9] * src[6] * src[15]
		+ src[9] * src[7] * src[14]
		+ src[13] * src[6] * src[11]
		- src[13] * src[7] * src[10];

	dst[1] =
		-src[1] * src[10] * src[15]
		+ src[1] * src[11] * src[14]
		+ src[9] * src[2] * src[15]
		- src[9] * src[3] * src[14]
		- src[13] * src[2] * src[11]
		+ src[13] * src[3] * src[10];

	dst[2] =
		+src[1] * src[6] * src[15]
		- src[1] * src[7] * src[14]
		- src[5] * src[2] * src[15]
		+ src[5] * src[3] * src[14]
		+ src[13] * src[2] * src[7]
		- src[13] * src[3] * src[6];

	dst[3] =
		-src[1] * src[6] * src[11]
		+ src[1] * src[7] * src[10]
		+ src[5] * src[2] * src[11]
		- src[5] * src[3] * src[10]
		- src[9] * src[2] * src[7]
		+ src[9] * src[3] * src[6];

	dst[4] =
		-src[4] * src[10] * src[15]
		+ src[4] * src[11] * src[14]
		+ src[8] * src[6] * src[15]
		- src[8] * src[7] * src[14]
		- src[12] * src[6] * src[11]
		+ src[12] * src[7] * src[10];

	dst[5] =
		+src[0] * src[10] * src[15]
		- src[0] * src[11] * src[14]
		- src[8] * src[2] * src[15]
		+ src[8] * src[3] * src[14]
		+ src[12] * src[2] * src[11]
		- src[12] * src[3] * src[10];

	dst[6] =
		-src[0] * src[6] * src[15]
		+ src[0] * src[7] * src[14]
		+ src[4] * src[2] * src[15]
		- src[4] * src[3] * src[14]
		- src[12] * src[2] * src[7]
		+ src[12] * src[3] * src[6];

	dst[7] =
		+src[0] * src[6] * src[11]
		- src[0] * src[7] * src[10]
		- src[4] * src[2] * src[11]
		+ src[4] * src[3] * src[10]
		+ src[8] * src[2] * src[7]
		- src[8] * src[3] * src[6];

	dst[8] =
		+src[4] * src[9] * src[15]
		- src[4] * src[11] * src[13]
		- src[8] * src[5] * src[15]
		+ src[8] * src[7] * src[13]
		+ src[12] * src[5] * src[11]
		- src[12] * src[7] * src[9];

	dst[9] =
		-src[0] * src[9] * src[15]
		+ src[0] * src[11] * src[13]
		+ src[8] * src[1] * src[15]
		- src[8] * src[3] * src[13]
		- src[12] * src[1] * src[11]
		+ src[12] * src[3] * src[9];

	dst[10] =
		+src[0] * src[5] * src[15]
		- src[0] * src[7] * src[13]
		- src[4] * src[1] * src[15]
		+ src[4] * src[3] * src[13]
		+ src[12] * src[1] * src[7]
		- src[12] * src[3] * src[5];

	dst[11] =
		-src[0] * src[5] * src[11]
		+ src[0] * src[7] * src[9]
		+ src[4] * src[1] * src[11]
		- src[4] * src[3] * src[9]
		- src[8] * src[1] * src[7]
		+ src[8] * src[3] * src[5];

	dst[12] =
		-src[4] * src[9] * src[14]
		+ src[4] * src[10] * src[13]
		+ src[8] * src[5] * src[14]
		- src[8] * src[6] * src[13]
		- src[12] * src[5] * src[10]
		+ src[12] * src[6] * src[9];

	dst[13] =
		+src[0] * src[9] * src[14]
		- src[0] * src[10] * src[13]
		- src[8] * src[1] * src[14]
		+ src[8] * src[2] * src[13]
		+ src[12] * src[1] * src[10]
		- src[12] * src[2] * src[9];

	dst[14] =
		-src[0] * src[5] * src[14]
		+ src[0] * src[6] * src[13]
		+ src[4] * src[1] * src[14]
		- src[4] * src[2] * src[13]
		- src[12] * src[1] * src[6]
		+ src[12] * src[2] * src[5];

	dst[15] =
		+src[0] * src[5] * src[10]
		- src[0] * src[6] * src[9]
		- src[4] * src[1] * src[10]
		+ src[4] * src[2] * src[9]
		+ src[8] * src[1] * src[6]
		- src[8] * src[2] * src[5];

	/* Compute determinant: */

	det = +src[0] * dst[0] + src[1] * dst[4] + src[2] * dst[8] + src[3] * dst[12];

	/* Multiply adjoint with reciprocal of determinant: */

	det = 1.0f / det;

	dst[0] *= det;
	dst[1] *= det;
	dst[2] *= det;
	dst[3] *= det;
	dst[4] *= det;
	dst[5] *= det;
	dst[6] *= det;
	dst[7] *= det;
	dst[8] *= det;
	dst[9] *= det;
	dst[10] *= det;
	dst[11] *= det;
	dst[12] *= det;
	dst[13] *= det;
	dst[14] *= det;
	dst[15] *= det;
}

// The original code as provided by Intel in
// "STReaming SIMD Extensions - Inverse of 4x4 Matrix"
// (ftp://download.intel.com/design/pentiumiii/sml/24504301.pdf)

inline void m_invert_sse(const float* src, float* dst)
{
	__m128 minor0, minor1, minor2, minor3;
	__m128 row0, row2;
	__m128 det;// , tmp1;

	// added init var, its slowly
	__m128 tmp1 = _mm_loadh_pi(_mm_load_ps(src), (__m64*)(src + 4));
	__m128 row1 = _mm_loadh_pi(_mm_load_ps(src + 8), (__m64*)(src + 12));
	__m128 row3 = _mm_loadh_pi(_mm_load_ps(src + 10), (__m64*)(src + 14));


	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src + 4));
	row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src + 8)), (__m64*)(src + 12));

	row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
	row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);

	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src + 2)), (__m64*)(src + 6));
	row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src + 10)), (__m64*)(src + 14));

	row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
	row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);

	tmp1 = _mm_mul_ps(row2, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor0 = _mm_mul_ps(row1, tmp1);
	minor1 = _mm_mul_ps(row0, tmp1);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

	tmp1 = _mm_mul_ps(row1, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
	minor3 = _mm_mul_ps(row0, tmp1);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

	tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	row2 = _mm_shuffle_ps(row2, row2, 0x4E);

	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
	minor2 = _mm_mul_ps(row0, tmp1);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

	tmp1 = _mm_mul_ps(row0, row1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));

	tmp1 = _mm_mul_ps(row0, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));

	tmp1 = _mm_mul_ps(row0, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);

	det = _mm_mul_ps(row0, minor0);
	det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
	det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);

	tmp1 = _mm_rcp_ss(det);

	det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
	det = _mm_shuffle_ps(det, det, 0x00);

	minor0 = _mm_mul_ps(det, minor0);
	_mm_storel_pi((__m64*)(dst), minor0);
	_mm_storeh_pi((__m64*)(dst + 2), minor0);

	minor1 = _mm_mul_ps(det, minor1);
	_mm_storel_pi((__m64*)(dst + 4), minor1);
	_mm_storeh_pi((__m64*)(dst + 6), minor1);

	minor2 = _mm_mul_ps(det, minor2);
	_mm_storel_pi((__m64*)(dst + 8), minor2);
	_mm_storeh_pi((__m64*)(dst + 10), minor2);

	minor3 = _mm_mul_ps(det, minor3);
	_mm_storel_pi((__m64*)(dst + 12), minor3);
	_mm_storeh_pi((__m64*)(dst + 14), minor3);
}

/*
* 
void mm_mul_sse_dot(const float* a, const float* b, float* r)
{
	__m128 a_line, b_line, r_line;
	float mc[16] __attribute__((aligned(16)));  // 16-byte aligned temp array
	for (int i = 0; i < 16; i += 4) {
		b_line = _mm_load_ps(&b[i]);              // b_line = vec4(column(b, i))
												  // remember that we are column-major
		for (int j = 0; j < 4; j++) {
			a_line = _mm_set_ps(a[j + 12], a[j + 8], a[j + 4], a[j]);
			// a_line = vec4(row(a, j))
			// note that SSE is little-endian
			r_line = _mm_mul_ps(a_line, b_line);    // r_line = a_line * b_line
			_mm_store_ps(mc, r_line);               // copy r_line to memory
			r[i + j] = mc[0] + mc[1] + mc[2] + mc[3];       // r[i][j] = sum(r_line)
													//         = dot(a_line, b_line)
		}
	}
}

*/


__inline void mm_mul_noopt_dot(const float* a, const float* b, float* r)
{
	/*
	unsigned int rindex = 0;
	for (int i = 0; i < 4; i++) //row
	{
		for (int j = 0; j < 4; j++) //column
		{
			rindex = i * 4 + j;
			r[rindex] = 0;
			
			for (int k = 0; k < 4; k++)
			{
				r[rindex] += a[i * 4 + k] * b[k * 4 + j];
			}
			
		}
	}
	*/

	r[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8]  + a[3] * b[12];
	r[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9]  + a[3] * b[13];
	r[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
	r[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

	r[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8]  + a[7] * b[12];
	r[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9]  + a[7] * b[13];
	r[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
	r[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

	r[8] =  a[8] * b[0] + a[9] * b[4] + a[10] * b[8]  + a[11] * b[12];
	r[9] =  a[8] * b[1] + a[9] * b[5] + a[10] * b[9]  + a[11] * b[13];
	r[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
	r[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

	r[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8]  + a[15] * b[12];
	r[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9]  + a[15] * b[13];
	r[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
	r[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
}

__inline void mv_mul_noopt_dot(const float* a, const float* b, float* r)
{
	/*
	unsigned int rindex = 0;
	for (int i = 0; i < 4; i++) //row
	{
		r[i] = 0;
		for (int j = 0; j < 4; j++) //col
		{
			r[i] += a[j * 4 + i] * b[j]; // use matrix transpose i % j
		}
	}
	*/

	r[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
	r[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
	r[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
	r[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
}



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

gVector4::gVector4(const gVector3& other, float _w )
{
	x = other.x; y = other.y; z = other.z; w = _w;
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

gVector4& gVector4::operator += (const gVector4& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

gVector4& gVector4::operator -= (const gVector4& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

gVector4& gVector4::operator *= (float value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;
	return *this;

}

gVector4& gVector4::operator /= (float value)
{
	if (value != 0)
	{
		x /= value;
		y /= value;
		z /= value;
		w /= value;
	}
	return *this;
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

gVector3& gVector3::operator += (const gVector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

gVector3& gVector3::operator -= (const gVector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

gVector3& gVector3::operator *= (float value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

gVector3& gVector3::operator /= (float value)
{
	if (value != 0)
	{
		x /= value;
		y /= value;
		z /= value;
	}
	return *this;
}

gVector3& gVector3::operator ^= (const gVector3& other) // cross product with other vec
{
	gVector3 tmp(*this);
	x = tmp.y * other.z - tmp.z * other.y;
	y = tmp.z * other.x - tmp.x * other.z;
	z = tmp.x * other.y - tmp.y * other.x;
	return *this;
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
	if( l != 0 )
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

gVector2& gVector2::operator += (const gVector2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

gVector2& gVector2::operator -= (const gVector2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

gVector2& gVector2::operator *= (float value)
{
	x *= value;
	y *= value;
	return *this;
}

gVector2& gVector2::operator /= (float value)
{
	if (value != 0)
	{
		x /= value;
		y /= value;
	}
	return *this;
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

gQuaternion::gQuaternion(const gQuaternion& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
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
	fromXYZAngles(pitch, yaw, roll);
}

void gQuaternion::identity()  // make identity quat
{
	v = gQuaternion::Identity.v;
	w = gQuaternion::Identity.w;
}

float gQuaternion::norm()  // dot product of quat components
{
	return x * x + y * y + z * z + w * w;
}

float gQuaternion::magnitude()
{
	return sqrtf( x * x + y * y + z * z + w * w ); // "lenght" of quat
}

gQuaternion& gQuaternion::inverse() // build inverse quat
{
	float _norm = norm();
	if (_norm == 0) //err
	{
		*this = gQuaternion::Identity;
	}
	else
	{
		x = -x; y = -y; z = -z; // conjugate quat
		*this = *this / _norm;
	}
	return *this;
}

gVector3& gQuaternion::rotateVec(gVector3& v)
{
	gQuaternion vq(v.x, v.y, v.z, 0.f);
	gQuaternion qinv = *this; qinv.inverse();
	
	return v = (*this * vq * qinv).v;
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
void gQuaternion::fromXYZAngles( float pitch, float yaw, float roll ) // build quat pitch, yaw, roll angles in radians
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
gVector3 gQuaternion::getXYZAngles() const // get pitch, yaw, roll angles equivalent to quat
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

gQuaternion gQuaternion::operator * (float value) const
{
	gQuaternion r;
	r.x = x * value;
	r.y = y * value;
	r.z = z * value;
	r.w = w * value;
	return r;
}

gQuaternion gQuaternion::operator / (float value) const
{
	gQuaternion r;
	r.x = x / value;
	r.y = y / value;
	r.z = z / value;
	r.w = w / value;
	return r;
}

gQuaternion gQuaternion::operator * (const gQuaternion& other) const
{
	gQuaternion r;
	r.x = w * other.x + x * other.w + y * other.z - z * other.y;
	r.y = w * other.y - x * other.z + y * other.w + z * other.x;
	r.z = w * other.z + x * other.y - y * other.x + z * other.w;
	r.w = w * other.w - x * other.x - y * other.y - z * other.z;
	return r;
}

gQuaternion gQuaternion::operator + (const gQuaternion& other) const
{
	gQuaternion r;
	r.x = x + other.x;
	r.y = y + other.y;
	r.z = z + other.z;
	r.w = w + other.w;
	return r;
}

gQuaternion& gQuaternion::operator *= (float value)
{
	x *= value; y *= value; z *= value; w *= value;
	return *this;
}

gQuaternion& gQuaternion::operator /= (float value)
{
	if (value != 0)
	{
		x /= value; y /= value; z /= value; w /= value;
	}
	return *this;
}

gQuaternion& gQuaternion::operator *= (const gQuaternion& other)
{
	gQuaternion t = *this;
	return *this = t * other;
}

gQuaternion& gQuaternion::operator += (const gQuaternion& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

// ------------------------------------
//
//		*** class gMatrix4 ***
//
// ------------------------------------

gMatrix4::gMatrix4() 
{ 
	//setIdentity();  //opti
}

gMatrix4::gMatrix4( bool isIdentity ) 
{ 
	if (isIdentity)
		setIdentity();
}

gMatrix4::gMatrix4( const float* m )
{
	memcpy_s(p, gMatrix4::Size, m, gMatrix4::Size);
}

gMatrix4::gMatrix4(const gMatrix4& other) 
{ 
	memcpy_s( p, gMatrix4::Size, other.p, gMatrix4::Size );
}

bool  gMatrix4::inverse()
{
	gMatrix4 temp = *this;
	
#ifdef _BUILD_WITH_SSE_
	m_invert_sse(temp.p, p);
#else
	invert_noopt(temp.p, p);
#endif

	return true;
}


void gMatrix4::setIdentity()
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

gVector3& gMatrix4::transformVec(gVector3& v)
{
	gVector4 r(v);
	mv_mul_noopt_dot(p, gVector4(v).p, r.p);
	v = r.v;
	return v;
}

gVector4& gMatrix4::transformVec(gVector4& v)
{
	gVector4 r(v);
	mv_mul_noopt_dot(p, v.p, r.p);
	v = r;
	return v;
}

void gMatrix4::setTranslation( float x, float y, float z )
{
	v[0] = V0_Id; v[1] = V1_Id; v[2] = V2_Id;
	v[3] = gVector4(x, y, z, 1.f);
}

void gMatrix4::setTranslation(const gVector4& _v)  // use first 3 dim
{
	v[0] = V0_Id; v[1] = V1_Id; v[2] = V2_Id;
	v[3] = gVector4(_v.x, _v.y, _v.z, 1.f);
}

void gMatrix4::setTranslation(const gVector3& _v)
{
	v[0] = V0_Id; v[1] = V1_Id; v[2] = V2_Id;
	v[3] = gVector4(_v.x, _v.y, _v.z, 1.f);
}

void gMatrix4::setTranslation(const float* _v)  // 3 float ( unsafe )
{
	v[0] = V0_Id; v[1] = V1_Id; v[2] = V2_Id;
	v[3] = gVector4( _v[0], _v[1], _v[2], 1.f);
}

void gMatrix4::setRotationX(float angle)
{
	float sinA = sinf(angle);
	float cosA = cosf(angle);

	v[0] = V0_Id;
	v[1] = gVector4( 0.f, cosA, sinA, 0.f );
	v[2] = gVector4( 0.f, -sinA, cosA, 0.f );
	v[3] = V3_Id;
}

void gMatrix4::setRotationY(float angle)
{
	float sinA = sinf(angle);
	float cosA = cosf(angle);

	v[0] = gVector4(cosA, 0.f, -sinA, 0.f);
	v[1] = V1_Id;
	v[2] = gVector4(sinA, 0, cosA, 0.f);
	v[3] = V3_Id;
}

void gMatrix4::setRotationZ(float angle)
{
	float sinA = sinf(angle);
	float cosA = cosf(angle);

	v[0] = gVector4(cosA, sinA, 0.f, 0.f);
	v[1] = gVector4(-sinA, cosA, 0.f,  0.f);
	v[2] = V2_Id;
	v[3] = V3_Id;
}

void gMatrix4::setRotationByQuat(const gQuaternion& q)
{
	float ww = 2.0f * q.w;
	float xx = 2.0f * q.x;
	float yy = 2.0f * q.y;
	float zz = 2.0f * q.z;

	_11 = 1.0f - yy * q.y - zz * q.z;
	_12 = xx * q.y + ww * q.z;
	_13 = xx * q.z - ww * q.y;
	_14 = 0.0f;

	_21 = xx * q.y - ww * q.z;
	_22 = 1.0f - xx * q.x - zz * q.z;
	_23 = yy * q.z + ww * q.x;
	_24 = 0.0f;

	_31 = xx * q.z + ww * q.y;
	_32 = yy * q.z - ww * q.x;
	_33 = 1.0f - xx * q.x - yy * q.y;
	_34 = 0.0f;

	v[3] = V3_Id;
}

void gMatrix4::setRotationByQuat(const float* _v)  // 4 float
{
	float ww = 2.0f * _v[3];
	float xx = 2.0f * _v[0];
	float yy = 2.0f * _v[1];
	float zz = 2.0f * _v[2];

	_11 = 1.0f - yy * _v[1] - zz * _v[2];
	_12 = xx * _v[1] + ww * _v[2];
	_13 = xx * _v[2] - ww * _v[1];
	_14 = 0.0f;

	_21 = xx * _v[1] - ww * _v[2];
	_22 = 1.0f - xx * _v[0] - zz * _v[2];
	_23 = yy * _v[2] + ww * _v[0];
	_24 = 0.0f;

	_31 = xx * _v[2] + ww * _v[1];
	_32 = yy * _v[2] - ww * _v[0];
	_33 = 1.0f - xx * _v[0] - yy * _v[1];
	_34 = 0.0f;

	v[3] = V3_Id;
}

void gMatrix4::setRotationXYZ(float rx, float ry, float rz)
{

}

void gMatrix4::setRotationXYZ(const float* v)  // 3 float
{

}

void gMatrix4::setScale(float sx, float sy, float sz)
{
	v[0] = gVector4(sx, 0.f, 0.f, 0.f);
	v[1] = gVector4(0.f, sy, 0.f, 0.f);
	v[2] = gVector4(0.f, 0.f, sz, 0.f);
	v[3] = V3_Id;
}

void gMatrix4::setScale(const gVector3& _v)
{
	v[0] = gVector4(_v.x, 0.f, 0.f, 0.f);
	v[1] = gVector4(0.f, _v.y, 0.f, 0.f);
	v[2] = gVector4(0.f, 0.f, _v.z, 0.f);
	v[3] = V3_Id;
}

void gMatrix4::setScale(const float* _v)  // 3 float
{
	v[0] = gVector4(_v[0], 0.f, 0.f, 0.f);
	v[1] = gVector4(0.f, _v[1], 0.f, 0.f);
	v[2] = gVector4(0.f, 0.f, _v[2], 0.f);
	v[3] = V3_Id;
}

// frox d3dx lib
void gMatrix4::setPerspetiveRHFOV( float fowY, float aspectRatio, float zn, float zf )
{
	float zf_zn = zf - zn;
	if( (fabs(fowY)>=gPI_f) || !( zf_zn != 0 ) || !(aspectRatio>=0 ))
		return;  // invalig diaposone

	float yScale = 1 / tanf( fowY / 2.f );
	float xScale = yScale / aspectRatio;

	zf_zn = zf / zf_zn;

	v[0] = gVector4(xScale, 0.f, 0.f, 0.f);
	v[1] = gVector4(0.f, yScale, 0.f, 0.f);
	v[2] = gVector4(0.f, 0.f, zf_zn, -1.f);
	v[3] = gVector4(0.f, 0.f, zn * zf_zn, 0.f);
}

void gMatrix4::setPerspetiveRH(float w, float h, float zn, float zf)
{
	float zf_zn = zf - zn;
	
	if ( (w==0) || (h==0) || (zf_zn == 0))
		return;  // invalig diaposone

	zf_zn = zf / zf_zn;

	v[0] = gVector4(2*zn/w, 0.f, 0.f, 0.f);
	v[1] = gVector4(0.f, 2*zn/h, 0.f, 0.f);
	v[2] = gVector4(0.f, 0.f, zf_zn, -1.f);
	v[3] = gVector4(0.f, 0.f, zn * zf_zn, 0.f);
}

void gMatrix4::setOrthoRH( float w, float h, float zn, float zf )
{
	float zn_zf = zn - zf;

	if ((w == 0) || (h == 0) || (zn_zf == 0))
		return;  // invalig diaposone

	zn_zf = 1.f / zn_zf;

	v[0] = gVector4(2 / w, 0.f, 0.f, 0.f);
	v[1] = gVector4(0.f, 2 / h, 0.f, 0.f);
	v[2] = gVector4(0.f, 0.f, zn_zf, 0.f);
	v[3] = gVector4(0.f, 0.f, zn*zn_zf, 1.f);
}

void gMatrix4::setLookAtRH( const gVector3& eye, const gVector3& at, const gVector3& up )
{
	gVector3 zAxis = eye - at; zAxis.normalize();
	gVector3 xAxis = up ^ zAxis; xAxis.normalize();
	gVector3 yAxis = zAxis ^ xAxis;

	float dotX = (xAxis * eye);
	float dotY = (yAxis * eye);
	float dotZ = (zAxis * eye);

	v[0] = gVector4(xAxis.x, yAxis.x, zAxis.x, 0.f);
	v[1] = gVector4(xAxis.y, yAxis.y, zAxis.y, 0.f);
	v[2] = gVector4(xAxis.z, yAxis.z, zAxis.z, 0.f);
	v[3] = gVector4( dotX, dotY, dotZ, 1.f);
}

void gMatrix4::operator = (const gMatrix4& other)
{
	memcpy_s(p, gMatrix4::Size, other.p, gMatrix4::Size);
}

void gMatrix4::operator *= (const gMatrix4& other)
{
	s_temp = *this;
	mm_mul_noopt_dot( s_temp.p, other.p, this->p );
}

gMatrix4 gMatrix4::operator * (const gMatrix4& other) const // matrix concatenation
{
	gMatrix4 m = *this;
	mm_mul_noopt_dot( this->p, other.p, m.p);
	return m;
}




