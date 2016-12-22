#pragma once


class IntVector2
{
public:

	inline IntVector2();
	inline IntVector2( int initialX, int initialY );
	inline IntVector2( const IntVector2& vectorToCopy );

	inline const IntVector2 operator*( float scalarToScaleBy ) const;
	inline const IntVector2 operator-( const IntVector2& vectorToAdd ) const;
	inline bool operator<( const IntVector2& compareTo ) const;
	inline bool operator==( const IntVector2& compareTo ) const;
	inline bool operator!=( const IntVector2& compareTo ) const;


public:

	int x;
	int y;
	static const IntVector2 ZERO;
	static const IntVector2 ONE;
};


//--------------------------------------------------------------------------------------------------------------
// Do-nothing default ctor: because it saves time to leave trash values rather than allocate and initialize.
inline IntVector2::IntVector2()
	: x()
	, y()
{
}


//--------------------------------------------------------------------------------------------------------------
inline IntVector2::IntVector2( int initialX, int initialY )
	: x( initialX )
	, y( initialY )
{
}


//--------------------------------------------------------------------------------------------------------------
inline IntVector2::IntVector2( const IntVector2& vectorToCopy )
	: x( vectorToCopy.x )
	, y( vectorToCopy.y )
{
}


//--------------------------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator*( float scalarToScaleBy ) const
{
	return IntVector2( x * (int)scalarToScaleBy, y * (int)scalarToScaleBy );
}


//--------------------------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator-( const IntVector2& vectorToAdd ) const
{
	return IntVector2( x - vectorToAdd.x, y - vectorToAdd.y );
}


//--------------------------------------------------------------------------------------------------------------
inline bool IntVector2::operator<( const IntVector2& compareTo ) const
{
	if ( this->x < compareTo.x ) return true; //First on x,
	if ( this->x > compareTo.x ) return false;
	return this->y < compareTo.y; //Second ties broken on y.
}


//--------------------------------------------------------------------------------------------------------------
inline bool IntVector2::operator==( const IntVector2& compareTo ) const
{
	return ( x == compareTo.x ) && ( y == compareTo.y );
}


//--------------------------------------------------------------------------------------------------------------
inline bool IntVector2::operator!=( const IntVector2& compareTo ) const
{
	return !(*this == compareTo);
}
