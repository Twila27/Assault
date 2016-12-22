#include "Engine/Renderer/BitmapFont.hpp"


#include "Engine/EngineCommon.hpp"

#define STATIC // Do-nothing indicator that method/member is static in class definition


//---------------------------------------------------------------------------
STATIC std::map< std::string, BitmapFont* >	BitmapFont::s_fontRegistry;


//--------------------------------------------------------------------------------------------------------------
BitmapFont::BitmapFont( const std::string& bitmapFontName )
	: m_spriteSheet( bitmapFontName, BITMAP_FONT_GLYPHS_WIDE, BITMAP_FONT_GLYPHS_HIGH, BITMAP_FONT_GLYPH_WIDTH, BITMAP_FONT_GLYPH_HEIGHT )
{
}


//--------------------------------------------------------------------------------------------------------------
BitmapFont* BitmapFont::CreateOrGetFont( const std::string& bitmapFontName )
{
	if ( s_fontRegistry.find( bitmapFontName ) != s_fontRegistry.end( ) ) return s_fontRegistry[ bitmapFontName ];
	else s_fontRegistry[ bitmapFontName ] = new BitmapFont( bitmapFontName );
	
	return s_fontRegistry[ bitmapFontName ];
}


//--------------------------------------------------------------------------------------------------------------
AABB2 BitmapFont::GetTexCoordsForGlyph( int glyphUnicode ) const
{
	return m_spriteSheet.GetTexCoordsFromSpriteIndex( glyphUnicode ); //Assumes ASCII maps directly to indices.
}