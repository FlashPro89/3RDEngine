#pragma once

#ifndef _MATH_H_
#define _MATH_H_

#include "3RDEngine.h"

constexpr double gPI = 3.14159265358979323846;
constexpr double gPI_2 = 1.57079632679489661923;
constexpr double gPI_4 = 0.78539816339744830962;
constexpr double gPI_8 = 0.39269908169872415480;
constexpr double gPI_16 = 0.19634954084936207740;

constexpr float gPI_f = 3.14159265358979323846f;
constexpr float gPI_2_f = 1.57079632679489661923f;
constexpr float gPI_4_f = 0.78539816339744830962f;
constexpr float gPI_8_f = 0.39269908169872415480f;
constexpr float gPI_16_f = 0.19634954084936207740f;

struct gVector2
{
    union
    {
        float p[2];
        struct
        {
            float x, y;
        };
    };

    _3RDE_API_ static const size_t Size;
    _3RDE_API_ static const gVector2 Up;
    _3RDE_API_ static const gVector2 Right;
    _3RDE_API_ static const gVector2 Zero;

    _3RDE_API_ gVector2();
    _3RDE_API_ gVector2(const float* p); // 2 float
    _3RDE_API_ gVector2(float value);
    _3RDE_API_ gVector2(float x, float y);
    _3RDE_API_ gVector2(const gVector2& other);

    _3RDE_API_ void normalize();
    _3RDE_API_ float lenght() const;
    _3RDE_API_ float sqLenght() const;

    _3RDE_API_ void set(float value);
    _3RDE_API_ void set(float x, float y);

    //_3RDE_API_ void operator = (const gVector2& other);

    _3RDE_API_ gVector2 operator + () const;
    _3RDE_API_ gVector2 operator - () const;

    _3RDE_API_ gVector2 operator + (const gVector2& other) const;
    _3RDE_API_ gVector2 operator - (const gVector2& other) const;

    _3RDE_API_ gVector2& operator += (const gVector2& other);
    _3RDE_API_ gVector2& operator -= (const gVector2& other);
    _3RDE_API_ gVector2& operator *= (float value);
    _3RDE_API_ gVector2& operator /= (float value);

    _3RDE_API_ float operator * (const gVector2& other) const; // dot product of two vecs
    _3RDE_API_ gVector2 operator * (float value) const; // scale vec


};

struct gVector3
{
    union
    {
        float p[3];
        struct
        {
            float x, y, z;
        };
    };

    _3RDE_API_ static const size_t Size;
    _3RDE_API_ static const gVector3 Right;
    _3RDE_API_ static const gVector3 Up;
    _3RDE_API_ static const gVector3 Front;
    _3RDE_API_ static const gVector3 Zero;
    
    _3RDE_API_ gVector3();
    _3RDE_API_ gVector3(const float* p); // 3 float
    _3RDE_API_ gVector3(float value);
    _3RDE_API_ gVector3(float x, float y, float z);
    _3RDE_API_ gVector3(const gVector3& other);

    _3RDE_API_ void normalize();
    _3RDE_API_ float lenght() const;
    _3RDE_API_ float sqLenght() const;
    _3RDE_API_ float distanceTo(const gVector3& other) const;

    _3RDE_API_ void set(float value);
    _3RDE_API_ void set(float x, float y, float z);

    //_3RDE_API_ void operator = (const gVector3& other);

    _3RDE_API_ gVector3 operator + () const;
    _3RDE_API_ gVector3 operator - () const;

    _3RDE_API_ gVector3 operator + (const gVector3& other) const;
    _3RDE_API_ gVector3 operator - (const gVector3& other) const;

    _3RDE_API_ gVector3& operator += (const gVector3& other);
    _3RDE_API_ gVector3& operator -= (const gVector3& other);
    _3RDE_API_ gVector3& operator *= (float value);
    _3RDE_API_ gVector3& operator /= (float value);

    _3RDE_API_ gVector3& operator ^= ( const gVector3& other ); // cross product with other vec

    _3RDE_API_ float operator * (const gVector3& other) const; // dot product of two vecs
    _3RDE_API_ gVector3 operator * (float value) const; // scale vec
    _3RDE_API_ gVector3 operator ^ (const gVector3& other) const; // cross product of two vecs

};

