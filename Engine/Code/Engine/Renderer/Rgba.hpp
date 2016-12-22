#pragma once


struct Rgba
{
	Rgba();
	Rgba( float red, float green, float blue, float alphaOpacity = 1.f );
	Rgba( unsigned char red, unsigned char green, unsigned char blue, unsigned char alphaOpacity = 255 );
	Rgba( const Rgba& other );
	bool operator==( const Rgba& compareTo ) const; //Currently NOT comparing alpha.


	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alphaOpacity;

	static const Rgba BLACK;
	static const Rgba DARK_GRAY;
	static const Rgba GRAY;
	static const Rgba WHITE;
	static const Rgba RED;
	static const Rgba BLUE;
	static const Rgba GREEN;
	static const Rgba MAGENTA;
	static const Rgba YELLOW;
	static const Rgba CYAN;
};