#include "Engine/Renderer/Rgba.hpp"
#include "Engine/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"



STATIC const Rgba Rgba::BLACK = Rgba( 0.f, 0.f, 0.f );
STATIC const Rgba Rgba::DARK_GRAY = Rgba( 0.27f, 0.27f, 0.27f );
STATIC const Rgba Rgba::GRAY = Rgba( 0.5f, 0.5f, 0.5f );
STATIC const Rgba Rgba::WHITE = Rgba( 1.f, 1.f, 1.f );
STATIC const Rgba Rgba::RED = Rgba( 1.f, 0.f, 0.f );
STATIC const Rgba Rgba::GREEN = Rgba( 0.f, 1.f, 0.f );
STATIC const Rgba Rgba::BLUE = Rgba( 0.f, 0.f, 1.f );
STATIC const Rgba Rgba::MAGENTA = Rgba( 1.f, 0.f, 1.f );
STATIC const Rgba Rgba::YELLOW = Rgba( 1.f, 1.f, 0.f );
STATIC const Rgba Rgba::CYAN = Rgba( 0.f, 1.f, 1.f );


//--------------------------------------------------------------------------------------------------------------
Rgba::Rgba()
	: red( 255 )
	, green( 255 )
	, blue( 255 )
	, alphaOpacity( 255 )
{
}


//--------------------------------------------------------------------------------------------------------------
Rgba::Rgba( float red, float green, float blue, float alphaOpacity /*=1.f*/ )
{
	this->red = static_cast<unsigned char>( RangeMap( red, 0.f, 1.f, 0.f, 255.f ) );
	this->green = static_cast<unsigned char>( RangeMap( green, 0.f, 1.f, 0.f, 255.f ) );
	this->blue = static_cast<unsigned char>( RangeMap( blue, 0.f, 1.f, 0.f, 255.f ) );
	this->alphaOpacity = static_cast<unsigned char>( RangeMap( alphaOpacity, 0.f, 1.f, 0.f, 255.f ) );
}


//--------------------------------------------------------------------------------------------------------------
Rgba::Rgba( unsigned char red, unsigned char green, unsigned char blue, unsigned char alphaOpacity /*= 255 */ )
	: red( red )
	, blue( blue )
	, green( green )
	, alphaOpacity( alphaOpacity )	
{
}


//--------------------------------------------------------------------------------------------------------------
Rgba::Rgba( const Rgba& other )
	: red( other.red )
	, green( other.green )
	, blue( other.blue )
	, alphaOpacity( other.alphaOpacity )
{
}


//--------------------------------------------------------------------------------------------------------------
bool Rgba::operator==( const Rgba& compareTo ) const
{
	return ( red == compareTo.red ) && ( green == compareTo.green ) && ( blue == compareTo.blue );
}
