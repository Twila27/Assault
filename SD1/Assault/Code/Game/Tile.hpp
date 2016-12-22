#pragma once


#include "Engine/Renderer/Rgba.hpp"
#include "Game/GameCommon.hpp"
#include "Game/TileDefinition.hpp"


//-----------------------------------------------------------------------------
class SpriteSheet;
class Texture;
class AABB2;


//-----------------------------------------------------------------------------
class Tile
{
public:
	Tile( const TileCoords& tileCoords, const TileType& tileType );
	~Tile();

	bool IsSolid() const { return m_tileDefinition.isSolid(); }
	Rgba GetColor() const { return m_tileDefinition.GetColor(); }
	AABB2 GetSpriteTexCoords() const; //Recall these are 0-1 normalized.
	TileCoords GetTileCoords() const { return m_tileCoords; }
	TileType GetTileType() const { return m_tileType; }
	static TileType GetTileTypeFromColor( const Rgba& color );
	void SetTileType( TileType newType ) { m_tileType = newType; m_tileDefinition = TileDefinition(newType);  }
	Texture* GetTilesetTexture() const;

	WorldCoords GetWorldMins() const { return Vector2( static_cast<float>( m_tileCoords.x ), static_cast<float>( m_tileCoords.y ) ); }
	WorldCoords GetWorldMaxs() const { return GetWorldMins() + WorldCoords( 1.f, 1.f ); }
	WorldCoords GetWorldSEs() const { return GetWorldMins() + WorldCoords( 1.f, 0.f ); }
	WorldCoords GetWorldNWs() const { return GetWorldMins() + WorldCoords( 0.f, 1.f ); }


private:
	TileType m_tileType;
	TileDefinition m_tileDefinition;
	TileCoords m_tileCoords;
	static SpriteSheet* m_spriteSheet;

	static const int NUMBER_TILES_ACROSS_ON_SHEET;
	static const int NUMBER_TILES_DOWN_ON_SHEET;
	static const int WIDTH_OF_TILE_ON_SHEET;
	static const int HEIGHT_OF_TILE_ON_SHEET;
};