struct gVector4
{
    union
    {
        float p[4];
        struct
        {
            float x, y, z, w;
        };
        struct
        {
            gVector3 v;
            float w;
        };
    };

    _3RDE_API_ static const size_t Size;
    _3RDE_API_ static const gVector4 Right;
    _3RDE_API_ static const gVector4 Up;
    _3RDE_API_ static const gVector4 Front;
    _3RDE_API_ static const gVector4 Zero;

    _3RDE_API_ gVector4();
    _3RDE_API_ gVector4(const float* p); // 4 float
    _3RDE_API_ gVector4(float value);
    _3RDE_API_ gVector4(float x, float y, float z, float w);
    _3RDE_API_ gVector4(const gVector3& other, float w = 1.f);
    _3RDE_API_ gVector4(const gVector4& other);

    _3RDE_API_ void normalize();
    _3RDE_API_ float lenght() const;
    _3RDE_API_ float sqLenght() const;

    _3RDE_API_ void set(float value);
    _3RDE_API_ void set(float x, float y, float z, float w);

    //_3RDE_API_ gVector4& operator = (const gVector4& other);

    _3RDE_API_ gVector4 operator + () const;
    _3RDE_API_ gVector4 operator - () const;

    _3RDE_API_ gVector4 operator + (const gVector4& other) const;
    _3RDE_API_ gVector4 operator - (const gVector4& other) const;

    _3RDE_API_ gVector4& operator += (const gVector4& other);
    _3RDE_API_ gVector4& operator -= (const gVector4& other);
    _3RDE_API_ gVector4& operator *= (float value);
    _3RDE_API_ gVector4& operator /= (float value);

    _3RDE_API_ float operator * (const gVector4& other) const; // dot product of two vecs
    _3RDE_API_ gVector4 operator * (float value) const; // scale vec
};

struct gPlane
{
    union
    {
        float p[4];
        struct
        {
            float a, b, c, d;
        };
        struct
        {
            gVector3 n;
            float d;
        };
    };

    _3RDE_API_ gPlane();
    _3RDE_API_ gPlane( float a, float b, float c, float d );
    _3RDE_API_ gPlane( const gVector3& p0, const gVector3& p1, const gVector3& p2 );

    _3RDE_API_ void fromEquationConstants( float a, float b, float c, float d ); // build plane equation constants
    _3RDE_API_ void fromPointNormal( const gVector3& p, const gVector3& n ); // build plane by pointand non - normalized normal
    _3RDE_API_ void fromPointNormalizedNormal(const gVector3& p, const gVector3& n); // build plane by point & normalized normal
    _3RDE_API_ void fromPointVectors(const gVector3& p, const gVector3& v0, const gVector3& v1); // build plane by two vecs of point
    _3RDE_API_ void fromPoints( const gVector3& p0, const gVector3& p1, const gVector3& p2 ); // build plane by 3 points

    _3RDE_API_ void normalize(); // normalize plane

    _3RDE_API_ gVector3 projectPointToPlane( const gVector3& p) const; // project point to plane in plane direction ( plane must be normalized! )

    _3RDE_API_ float getDistanceTo(const gVector3& p) const;

    _3RDE_API_ static const size_t Size;
};

struct gQuaternion
{
    union
    {
        float p[4];
        struct
        {
            float x, y, z, w;
        };

        struct
        {
            gVector3 v;
            float w;
        };
    };

    _3RDE_API_ static const size_t Size;
    _3RDE_API_ static const gQuaternion Identity;

    _3RDE_API_ gQuaternion();
    _3RDE_API_ gQuaternion( const gQuaternion& other );
    _3RDE_API_ gQuaternion( const float* v ); // 4 floats
    _3RDE_API_ gQuaternion( float x, float y, float z, float w );
    _3RDE_API_ gQuaternion( gVector3 axis, float angle );
    _3RDE_API_ gQuaternion( float pitch, float yaw, float roll );

    _3RDE_API_ void identity(); // make identity quat
    _3RDE_API_ float norm();  // dot product of quat components
    _3RDE_API_ float magnitude(); // "lenght" of quat
    _3RDE_API_ gQuaternion& inverse(); // build inverse quat

