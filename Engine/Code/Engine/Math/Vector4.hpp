#pragma once


#include <math.h>
#include "Engine/Error/ErrorWarningAssert.hpp"


class Vector4
{
public:
	inline Vector4();
	inline Vector4( float initialX, float initialY, float initialZ, float initialW );
	inline Vector4( const Vector4& vectorToCopy );

	inline void GetXYZ( float& out_x, float& out_y, float& out_z, float& out_w ) const;
	inline void SetXYZ( float newX, float newY, float newZ, float newW );

	inline float CalcLength() const;
	inline void Normalize();

	inline Vector4 operator+( const Vector4& vectorToAdd ) const;
	inline Vector4 operator-( const Vector4& vectorToSubtract ) const;
	inline Vector4 operator-();
	inline Vector4 operator*( float scalarToScaleBy ) const;
	inline Vector4 operator/( float scalarToScaleBy ) const;
	inline void operator+=( const Vector4& vectorToAdd );
	inline void operator-=( const Vector4& vectorToSubtract );
	inline void operator*=( const float uniformScale );
	inline void operator=( const Vector4& copyFrom );
	inline bool operator==( const Vector4& compareTo ) const;
	inline bool operator!=( const Vector4& compareTo ) const;

public:

	static const Vector4 ZERO;
	static const Vector4 ONE;

	float x;
	float y;
	float z;
	float w;
};


//--------------------------------------------------------------------------------------------------------------
// Do-nothing default ctor: because it saves time to leave trash values rather than allocate and initialize.
inline Vector4::Vector4()
	: x()
	, y()
	, z()
	, w()
{
}


//--------------------------------------------------------------------------------------------------------------
inline Vector4::Vector4( float initialX, float initialY, float initialZ, float initialW )
	: x( initialX )
	, y( initialY )
	, z( initialZ )
	, w( initialW )
{
}


//--------------------------------------------------------------------------------------------------------------
inline Vector4::Vector4( const Vector4& vectorToCopy )
	: x( vectorToCopy.x )
	, y( vectorToCopy.y )
	, z( vectorToCopy.z )
	, w( vectorToCopy.w )
{
}


//--------------------------------------------------------------------------------------------------------------
inline Vector4 Vector4::operator+( const Vector4& vectorToAdd ) const
{
	return Vector4( x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z, w + vectorToAdd.w );
}


//--------------------------------------------------------------------------------------------------------------
inline Vector4 Vector4::operator-( const Vector4& vectorToSubtract ) const
{
	return Vector4( x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z, w - vectorToSubtract.w );
}


//--------------------------------------------------------------------------------------------------------------
inline Vector4 Vector4::operator-()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
	return Vector4( x, y, z, w );
}


//--------------------------------------------------------------------------------------------------------------
inline Vector4 Vector4::operator*( float scalarToScaleBy ) const
{
	return Vector4( x * scalarToScaleBy, y * scalarToScaleBy, z * scalarToScaleBy, w * scalarToScaleBy );
}


//--------------------------------------------------------------------------------------------------------------
inline Vector4 Vector4::operator/( float scalarToScaleBy ) const
{
	if ( scalarToScaleBy == 0.f ) ERROR_AND_DIE( "Vector4 Divided By Scalar Zero" );
	return Vector4( x / scalarToScaleBy, y / scalarToScaleBy, z / scalarToScaleBy, w / scalarToScaleBy );
}


//--------------------------------------------------------------------------------------------------------------
inline bool Vector4::operator==( const Vector4& compareTo ) const
{
	return ( x == compareTo.x ) && ( y == compareTo.y ) && ( z == compareTo.z ) && ( w == compareTo.w );
}


//--------------------------------------------------------------------------------------------------------------
inline bool Vector4::operator!=( const Vector4& compareTo ) const
{
	return !( *this == compareTo );
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector4::operator+=( const Vector4& vectorToAdd )
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
	w += vectorToAdd.w;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector4::operator-=( const Vector4& vectorToSubtract )
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
	w -= vectorToSubtract.w;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector4::operator*=( const float scalarToScaleBy )
{
	x *= scalarToScaleBy;
	y *= scalarToScaleBy;
	z *= scalarToScaleBy;
	w *= scalarToScaleBy;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector4::operator=( const Vector4& vectorToCopy )
{
	x = vectorToCopy.x;
	y = vectorToCopy.y;
	z = vectorToCopy.z;
	w = vectorToCopy.w;
}


//--------------------------------------------------------------------------------------------------------------
inline float Vector4::CalcLength() const
{
	return sqrt( ( x * x ) + ( y * y ) + ( z * z ) + ( w * w ) );
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector4::GetXYZ( float& out_x, float& out_y, float& out_z, float& out_w ) const
{
	out_x = x;
	out_y = y;
	out_z = z;
	out_w = w;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector4::SetXYZ( float newX, float newY, float newZ, float newW )
{
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector4::Normalize()
{
	float len = CalcLength();
	if ( len == 0.f ) ERROR_AND_DIE( "Normalizing Vector4 By Length Zero" );

	x /= len;
	y /= len;
	z /= len;
	w /= len;
}