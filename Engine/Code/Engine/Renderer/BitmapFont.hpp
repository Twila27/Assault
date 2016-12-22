#pragma once


#include <map>
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"


class BitmapFont
{
public:
	static BitmapFont* CreateOrGetFont( const std::string& bitmapFontName );
	AABB2 GetTexCoordsForGlyph( int glyphUnicode ) const;
	Texture* GetFontTexture() const { return m_spriteSheet.GetAtlasTexture();  }

private:
	BitmapFont( const std::string& bitmapFontName );

	static std::map< std::string, BitmapFont* > s_fontRegistry;
	SpriteSheet m_spriteSheet;
};
