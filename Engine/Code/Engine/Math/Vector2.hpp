#pragma once


#include "Engine/Error/ErrorWarningAssert.hpp"


//-----------------------------------------------------------------------------
class Vector2
{
public:

	inline Vector2();
	inline Vector2( float initialX, float initialY );
	inline Vector2( const Vector2& vectorToCopy );

	inline void GetXY( float& out_x, float& out_y ) const;
	inline void SetXY( float newX, float newY );

	inline float CalcLength() const;
	inline void Normalize();

	inline const Vector2 operator+( const Vector2& vectorToAdd ) const;
	inline const Vector2 operator-( const Vector2& vectorToAdd ) const;
	inline const Vector2 operator-( );
	inline const Vector2 operator*( float scalarToScaleBy ) const;
	inline const Vector2 operator/( float scalarToScaleBy ) const;
	inline void operator+=( const Vector2& vectorToAdd );
	inline void operator-=( const Vector2& vectorToSubtract );
	inline void operator*=( const float uniformScale );
	inline void operator=( const Vector2& copyFrom );
	inline bool operator<( const Vector2& compareTo ) const;
	inline bool operator==( const Vector2& compareTo ) const;
	inline bool operator!=( const Vector2& compareTo ) const;


public: //Because Vector2 is virtually a primitive.

	static const Vector2 ZERO;
	static const Vector2 ONE;

	float x;
	float y;
};


//--------------------------------------------------------------------------------------------------------------
// Do-nothing default ctor: because it saves time to leave trash values rather than allocate and initialize.
inline Vector2::Vector2()
	: x()
	, y()
{
}


//--------------------------------------------------------------------------------------------------------------
inline Vector2::Vector2( float initialX, float initialY )
	: x( initialX )
	, y( initialY )
{
}


//--------------------------------------------------------------------------------------------------------------
inline Vector2::Vector2( const Vector2& vectorToCopy )
	: x( vectorToCopy.x )
	, y( vectorToCopy.y )
{
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector2::GetXY( float& out_x, float& out_y ) const
{
	out_x = x;
	out_y = y;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector2::SetXY( float newX, float newY )
{
	x = newX;
	y = newY;
}


//--------------------------------------------------------------------------------------------------------------
inline const Vector2 Vector2::operator+( const Vector2& vectorToAdd ) const
{
	return Vector2( x + vectorToAdd.x, y + vectorToAdd.y );
}


//--------------------------------------------------------------------------------------------------------------
inline const Vector2 Vector2::operator-( const Vector2& vectorToAdd ) const
{
	return Vector2( x - vectorToAdd.x, y - vectorToAdd.y );
}


//--------------------------------------------------------------------------------------------------------------
inline const Vector2 Vector2::operator-( )
{
	x = -x;
	y = -y;
	return Vector2( x, y );
}

//--------------------------------------------------------------------------------------------------------------
inline const Vector2 Vector2::operator*( float scalarToScaleBy ) const
{
	return Vector2( x * scalarToScaleBy, y * scalarToScaleBy );
}


//--------------------------------------------------------------------------------------------------------------
inline const Vector2 Vector2::operator/( float scalarToScaleBy ) const
{
	if ( scalarToScaleBy == 0.f ) ERROR_AND_DIE( "Vector2 Divided By Scalar Zero" );
	return Vector2( x / scalarToScaleBy, y / scalarToScaleBy );
}


//--------------------------------------------------------------------------------------------------------------
inline bool Vector2::operator==( const Vector2& compareTo ) const
{
	return ( x == compareTo.x ) && ( y == compareTo.y );
}


//--------------------------------------------------------------------------------------------------------------
inline bool Vector2::operator!=( const Vector2& compareTo ) const
{
	return !( *this == compareTo );
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector2::operator+=( const Vector2& vectorToAdd )
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector2::operator-=( const Vector2& vectorToSubtract )
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector2::operator*=( const float scalarToScaleBy )
{
	x *= scalarToScaleBy;
	y *= scalarToScaleBy;
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector2::operator=( const Vector2& vectorToCopy )
{
	x = vectorToCopy.x;
	y = vectorToCopy.y;
}


//--------------------------------------------------------------------------------------------------------------
inline bool Vector2::operator<( const Vector2& compareTo ) const
{
	if ( this->x < compareTo.x ) return true; //First on x,
	if ( this->x > compareTo.x ) return false;
	return this->y < compareTo.y; //Second ties broken on y.
}


//--------------------------------------------------------------------------------------------------------------
inline float Vector2::CalcLength( ) const
{
	return sqrt( ( x * x ) + ( y * y ) );
}


//--------------------------------------------------------------------------------------------------------------
inline void Vector2::Normalize( )
{
	float len = CalcLength();
	if ( len == 0.f ) ERROR_AND_DIE( "Normalizing Vector2 By Length Zero" );

	x /= len;
	y /= len;
}
