#include "Engine/Math/MathUtils.hpp"

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/PolarCoords.hpp"
#include "Engine/Error/ErrorWarningAssert.hpp"

//-----------------------------------------------------------------------------------------------
// Lookup table for sin() values for 256 evenly distributed angles around the unit circle,
//	where Byte 0 = 0 degrees, Byte 64 = 90 degrees, Byte 128 = 180 degrees, etc.
//
const float g_sinValuesForByteAngles[ 256 ] = {
	0.000000000f,	0.024541229f,	0.049067674f,	0.073564564f,	0.098017140f,	0.122410675f,	0.146730474f,	0.170961889f,	
	0.195090322f,	0.219101240f,	0.242980180f,	0.266712757f,	0.290284677f,	0.313681740f,	0.336889853f,	0.359895037f,	
	0.382683432f,	0.405241314f,	0.427555093f,	0.449611330f,	0.471396737f,	0.492898192f,	0.514102744f,	0.534997620f,	
	0.555570233f,	0.575808191f,	0.595699304f,	0.615231591f,	0.634393284f,	0.653172843f,	0.671558955f,	0.689540545f,	
	0.707106781f,	0.724247083f,	0.740951125f,	0.757208847f,	0.773010453f,	0.788346428f,	0.803207531f,	0.817584813f,	
	0.831469612f,	0.844853565f,	0.857728610f,	0.870086991f,	0.881921264f,	0.893224301f,	0.903989293f,	0.914209756f,	
	0.923879533f,	0.932992799f,	0.941544065f,	0.949528181f,	0.956940336f,	0.963776066f,	0.970031253f,	0.975702130f,	
	0.980785280f,	0.985277642f,	0.989176510f,	0.992479535f,	0.995184727f,	0.997290457f,	0.998795456f,	0.999698819f,	
	1.000000000f,	0.999698819f,	0.998795456f,	0.997290457f,	0.995184727f,	0.992479535f,	0.989176510f,	0.985277642f,	
	0.980785280f,	0.975702130f,	0.970031253f,	0.963776066f,	0.956940336f,	0.949528181f,	0.941544065f,	0.932992799f,	
	0.923879533f,	0.914209756f,	0.903989293f,	0.893224301f,	0.881921264f,	0.870086991f,	0.857728610f,	0.844853565f,	
	0.831469612f,	0.817584813f,	0.803207531f,	0.788346428f,	0.773010453f,	0.757208847f,	0.740951125f,	0.724247083f,	
	0.707106781f,	0.689540545f,	0.671558955f,	0.653172843f,	0.634393284f,	0.615231591f,	0.595699304f,	0.575808191f,	
	0.555570233f,	0.534997620f,	0.514102744f,	0.492898192f,	0.471396737f,	0.449611330f,	0.427555093f,	0.405241314f,	
	0.382683432f,	0.359895037f,	0.336889853f,	0.313681740f,	0.290284677f,	0.266712757f,	0.242980180f,	0.219101240f,	
	0.195090322f,	0.170961889f,	0.146730474f,	0.122410675f,	0.098017140f,	0.073564564f,	0.049067674f,	0.024541229f,	
	0.000000000f,	-0.024541229f,	-0.049067674f,	-0.073564564f,	-0.098017140f,	-0.122410675f,	-0.146730474f,	-0.170961889f,	
	-0.195090322f,	-0.219101240f,	-0.242980180f,	-0.266712757f,	-0.290284677f,	-0.313681740f,	-0.336889853f,	-0.359895037f,	
	-0.382683432f,	-0.405241314f,	-0.427555093f,	-0.449611330f,	-0.471396737f,	-0.492898192f,	-0.514102744f,	-0.534997620f,	
	-0.555570233f,	-0.575808191f,	-0.595699304f,	-0.615231591f,	-0.634393284f,	-0.653172843f,	-0.671558955f,	-0.689540545f,	
	-0.707106781f,	-0.724247083f,	-0.740951125f,	-0.757208847f,	-0.773010453f,	-0.788346428f,	-0.803207531f,	-0.817584813f,	
	-0.831469612f,	-0.844853565f,	-0.857728610f,	-0.870086991f,	-0.881921264f,	-0.893224301f,	-0.903989293f,	-0.914209756f,	
	-0.923879533f,	-0.932992799f,	-0.941544065f,	-0.949528181f,	-0.956940336f,	-0.963776066f,	-0.970031253f,	-0.975702130f,	
	-0.980785280f,	-0.985277642f,	-0.989176510f,	-0.992479535f,	-0.995184727f,	-0.997290457f,	-0.998795456f,	-0.999698819f,	
	-1.000000000f,	-0.999698819f,	-0.998795456f,	-0.997290457f,	-0.995184727f,	-0.992479535f,	-0.989176510f,	-0.985277642f,	
	-0.980785280f,	-0.975702130f,	-0.970031253f,	-0.963776066f,	-0.956940336f,	-0.949528181f,	-0.941544065f,	-0.932992799f,	
	-0.923879533f,	-0.914209756f,	-0.903989293f,	-0.893224301f,	-0.881921264f,	-0.870086991f,	-0.857728610f,	-0.844853565f,	
	-0.831469612f,	-0.817584813f,	-0.803207531f,	-0.788346428f,	-0.773010453f,	-0.757208847f,	-0.740951125f,	-0.724247083f,	
	-0.707106781f,	-0.689540545f,	-0.671558955f,	-0.653172843f,	-0.634393284f,	-0.615231591f,	-0.595699304f,	-0.575808191f,	
	-0.555570233f,	-0.534997620f,	-0.514102744f,	-0.492898192f,	-0.471396737f,	-0.449611330f,	-0.427555093f,	-0.405241314f,	
	-0.382683432f,	-0.359895037f,	-0.336889853f,	-0.313681740f,	-0.290284677f,	-0.266712757f,	-0.242980180f,	-0.219101240f,	
	-0.195090322f,	-0.170961889f,	-0.146730474f,	-0.122410675f,	-0.098017140f,	-0.073564564f,	-0.049067674f,	-0.024541229f
};


