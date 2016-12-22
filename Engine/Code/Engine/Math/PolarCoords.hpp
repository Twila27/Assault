#pragma once


class PolarCoords
{
public:

	inline PolarCoords();
	inline PolarCoords( float initialX, float initialY );
	inline PolarCoords( const PolarCoords& coordsToCopy );


public:

	float radius;
	float thetaRadians;
};


//--------------------------------------------------------------------------------------------------------------
// Do-nothing ctor: because it's faster to leave trash values than to allocate and initialize.
inline PolarCoords::PolarCoords( )
	: radius()
	, thetaRadians()
{
}


//--------------------------------------------------------------------------------------------------------------
inline PolarCoords::PolarCoords( float initialRadius, float initialThetaRadians )
	: radius( initialRadius )
	, thetaRadians( initialThetaRadians )
{
}


//--------------------------------------------------------------------------------------------------------------
inline PolarCoords::PolarCoords( const PolarCoords& coordsToCopy )
	: radius( coordsToCopy.radius )
	, thetaRadians( coordsToCopy.thetaRadians )
{
}
