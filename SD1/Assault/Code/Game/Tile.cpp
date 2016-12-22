#include "Game/Tile.hpp"


#include "Game/TileDefinition.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Math/AABB2.hpp"


//--------------------------------------------------------------------------------------------------------------
STATIC const int Tile::NUMBER_TILES_ACROSS_ON_SHEET				= 2;
STATIC const int Tile::NUMBER_TILES_DOWN_ON_SHEET				= 1;
STATIC const int Tile::WIDTH_OF_TILE_ON_SHEET					= 16;
STATIC const int Tile::HEIGHT_OF_TILE_ON_SHEET					= 16;
STATIC SpriteSheet* Tile::m_spriteSheet							= nullptr;


//--------------------------------------------------------------------------------------------------------------
Tile::Tile( const TileCoords& tileCoords, const TileType& tileType )
	: m_tileCoords( tileCoords )
	, m_tileType( tileType )
	, m_tileDefinition( m_tileType )
{
	if ( m_spriteSheet == nullptr ) 
		m_spriteSheet = new SpriteSheet( "Data/Images/Tiles.png", 
		Tile::NUMBER_TILES_ACROSS_ON_SHEET, Tile::NUMBER_TILES_DOWN_ON_SHEET,
		Tile::WIDTH_OF_TILE_ON_SHEET,		Tile::HEIGHT_OF_TILE_ON_SHEET );
}


//--------------------------------------------------------------------------------------------------------------
AABB2 Tile::GetSpriteTexCoords() const
{
	return m_spriteSheet->GetTexCoordsFromSpriteIndex( m_tileDefinition.GetSpriteTexIndex() );
}


//--------------------------------------------------------------------------------------------------------------
TileType Tile::GetTileTypeFromColor( const Rgba& color )
{
	if ( color == Rgba( NOT_REALLY_HALF_OF_FULL_HUE_BUT_WHO_CARES, NOT_REALLY_HALF_OF_FULL_HUE_BUT_WHO_CARES / 2.f, NOT_REALLY_HALF_OF_FULL_HUE_BUT_WHO_CARES / 2.f ) )
	{
		return TileType::STONE;
	}
	else
	{
		return TileType::GRASS;
	}
}

//--------------------------------------------------------------------------------------------------------------
Texture* Tile::GetTilesetTexture() const
{
	return m_spriteSheet->GetAtlasTexture();
}


//--------------------------------------------------------------------------------------------------------------
Tile::~Tile()
{
//I think the below -might- be handled later as part of handling ~Texture() later as said to be waited for by Squirrel.
// 	if ( m_spriteSheet != nullptr )
// 	{
// 		delete m_spriteSheet;
// 		m_spriteSheet = nullptr;
// 	}
}