#pragma once


#include <vector>

#include "Game/Tile.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/GameCommon.hpp"


//-----------------------------------------------------------------------------
class Entity;
class Vector2;
class PlayerTank;
struct Camera2D;
class Bullet;
class Enemy;
class Explosion;


//-----------------------------------------------------------------------------
class Map
{
public:
	Map( PlayerTank* playerTank, const char* imageFilePath, const Vector2& screenDimensionsInTiles );
	~Map();

	void Update( float deltaSeconds );
	void Render();


private:
	bool HasLineOfSight( const WorldCoords& start, const WorldCoords& end );

	void HandleTileGivenColorAndTileCoords( const Rgba& currentPixelColor, const TileCoords& currentTileCoords );
	void SpawnExplosion( const Vector2& position, ExplosionType type );

	TileCoords GetTileCoordsForTile( const Tile& tile ) const;
	Tile& GetTileFromTileCoords( const TileCoords& tileCoords );
	Tile& GetTileAtIndex(TileIndex index);
	Tile& GetTileFromWorldCoords( const WorldCoords& worldCoords );
	TileCoords GetTileCoordsFromWorldCoords( const WorldCoords& worldCoords ) const;
	WorldCoords GetWorldCoordsForTileCoordsCenter( const TileCoords& tileCoords ) const;
	WorldCoords GetWorldCoordsForTileCoordsMins( const TileCoords& tileCoords ) const;
	WorldCoords GetWorldCoordsForTileCoordsMaxs( const TileCoords& tileCoords ) const;
	TileCoords GeTileCoordsForIndex( TileIndex tileIndex ) const;
	TileIndex GetTileIndexForTileCoords( const TileCoords& tileCoords ) const;
	void PushEntityOutOfSolidTile( Entity* entity );
	void PushEntityOutOfEntity( Entity* ent1, Entity* ent2 );
	bool IsPlayerNearAndSeen( const Enemy& enemy );
	std::vector< Tile > m_tiles;
	TileCoords m_size;
	Vector2 m_screenSizeInTiles;

	std::vector< Bullet* > m_bullets; //Used for collision.
	std::vector< Enemy* > m_enemies; //Used for collision.
	std::vector< Explosion* > m_explosions; //Used for fire-and-forget pattern.
	Camera2D* m_camera;
	PlayerTank* m_playerTank;


	SoundID m_playerBulletFireSoundID;
	SoundID m_enemyBulletFireSoundID;
	SoundID m_bulletHitPlayerSoundID;
	SoundID m_bulletHitEnemySoundID;

	float m_playerAutoFireCooldown;

	static const float TILE_RADIUS_FROM_CENTER;
	static const int RAYCAST_NUM_STEPS;
	static const float PLAYER_RAPID_FIRE_COOLDOWN_SECONDS;
	static const int NUMBER_OF_ENEMY_TANKS;
	static const int NUMBER_OF_ENEMY_TURRETS;
	static const float ENEMY_RANGE_IN_TILES;
	static const float EXPLOSION_DURATION_IN_SECONDS_KNOB;
	static const float EXPLOSION_DURATION_IN_SECONDS_SCALING_KNOB;
	static const float EXPLOSION_SIZE_KNOB;
	static const float EXPLOSION_SIZE_SCALING_KNOB;
};