//-----------------------------------------------------------------------------------------------
// A replacement for floor(), about 3x faster on my machine.
// Reliable within [-2 billion, +2 billion] or so.  I think.
//
float FastFloor( float f )
{
	if( f >= 0.f )
		return (float) (int) f;
	else
	{
		float f2 = (float) (int) f;
		if( f == f2 )
			return f2;
		else
			return f2 - 1.f;
	}
}


//-----------------------------------------------------------------------------------------------
// A replacement for (int) floor(), about 3x faster on my machine
// Reliable within the range of int.  I think.
//
int FastFloorToInt( float f )
{
	if( f >= 0.f )
		return (int) f;
	else
	{
		int i = (int) f;
		float f2 = (float) i;
		if( f == f2 )
			return i;
		else
			return i-1;
	}
}


//--------------------------------------------------------------------------------------------------------------
float Interpolate( float start, float end, float fractionTowardEnd )
{
	return ( ( end - start ) * fractionTowardEnd ) + start;
}


//--------------------------------------------------------------------------------------------------------------
//Map inValue, from the range inStart to inEnd, to the new range outStart to outEnd.
//
float RangeMap( float inValue, float inStart, float inEnd, float outStart, float outEnd )
{
	if ( inStart == inEnd )
		ERROR_AND_DIE( "RangeMap Division by Zero: inStart == inEnd" );


	float inLength = inEnd - inStart;
	float outLength = outEnd - outStart;
	float inValueToStartLength = inValue - inStart;
	float inValueLengthAlongRange = inValueToStartLength / inLength;

	return ( inValueLengthAlongRange * outLength ) + outStart;
}


//--------------------------------------------------------------------------------------------------------------
float CalcFractionInRange( float inValue, float rangeStart, float rangeEnd )
{
	if ( rangeEnd - rangeStart == 0 ) 
		ERROR_AND_DIE( "CalcFractionInRange Division by Zero: rangeStart == rangeEnd" );


	return (inValue - rangeStart) / (rangeEnd - rangeStart);
}


//--------------------------------------------------------------------------------------------------------------
float ClampFloat( float inValue, float min, float max )
{
	//In case the values came in backwards.
	float newMin = ( min < max ) ? min : max;
	float newMax = ( max > min ) ? max : min;

	if ( inValue < newMin ) return newMin;
	if ( inValue > newMax ) return newMax;
	return inValue;
}


//--------------------------------------------------------------------------------------------------------------
int ClampInt( int inValue, int min, int max )
{
	//In case the values came in backwards.
	int newMin = ( min < max ) ? min : max;
	int newMax = ( max > min ) ? max : min;

	if ( inValue < newMin ) return newMin;
	if ( inValue > newMax ) return newMax;
	return inValue;
}


//--------------------------------------------------------------------------------------------------------------
float ClampFloatZeroToOne( float inValue )
{
	return ClampFloat( inValue, 0.f, 1.f );
}


