#pragma once


#include "Engine/Math/Vector2.hpp"


class AABB2
{
public:

	inline AABB2();
	inline AABB2( const Vector2& mins, const Vector2& maxs );
	inline AABB2( float minX, float minY, float maxX, float maxY );
	inline AABB2( int minX, int minY, int maxX, int maxY );


public:

	Vector2 mins;
	Vector2 maxs;
};


//--------------------------------------------------------------------------------------------------------------
// Do-nothing default ctor: because it saves time to leave trash values rather than allocate and initialize.
inline AABB2::AABB2()
	: mins()
	, maxs()
{
}


//--------------------------------------------------------------------------------------------------------------
inline AABB2::AABB2( const Vector2& mins, const Vector2& maxs )
	: mins( mins )
	, maxs( maxs )
{
}


//--------------------------------------------------------------------------------------------------------------
inline AABB2::AABB2( float minX, float minY, float maxX, float maxY )
	: mins( Vector2( minX, minY ) )
	, maxs( Vector2( maxX, maxY ) )
{
}


//--------------------------------------------------------------------------------------------------------------
inline AABB2::AABB2( int minX, int minY, int maxX, int maxY )
	: mins( Vector2( static_cast<float>( minX ), static_cast<float>( minY ) ) )
	, maxs( Vector2( static_cast<float>( maxX ), static_cast<float>( maxY ) ) )
{
}