    //vec transform
    _3RDE_API_ gVector3& rotateVec( gVector3& v );

    //loading funcs
    _3RDE_API_ void fromEquationConstants(float x, float y, float z, float w ); //build quat from constants
    _3RDE_API_ void fromAxisAngle( gVector3 axis, float angle );  // build quat from axis & angle in radians
    _3RDE_API_ void fromXYZAngles( float x, float y, float z ); // build quat pitch, yaw, roll angles in radians
    _3RDE_API_ gVector3 getXYZAngles() const; // get pitch, yaw, roll angles equivalent to quat

    //operators
    _3RDE_API_ gQuaternion operator * (float value) const;
    _3RDE_API_ gQuaternion operator / (float value) const;
    _3RDE_API_ gQuaternion operator * (const gQuaternion& other) const;
    _3RDE_API_ gQuaternion operator + (const gQuaternion& other) const;

    _3RDE_API_ gQuaternion& operator *= (float value);
    _3RDE_API_ gQuaternion& operator /= (float value);
    _3RDE_API_ gQuaternion& operator *= (const gQuaternion& other);
    _3RDE_API_ gQuaternion& operator += (const gQuaternion& other);

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
            gVector4 v[4];
        };

        //float m[4][4];
        float p[16];
    };

    _3RDE_API_ static const size_t Size;
    _3RDE_API_ static const gMatrix4 Identity;
    _3RDE_API_ static const gVector4 V0_Id;
    _3RDE_API_ static const gVector4 V1_Id;
    _3RDE_API_ static const gVector4 V2_Id;
    _3RDE_API_ static const gVector4 V3_Id;

    _3RDE_API_ gMatrix4();
    _3RDE_API_ gMatrix4(const float* m);  //16 floats
    _3RDE_API_ gMatrix4(bool isIdentity);
    _3RDE_API_ gMatrix4(const gMatrix4& other);

    _3RDE_API_ bool  inverse(); 
    _3RDE_API_ void  setIdentity();
    _3RDE_API_ float determinant();
    _3RDE_API_ const gMatrix4& transpose();

    //vec transforms
    _3RDE_API_ gVector3& transformVec(gVector3& v);
    _3RDE_API_ gVector4& transformVec(gVector4& v);

    // translating
    _3RDE_API_ void setTranslation( float x, float y, float z );
    _3RDE_API_ void setTranslation( const gVector4& v );  // use first 3 dim
    _3RDE_API_ void setTranslation( const gVector3& v );
    _3RDE_API_ void setTranslation( const float* v );  // 3 float

    // rotating
    _3RDE_API_ void setRotationX(float angle);
    _3RDE_API_ void setRotationY(float angle);
    _3RDE_API_ void setRotationZ(float angle);

    _3RDE_API_ void setRotationByQuat( const gQuaternion& q );
    _3RDE_API_ void setRotationByQuat(const float* v);  // 4 float
    _3RDE_API_ void setRotationXYZ( float rx, float ry, float rz );
    _3RDE_API_ void setRotationXYZ(const float* v);  // 3 float

    // scaling
    _3RDE_API_ void setScale( float sx, float sy, float sz );
    _3RDE_API_ void setScale( const gVector3& v );
    _3RDE_API_ void setScale( const float* v );  // 3 float

    //projection matrixes
    _3RDE_API_ void setPerspetiveRHFOV(float fowY, float aspectRatio, float zn, float zf);
    _3RDE_API_ void setPerspetiveRH(float w, float h, float zn, float zf);
    _3RDE_API_ void setOrthoRH(float w, float h, float zn, float zf);

    // view matrixes
    _3RDE_API_ void setLookAtRH( const gVector3& eye, const gVector3& at, const gVector3& up = gVector3::Up );

    // operators
    _3RDE_API_ inline void operator = (const gMatrix4& other);
    _3RDE_API_ inline void operator *= ( const gMatrix4& other ); // matrix concatenation
    _3RDE_API_ inline gMatrix4 operator * ( const gMatrix4& other ) const; // matrix concatenation

};

#endif