//--------------------------------------------------------------------------------------------------------------
float ClampFloatNegativeOneToOne( float inValue )
{
	return ClampFloat( inValue, -1.f, 1.f );
}


//--------------------------------------------------------------------------------------------------------------
float WrapNumberWithinCircularRange( float inValue, float minValue, float maxValue )
{
	float wrappedValue = inValue;
	float rangeSize = maxValue - minValue;
	
	while ( wrappedValue < minValue ) wrappedValue += rangeSize;
	while ( wrappedValue >= maxValue ) wrappedValue -= rangeSize;

	return wrappedValue;
}


//--------------------------------------------------------------------------------------------------------------
int WrapNumberWithinCircularRange( int inValue, int minValue, int maxValue )
{
	int wrappedValue = inValue;
	int rangeSize = maxValue - minValue;

	while ( wrappedValue < minValue ) wrappedValue += rangeSize;
	while ( wrappedValue >= maxValue ) wrappedValue -= rangeSize;

	return wrappedValue;
}


//--------------------------------------------------------------------------------------------------------------
float Atan2Degrees( const Vector2& direction )
{
	return ConvertRadiansToDegrees( atan2( direction.y, direction.x ) );
}


//--------------------------------------------------------------------------------------------------------------
float DotProduct( const Vector2& lhs, const Vector2& rhs )
{
	return ( lhs.x * rhs.x ) + ( lhs.y * rhs.y );
}


//--------------------------------------------------------------------------------------------------------------
float DotProduct( const Vector3& lhs, const Vector3& rhs )
{
	return ( lhs.x * rhs.x ) + ( lhs.y * rhs.y ) + ( lhs.z * rhs.z );
}


//--------------------------------------------------------------------------------------------------------------
float DotProduct( const Vector4& lhs, const Vector4& rhs )
{
	return ( lhs.x * rhs.x ) + ( lhs.y * rhs.y ) + ( lhs.z * rhs.z ) + ( lhs.w * rhs.w );
}


//--------------------------------------------------------------------------------------------------------------
float CalcDistBetweenPoints( const Vector2& pos1, const Vector2& pos2 )
{
	float xDistance = pos2.x - pos1.x;
	float yDistance = pos2.y - pos1.y;

	return sqrt( ( xDistance * xDistance ) + ( yDistance * yDistance ) );
}


//--------------------------------------------------------------------------------------------------------------
float CalcDistSquaredBetweenPoints( const Vector2& pos1, const Vector2& pos2 )
{
	float xDistance = pos2.x - pos1.x;
	float yDistance = pos2.y - pos1.y;

	return ( xDistance * xDistance ) + ( yDistance * yDistance );
}


//--------------------------------------------------------------------------------------------------------------
float CalcShortestAngularDisplacement( float fromDegrees, float toDegrees )
{
	float angularDisplacement = toDegrees - fromDegrees;

	while ( angularDisplacement > 180.f ) angularDisplacement -= 360.f;
	while ( angularDisplacement < -180.f ) angularDisplacement += 360.f;

	return angularDisplacement;
}


//--------------------------------------------------------------------------------------------------------------
bool DoDiscsOverlap( const Vector2& center1, float radius1, const Vector2& center2, float radius2 )
{
	float distanceSquared = CalcDistSquaredBetweenPoints( center1, center2 );
	float radiiSum = radius1 + radius2;

	return distanceSquared < ( radiiSum * radiiSum );
}


//--------------------------------------------------------------------------------------------------------------
bool IsPointInDisc( const Vector2& point, const Vector2& discCenter, float discRadius )
{
	if ( ( point - discCenter ).CalcLength() < discRadius ) return true;
	else return false;
}


// --------------------------------------------------------------------------------------------------------------
// float GetRandomFloatZeroToOne()
// {
// 	return rand() / (float)RAND_MAX;
// }
// 
// 
// --------------------------------------------------------------------------------------------------------------
// float GetRandomFloatInRange( float min, float max )
// {
// 	return RangeMap( (float)rand(), 0.f, (float)RAND_MAX, min, max );
// }


//--------------------------------------------------------------------------------------------------------------
int GetRandomIntInRange( float min, float max )
{
	return (int) floor( GetRandomFloatInRange( min, max ) );
}


//--------------------------------------------------------------------------------------------------------------
bool CheckRandomChance( float zeroToOneChanceForSuccess )
{
	return zeroToOneChanceForSuccess >= GetRandomFloatZeroToOne();
}
