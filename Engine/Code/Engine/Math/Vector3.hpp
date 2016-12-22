#pragma once


#include <math.h>
#include "Engine/Error/ErrorWarningAssert.hpp"


class Vector3
{
public:
	inline Vector3();
	inline Vector3( float initialX, float initialY, float initialZ );
	inline Vector3( const Vector3& vectorToCopy );
	 
	inline void GetXYZ( float& out_x, float& out_y, float& out_z ) const;
	inline void SetXYZ( float newX, float newY, float newZ );
	 
	inline float CalcLength() const;
	inline void Normalize();

	inline Vector3 operator+( const Vector3& vectorToAdd ) const;
	inline Vector3 operator-( const Vector3& vectorToSubtract ) const;
	inline Vector3 operator-( );
	inline Vector3 operator*( float scalarToScaleBy ) const;
	inline Vector3 operator/( float scalarToScaleBy ) const;
	inline void operator+=( const Vector3& vectorToAdd );
	inline void operator-=( const Vector3& vectorToSubtract );
	inline void operator*=( const float uniformScale );
	inline void operator=( const Vector3& copyFrom );
	inline bool operator==( const Vector3& compareTo ) const;
	inline bool operator!=( const Vector3& compareTo ) const;

public: //Because Vector3 is virtually a primitive.

	static const Vector3 ZERO;
	static const Vector3 ONE;

	float x;
	float y;
	float z;
};


//--------------------------------------------------------------------------------------------------------------
// Do-nothing default ctor: because it saves time to leave trash values rather than allocate and initialize.
inline Vector3::Vector3()
	: x()
	, y()
	, z()
{
}


//--------------------------------------------------------------------------------------------------------------
inline Vector3::Vector3( float initialX, float initialY, float initialZ )
	: x( initialX )
	, y( initialY )
	, z( initialZ )
{
}


//--------------------------------------------------------------------------------------------------------------
inline Vector3::Vector3( const Vector3& vectorToCopy )
	: x( vectorToCopy.x )
	, y( vectorToCopy.y )
	, z( vectorToCopy.z )
{
}


//--------------------------------------------------------------------------------------------------------------
inline Vector3 Vector3::operator+( const Vector3& vectorToAdd ) const
{
	return Vector3( x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z );
}


//--------------------------------------------------------------------------------------------------------------
inline Vector3 Vector3::operator-( const Vector3& vectorToSubtract ) const
{
	return Vector3( x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z );
}


//--------------------------------------------------------------------------------------------------------------
inline Vector3 Vector3::operator-( )
{
	x = -x;
	y = -y;
	z = -z;
	return Vector3( x, y, z );
}


//--------------------------------------------------------------------------------------------------------------
inline Vector3 Vector3::operator*( float scalarToScaleBy ) const
{
	return Vector3( x * scalarToScaleBy, y * scalarToScaleBy, z * scalarToScaleBy );
}


//--------------------------------------------------------------------------------------------------------------
inline Vector3 Vector3::operator/( float scalarToScaleBy ) const
{
	if ( scalarToScaleBy == 0.f ) ERROR_AND_DIE( "Vector3 Divided By Scalar Zero" );
	return Vector3( x / scalarToScaleBy, y / scalarToScaleBy, z / scalarToScaleBy );
}


//--------------------------------------------------------------------------------------------------------------
inline bool Vector3::operator==( const Vector3& compareTo ) const
{
	return ( x == compareTo.x ) && ( y == compareTo.y ) && ( z == compareTo.z );
}


//--------------------------------------------------------------------------------------------------------------
inline bool Vector3::operator!=( const Vector3& compareTo ) const
{
	return !( *this == compareTo );
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector3::operator+=( const Vector3& vectorToAdd )
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector3::operator-=( const Vector3& vectorToSubtract )
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector3::operator*=( const float scalarToScaleBy )
{
	x *= scalarToScaleBy;
	y *= scalarToScaleBy;
	z *= scalarToScaleBy;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector3::operator=( const Vector3& vectorToCopy )
{
	x = vectorToCopy.x;
	y = vectorToCopy.y;
	z = vectorToCopy.z;
}


//--------------------------------------------------------------------------------------------------------------
inline float Vector3::CalcLength() const
{
	return sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector3::GetXYZ( float& out_x, float& out_y, float& out_z ) const
{
	out_x = x;
	out_y = y;
	out_z = z;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector3::SetXYZ( float newX, float newY, float newZ )
{
	x = newX;
	y = newY;
	z = newZ;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector3::Normalize()
{
	float len = CalcLength();
	if ( len == 0.f ) ERROR_AND_DIE( "Normalizing Vector3 By Length Zero" );

	x /= len;
	y /= len;
	z /= len;
}