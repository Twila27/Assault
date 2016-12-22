#pragma once


#include "Engine/Math/Vector3.hpp"


class AABB3
{
public:

	inline AABB3();
	inline AABB3( const Vector3& mins, const Vector3& maxs );

public:

	Vector3 mins;
	Vector3 maxs;
};

//--------------------------------------------------------------------------------------------------------------
// Do-nothing default ctor: because it saves time to leave trash values rather than allocate and initialize.
inline AABB3::AABB3() 
	: mins()
	, maxs()
{
}


//--------------------------------------------------------------------------------------------------------------
inline AABB3::AABB3( const Vector3& mins, const Vector3& maxs )
	: mins( mins )
	, maxs( maxs )
{
}
