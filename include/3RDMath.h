#pragma once

#ifndef _MATH_H_
#define _MATH_H_

#include "3RDEngine.h"

#define gPI   3.14159265358979323846f
#define gPI_2 1.57079632679489661923f
#define gPI_4 0.78539816339744830962f

struct gVector4
{
    union
    {
        float _v[4];
        struct
        {
            float x, y, z, w;
        };
    };

    _3RDE_API_ gVector4();
    _3RDE_API_ gVector4(float value);
    _3RDE_API_ gVector4(float x, float y, float z, float w);
    _3RDE_API_ gVector4(const gVector4& other);

    _3RDE_API_ void operator = (const gVector4& other);

    _3RDE_API_ void operator += (const gVector4& other);
    _3RDE_API_ void operator -= (const gVector4& other);
    _3RDE_API_ void operator *= ( float value );

    _3RDE_API_ float operator * ( const gVector4& other ); // dot product of two vecs
};

struct gVector3
{
    union
    {
        float _v[3];
        struct
        {
            float x, y, z;
        };
    };

    _3RDE_API_ gVector3();
    _3RDE_API_ gVector3(float value);
    _3RDE_API_ gVector3(float x, float y, float z);
    _3RDE_API_ gVector3(const gVector3& other);

    _3RDE_API_ void operator = (const gVector3& other);

    _3RDE_API_ void operator += (const gVector3& other);
    _3RDE_API_ void operator -= (const gVector3& other);
    _3RDE_API_ void operator *= (float value);

    _3RDE_API_ void operator ^= ( const gVector3& other ); // cross product with other vec

    _3RDE_API_ float operator * (const gVector3& other); // dot product of two vecs
    _3RDE_API_ const gVector3 operator ^ (const gVector3& other); // cross product of two vecs

};

struct gVector2
{
    union u
    {
        float v[2];
        struct
        {
            float x, y;
        };
    };
};

struct gPlane
{
    union u
    {
        float p[4];
        struct
        {
            float a, b, c, d;
        };
    };

};

struct gQuaternion
{
    union
    {
        float v[4];
        struct
        {
            float x, y, z, w;
        };
    };

    _3RDE_API_ gQuaternion(const float* v); // 4 floats
    _3RDE_API_ gQuaternion(float x, float y, float z, float w);

    _3RDE_API_ void identity();
};


// d3d matrix analog
struct gMatrix4
{
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;
        };

        struct {
            gVector4 _v1, _v2, _v3, _v4;
        };

        float _m[4][4];
        float _p[16];
    };

    _3RDE_API_ gMatrix4();
    _3RDE_API_ gMatrix4(bool isIdentity);
    _3RDE_API_ gMatrix4(const gMatrix4& other);

    _3RDE_API_ void  identity();
    _3RDE_API_ float determinant();
    _3RDE_API_ const gMatrix4& transpose();

    _3RDE_API_ void translation( float x, float y, float z );
    _3RDE_API_ void translation( const gVector4& v );  // use first 3 dim
    _3RDE_API_ void translation( const gVector3& v );
    _3RDE_API_ void translation( const float* v );  // 3 float

    _3RDE_API_ void rotationQuat( const gQuaternion& q );
    _3RDE_API_ void rotationQuat(const float* v);  // 4 float
    _3RDE_API_ void rotationPYR( float pitch, float yaw, float roll );
    _3RDE_API_ void rotationPYR(const float* v);  // 3 float

    _3RDE_API_ void scale( float sx, float sy, float sz );
    _3RDE_API_ void scale( const gVector3& v );
    _3RDE_API_ void scale( const float* v );  // 3 float

    _3RDE_API_ void operator = (const gMatrix4& other);
    
};

#endif
