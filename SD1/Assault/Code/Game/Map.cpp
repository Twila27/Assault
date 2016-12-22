#include "Game/Map.hpp"


#include <algorithm>

#include "Engine/Renderer/RenderCommand.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Audio/TheAudio.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Error/ErrorWarningAssert.hpp"
#include "Engine/Renderer/Texture.hpp"

#define STBI_HEADER_FILE_ONLY
#include "ThirdParty/stb/stb_image.c"

#include "Game/Camera2D.hpp"
#include "Game/PlayerTank.hpp"
#include "Game/Bullet.hpp"
#include "Game/TheApp.hpp"
#include "Game/EnemyTurret.hpp"
#include "Game/EnemyTank.hpp"
#include "Game/Explosion.hpp"


//--------------------------------------------------------------------------------------------------------------
STATIC const float Map::TILE_RADIUS_FROM_CENTER							= .5f;
STATIC const int Map::RAYCAST_NUM_STEPS									= 100;
STATIC const float Map::PLAYER_RAPID_FIRE_COOLDOWN_SECONDS				= 1.f / 8.f;
STATIC const float Map::ENEMY_RANGE_IN_TILES							= 3.f;
STATIC const int Map::NUMBER_OF_ENEMY_TURRETS							= 10;
STATIC const int Map::NUMBER_OF_ENEMY_TANKS								= 10;

//Scaled once per step up from a small explosion.
STATIC const float Map::EXPLOSION_DURATION_IN_SECONDS_KNOB				= .5f;
STATIC const float Map::EXPLOSION_DURATION_IN_SECONDS_SCALING_KNOB		= 2.f; 
STATIC const float Map::EXPLOSION_SIZE_KNOB								= 1.f;
STATIC const float Map::EXPLOSION_SIZE_SCALING_KNOB						= 2.f; 


//--------------------------------------------------------------------------------------------------------------
Map::Map( PlayerTank* playerTank, const char* imageFilePath, const Vector2& screenDimensionsInTiles )
	: m_screenSizeInTiles( screenDimensionsInTiles )
	, m_playerAutoFireCooldown( 0.f )
	, m_playerTank( playerTank )
	, m_camera( new Camera2D() )
{
	m_playerBulletFireSoundID = g_theAudio->CreateOrGetSound( "Data/Audio/CannonFire_Whistle1.wav" );
	m_enemyBulletFireSoundID = g_theAudio->CreateOrGetSound( "Data/Audio/Cannon_Turret_Shot_2.wav" );
	m_bulletHitPlayerSoundID = g_theAudio->CreateOrGetSound( "Data/Audio/Explo_Electric1.wav" );
	m_bulletHitEnemySoundID = g_theAudio->CreateOrGetSound( "Data/Audio/Hit_LargeMetalImpact2.wav" );

	m_camera->m_centeredOnScreenPosition = Vector2( screenDimensionsInTiles.x * 0.5f, screenDimensionsInTiles.y * 0.2f );

	//Note map size is not set until stbi_load call.
	int numComponents = 0; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
	int numComponentsRequested = 3; // don't care; we support 3 (RGB) or 4 (RGBA)
	unsigned char* imageData = stbi_load( imageFilePath, &m_size.x, &m_size.y, &numComponents, numComponentsRequested );

	m_playerTank->SetPosition( static_cast<float>( m_size.x ) * 0.5f, static_cast<float>( m_size.y ) * 0.5f );

	//Pushing back tiles starting from the bottom left.
	for ( int y = 0; y < m_size.y; ++y )
	{
		for ( int x = 0; x < m_size.x; ++x )
		{
			TileCoords tc = TileCoords( x, y );

			//Forces to solid if tile is along outer ring.
			if ( x == 0 || x == ( m_size.x - 1 ) || y == 0 || y == ( m_size.y - 1 ) ) m_tiles.push_back( Tile( tc, TileType::STONE ) );
			else m_tiles.push_back( Tile( tc, TileType::GRASS ) );
		}
	}

	//Iterating tiles starting from the top left.
	int numTexels = m_size.x * m_size.y;
	int numBytes = numTexels * numComponents;
	for ( int byteIndex = 0; byteIndex < numBytes; byteIndex += numComponents ) //For each pixel (skips alpha channel if present):
	{
		float rangeMappedRed = RangeMap( imageData[ byteIndex ], 0.f, 255.f, 0.f, 1.f ); //256 so that 128 is .5f.
		float rangeMappedGreen = RangeMap( imageData[ byteIndex+1 ], 0.f, 255.f, 0.f, 1.f );
		float rangeMappedBlue = RangeMap( imageData[ byteIndex+2 ], 0.f, 255.f, 0.f, 1.f );
		Rgba currentPixel = Rgba( rangeMappedRed, rangeMappedGreen, rangeMappedBlue ); //R, G, B.


		int texelIndex = byteIndex / numComponents;
		int currentTileCoordX = texelIndex % m_size.x;
		int currentTileCoordY = texelIndex / m_size.x;
		currentTileCoordY = m_size.y - currentTileCoordY - 1; // Flip vertically
//		int verticalOffset = byteIndex / ( ( m_size.y * numComponents ) + m_size.x );
//		int currentTileCoordY = ( m_size.y - 1 ) - verticalOffset;

		Map::HandleTileGivenColorAndTileCoords( currentPixel, TileCoords( currentTileCoordX, currentTileCoordY ) );
	}
	stbi_image_free( imageData );
}


//--------------------------------------------------------------------------------------------------------------
Map::~Map()
{
	delete m_camera;

	//PlayerTank deleted by TheGame.

	for ( auto bulletIter = m_bullets.rbegin( ); bulletIter != m_bullets.rend( ); ++bulletIter )
	{
		if ( *bulletIter != nullptr )
		{
			delete *bulletIter;
			*bulletIter = nullptr;
		}
	}

	for ( auto enemyIter = m_enemies.rbegin( ); enemyIter != m_enemies.rend( ); ++enemyIter )
	{
		if ( *enemyIter != nullptr )
		{
			delete *enemyIter;
			*enemyIter = nullptr;
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
void Map::Render()
{
	g_theRenderer->SetOrtho( Vector2( 0.f, 0.f ), m_screenSizeInTiles );

	g_theRenderer->PushView();

	//Change of basis detour theorem's triple product to transform world-to-camera.
	g_theRenderer->TranslateView( m_camera->m_centeredOnScreenPosition );
	g_theRenderer->RotateViewByDegrees( m_camera->m_orientationDegrees * -1);
	g_theRenderer->TranslateView( m_camera->m_worldPosition * -1 );

	for ( auto tileIter = m_tiles.cbegin(); tileIter != m_tiles.cend(); ++tileIter )
		g_theRenderer->DrawAABB(
		TheRenderer::VertexGroupingRule::AS_QUADS,
		AABB2(	tileIter->GetTileCoords().x,		tileIter->GetTileCoords().y,
				tileIter->GetTileCoords().x + 1,	tileIter->GetTileCoords().y + 1 ),
				*tileIter->GetTilesetTexture(),
				tileIter->GetSpriteTexCoords()
		);

	m_playerTank->Render(); //Because of above transforms, renders (initially) centered where we want it.

	for ( int i = 0; i < (int)m_bullets.size(); i++ ) 
		m_bullets[ i ]->Render();

	for ( int i = 0; i < (int)m_enemies.size(); i++ ) 
		m_enemies[ i ]->Render();
	
	for ( int i = 0; i < (int)m_explosions.size(); i++ ) 
		m_explosions[ i ]->Render(); //Last for z-ordering.

	//Draw debug commands.
	auto commandIterEnd = g_theRenderCommands->end();
	for ( auto commandIter = g_theRenderCommands->begin(); commandIter != commandIterEnd; )
	{
		RenderCommand* currentCommand = *commandIter;

		currentCommand->Render();

		if ( currentCommand->IsExpired() ) //Expire after draw or 1-frame commands wouldn't show.
		{
			commandIter = g_theRenderCommands->erase( commandIter );

			delete currentCommand;
			currentCommand = nullptr;
		}
		else ++commandIter;
	}

	g_theRenderer->PopView();
}


//--------------------------------------------------------------------------------------------------------------
void Map::Update( float deltaSeconds )
{
	//-----------------------------------------------------------------------------
	//First, player updates, and camera to them.
	//
	if ( m_playerTank->IsAlive() )
	{
		m_playerTank->Update( deltaSeconds );
		PushEntityOutOfSolidTile( m_playerTank );
		bool controllerFiring = ( g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ] != nullptr )
			&& ( g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ]->GetRightTrigger() > 0.f );
		if ( g_theApp->isKeyDown( VK_SPACE ) || controllerFiring )
		{
			if ( g_theApp->WasKeyJustPressed( VK_SPACE ) || m_playerAutoFireCooldown >= PLAYER_RAPID_FIRE_COOLDOWN_SECONDS )
			{
				m_playerAutoFireCooldown = 0.f;
				Bullet *b = new Bullet( m_playerTank->GetPosition() + m_playerTank->GetLocalTipPosition(), m_playerTank->GetOrientation(), Bullet::PLAYER );
				m_bullets.push_back( b );
				b = nullptr;

				g_theAudio->PlaySound( m_playerBulletFireSoundID, VOLUME_ADJUST );
			}
			else m_playerAutoFireCooldown += deltaSeconds;
		}
		m_camera->m_worldPosition = m_playerTank->GetPosition();
		m_camera->m_orientationDegrees = m_playerTank->GetOrientation() - 90.f; //Should probably just make Entity orientation in degrees to avoid per-frame division.
	}

	//-----------------------------------------------------------------------------
	//Second, enemy reacts to updated player.
	//
	for ( auto enemyIter = m_enemies.begin( ); enemyIter != m_enemies.end( ); )
	{
		Enemy* currentEnemy = *enemyIter;

		if ( m_playerTank->IsAlive( ) && IsPlayerNearAndSeen( *currentEnemy ) )
		{
			currentEnemy->ReactToPlayer( *m_playerTank );
			if ( currentEnemy->IsShooting( ) && ( currentEnemy->IsWaitingForFiringCooldown( ) == false ) )
			{
				Bullet *b = new Bullet(
					currentEnemy->GetPosition() + currentEnemy->GetLocalTipPosition(),
					currentEnemy->GetOrientation(),
					Bullet::ENEMY
				);
				m_bullets.push_back( b );
				b = nullptr;

				g_theAudio->PlaySound( m_enemyBulletFireSoundID, VOLUME_ADJUST );
			}
		}
		else
		{
			if ( currentEnemy->IsWaitingForMotionCooldown( ) == false ) currentEnemy->FindNewGoalOrientation( );
			currentEnemy->ReactToNothing( );
		}

		currentEnemy->Update( deltaSeconds );

		//Entity-Entity physics: first versus player, then versus other enemies.
		PushEntityOutOfEntity( currentEnemy, m_playerTank );
		for ( auto enemyIter2 = m_enemies.begin(); enemyIter2 != m_enemies.end(); ++enemyIter2 )
		{
			if ( enemyIter == enemyIter2 ) continue;
			Enemy* enemyToCheckAgainst = *enemyIter2;
			PushEntityOutOfEntity( currentEnemy, enemyToCheckAgainst );
		}

		//Entity-Tile physics.
		PushEntityOutOfSolidTile( currentEnemy );

		Vector3 playerPos = Vector3( m_playerTank->GetPosition().x, m_playerTank->GetPosition().y, 0.f );

		if ( currentEnemy->IsAlive( ) == false )
		{
			SpawnExplosion( currentEnemy->GetPosition(), ExplosionType::MEDIUM );

			delete currentEnemy;
			enemyIter = m_enemies.erase( enemyIter );
		}
		else ++enemyIter;
	}

	//-----------------------------------------------------------------------------
	//Third, bullets update, hit walls (or enemies/player if player/enemy bullet) and die.
	//
	for ( auto bulletIter = m_bullets.begin( ); bulletIter != m_bullets.end( ); )
	{
		Bullet* currentBullet = *bulletIter;

		currentBullet->Update( deltaSeconds );


		switch ( currentBullet->GetBulletType( ) )
		{
			case Bullet::BulletType::PLAYER: 
				for ( auto enemyIter = m_enemies.cbegin(); enemyIter != m_enemies.cend(); ++enemyIter )
				{
					Enemy* currentEnemy = *enemyIter;
					if ( currentBullet->DoesOverlap( *currentEnemy ) )
					{
						currentBullet->DecreaseHealth();
						currentEnemy->DecreaseHealth();
						g_theAudio->PlaySound( m_bulletHitEnemySoundID, VOLUME_ADJUST );
					}
				}
				break;

			case Bullet::BulletType::ENEMY: 
				if ( currentBullet->DoesOverlap( *m_playerTank ) )
				{
					currentBullet->DecreaseHealth( );
					m_playerTank->DecreaseHealth();
					g_theAudio->PlaySound( m_bulletHitPlayerSoundID, VOLUME_ADJUST );

					if ( m_playerTank->IsAlive() == false ) 
						SpawnExplosion( m_playerTank->GetPosition(), ExplosionType::LARGE );
				}
				break;
		}


		if ( GetTileFromWorldCoords( currentBullet->GetPosition( ) ).IsSolid( ) )
			currentBullet->SetIsAlive( false );


		if ( currentBullet->IsAlive( ) == false )
		{
			SpawnExplosion( currentBullet->GetPosition( ), ExplosionType::SMALL );

			delete currentBullet;
			bulletIter = m_bullets.erase( bulletIter );

			//g_theAudio->PlaySound( m_bulletHitSoundID, VOLUME_ADJUST );
		}
		else ++bulletIter;
	}


	//-----------------------------------------------------------------------------
	//Lastly, explosions update.
	//
	for ( auto boomIter = m_explosions.begin(); boomIter != m_explosions.end(); )
	{
		Explosion* currentBoom = *boomIter;
		currentBoom->Update( deltaSeconds );

		if ( currentBoom->IsFinished() )
		{
			delete currentBoom;
			boomIter = m_explosions.erase( boomIter );
		}
		else ++boomIter;
	}


	//!\\ Update debug commands.
	auto commandIterEnd = g_theRenderCommands->end();
	for ( auto commandIter = g_theRenderCommands->begin(); commandIter != commandIterEnd; ++commandIter )
	{
		RenderCommand* currentCommand = *commandIter;
		currentCommand->Update( deltaSeconds );
	}
}


//--------------------------------------------------------------------------------------------------------------
bool Map::HasLineOfSight( const WorldCoords& start, const WorldCoords& end )
{
	//"Step and Sample" Poor Man's 2D Tile-based Raycast Algorithm
	//
	const int NUM_STEPS_INT = Map::RAYCAST_NUM_STEPS;
	const float NUM_STEPS_FLOAT = static_cast<float>( NUM_STEPS_INT );

	WorldCoords displacement = end - start;
	WorldCoords stepAlongDisplacement = displacement / NUM_STEPS_FLOAT;
	WorldCoords currentPos = start;

	for ( int i = 0; i < NUM_STEPS_INT; i++ )
	{
		currentPos += stepAlongDisplacement;
		if ( GetTileFromWorldCoords( currentPos ).IsSolid() ) return false;		
	}

	return true;
}


//--------------------------------------------------------------------------------------------------------------
//Sets tile and handles addition of whatever entities may be symbolized through the color.
void Map::HandleTileGivenColorAndTileCoords( const Rgba& currentPixelColor, const TileCoords& currentTileCoords )
{
	TileType currentTileType;
	currentTileType = Tile::GetTileTypeFromColor( currentPixelColor ); //Handle tile-only colors. MAYBE MOVE CODE TO HERE?

	//Some logic adds a tile straightforwardly, others may need entity placement.
	//e.g. Where to place tank or enemy.

	Tile& currentTile = GetTileFromTileCoords( currentTileCoords );
	WorldCoords currentTileCenterInWorld = GetWorldCoordsForTileCoordsCenter( currentTile.GetTileCoords() );

	//Place entities on tile: white for player, black for enemy tanks, gray for enemy turrets.
	if ( currentPixelColor == Rgba( 0.f, 0.f, 0.f ) )
	{
		m_enemies.push_back( new EnemyTank() );
		m_enemies.back( )->SetPosition( currentTileCenterInWorld );
	}
	else if ( currentPixelColor == Rgba( NOT_REALLY_HALF_OF_FULL_HUE_BUT_WHO_CARES, NOT_REALLY_HALF_OF_FULL_HUE_BUT_WHO_CARES, NOT_REALLY_HALF_OF_FULL_HUE_BUT_WHO_CARES ) )
	{
		m_enemies.push_back( new EnemyTurret() );
		m_enemies.back( )->SetPosition( currentTileCenterInWorld );
	}
	else if ( currentPixelColor == Rgba( 1.f, 1.f, 1.f ) )
	{
		m_playerTank->SetPosition( currentTileCenterInWorld );
	}

	currentTile.SetTileType( currentTileType );
}


//--------------------------------------------------------------------------------------------------------------
void Map::SpawnExplosion( const Vector2& position, ExplosionType type )
{
	float cosmeticRadius = Map::EXPLOSION_SIZE_KNOB;
	float durationSeconds = Map::EXPLOSION_DURATION_IN_SECONDS_KNOB;
	for ( int i = 0; i < (int)type; i++ )
	{
		cosmeticRadius *= Map::EXPLOSION_SIZE_SCALING_KNOB;
		durationSeconds *= Map::EXPLOSION_DURATION_IN_SECONDS_SCALING_KNOB;
	}
	m_explosions.push_back( new Explosion( position, cosmeticRadius, durationSeconds ) );
}

//--------------------------------------------------------------------------------------------------------------
bool Map::IsPlayerNearAndSeen( const Enemy& enemy )
{
	Vector2 displacement = m_playerTank->GetPosition() - enemy.GetPosition();

	if ( displacement.CalcLength() > Map::ENEMY_RANGE_IN_TILES ) return false;
	else return HasLineOfSight( enemy.GetPosition(), m_playerTank->GetPosition() );
}


//--------------------------------------------------------------------------------------------------------------
void Map::PushEntityOutOfSolidTile( Entity* entity )
{
	Tile currentTile = GetTileFromWorldCoords( entity->GetPosition() ); //Typically which tile entity's -center- is in.

	//Tank is at most one tile in size, so can touch at most 4 tiles, and only 1 in any direction.

	Tile tileOnLeft = GetTileFromTileCoords( TileCoords( currentTile.GetTileCoords().x - 1, currentTile.GetTileCoords().y ) ); //Checking against left.
	if ( tileOnLeft.GetTileType() == TileType::STONE )
	{
		float overlap = ( tileOnLeft.GetTileCoords().x + 1 ) - ( entity->GetPosition().x - entity->GetPhysicsRadius() );
		if ( overlap > 0.f ) entity->SetPositionX( entity->GetPosition().x + overlap );
	}
	
	Tile tileOnRight = GetTileFromTileCoords( TileCoords( currentTile.GetTileCoords().x + 1, currentTile.GetTileCoords().y ) ); //Checking against right.
	if ( tileOnRight.GetTileType() == TileType::STONE )
	{
		float overlap = ( entity->GetPosition().x + entity->GetPhysicsRadius() ) - tileOnRight.GetTileCoords().x;
		if ( overlap > 0.f ) entity->SetPositionX( entity->GetPosition().x - overlap );
	}
	
	Tile tileBelow = GetTileFromTileCoords( TileCoords( currentTile.GetTileCoords().x, currentTile.GetTileCoords().y - 1 ) ); //Checking against bottom.
	if ( tileBelow.GetTileType() == TileType::STONE )
	{
		float overlap = ( tileBelow.GetTileCoords().y + 1 ) - ( entity->GetPosition().y - entity->GetPhysicsRadius() );
		if ( overlap > 0.f ) entity->SetPositionY( entity->GetPosition().y + overlap );
	}
	
	Tile tileAbove = GetTileFromTileCoords( TileCoords( currentTile.GetTileCoords().x, currentTile.GetTileCoords().y + 1 ) ); //Checking against top.
	if ( tileAbove.GetTileType() == TileType::STONE )
	{
		float overlap = ( entity->GetPosition().y + entity->GetPhysicsRadius() ) - ( tileAbove.GetTileCoords().y );
		if ( overlap > 0.f ) entity->SetPositionY( entity->GetPosition().y - overlap );
	}



	Tile tileToNE = GetTileFromTileCoords( TileCoords( currentTile.GetTileCoords().x + 1, currentTile.GetTileCoords().y + 1 ) ); //Checking against NE's SW corner.
	if ( tileToNE.GetTileType() == TileType::STONE && IsPointInDisc( tileToNE.GetWorldMins(), m_playerTank->GetPosition(), m_playerTank->GetPhysicsRadius() ) )
	{
		//Pushing out at diagonal--i.e. both directions at once--feels best, hence pushDir is Vector2.
		Vector2 pushDir = m_playerTank->GetPosition() - tileToNE.GetWorldMins();
		float distFromTankCenterToCorner = pushDir.CalcLength();

		//To get the actual length of overlap, subtract distance along physics radius OUTSIDE tile (i.e. to corner).
		float pushDist = m_playerTank->GetPhysicsRadius() - distFromTankCenterToCorner;

		//To get length of push, we'd scale the push's pure (normalized) direction * distance between hit corner and tank center.
		pushDir.Normalize();
		pushDir *= pushDist;

		//Actual push-back occurs here.
		m_playerTank->SetPosition( m_playerTank->GetPosition() + pushDir );
	}
	
	Tile tileToSE = GetTileFromTileCoords( TileCoords( currentTile.GetTileCoords().x + 1, currentTile.GetTileCoords().y - 1 ) ); //Checking against SE's NW corner.
	if ( tileToSE.GetTileType() == TileType::STONE && IsPointInDisc( tileToSE.GetWorldNWs(), m_playerTank->GetPosition(), m_playerTank->GetPhysicsRadius() ) )
	{
		//Pushing out at diagonal--i.e. both directions at once--feels best, hence pushDir is Vector2.
		Vector2 pushDir = m_playerTank->GetPosition() - tileToSE.GetWorldNWs();
		float distFromTankCenterToCorner = pushDir.CalcLength();

		//To get the actual length of overlap, subtract distance along physics radius OUTSIDE tile (i.e. to corner).
		float pushDist = m_playerTank->GetPhysicsRadius() - distFromTankCenterToCorner;

		//To get length of push, we'd scale the push's pure (normalized) direction * distance between hit corner and tank center.
		pushDir.Normalize();
		pushDir *= pushDist;

		//Actual push-back occurs here.
		m_playerTank->SetPosition( m_playerTank->GetPosition() + pushDir );
	}
	
	Tile tileToSW = GetTileFromTileCoords( TileCoords( currentTile.GetTileCoords().x - 1, currentTile.GetTileCoords().y - 1 ) ); //Checking against SW's NE corner.
	if ( tileToSW.GetTileType() == TileType::STONE && IsPointInDisc( tileToSW.GetWorldMaxs(), m_playerTank->GetPosition(), m_playerTank->GetPhysicsRadius() ) )
	{
		//Pushing out at diagonal--i.e. both directions at once--feels best, hence pushDir is Vector2.
		Vector2 pushDir = m_playerTank->GetPosition() - tileToSW.GetWorldMaxs();
		float distFromTankCenterToCorner = pushDir.CalcLength();

		//To get the actual length of overlap, subtract distance along physics radius OUTSIDE tile (i.e. to corner).
		float pushDist = m_playerTank->GetPhysicsRadius() - distFromTankCenterToCorner;

		//To get length of push, we'd scale the push's pure (normalized) direction * distance between hit corner and tank center.
		pushDir.Normalize();
		pushDir *= pushDist;

		//Actual push-back occurs here.
		m_playerTank->SetPosition( m_playerTank->GetPosition() + pushDir );
	}
	
	Tile tileToNW = GetTileFromTileCoords( TileCoords( currentTile.GetTileCoords().x - 1, currentTile.GetTileCoords().y + 1 ) ); //Checking against NW's SE corner.
	if ( tileToNW.GetTileType() == TileType::STONE && IsPointInDisc( tileToNW.GetWorldSEs(), m_playerTank->GetPosition(), m_playerTank->GetPhysicsRadius() ) )
	{
		//Pushing out at diagonal--i.e. both directions at once--feels best, hence pushDir is Vector2.
		Vector2 pushDir = m_playerTank->GetPosition() - tileToNW.GetWorldSEs();
		float distFromTankCenterToCorner = pushDir.CalcLength();

		//To get the actual length of overlap, subtract distance along physics radius OUTSIDE tile (i.e. to corner).
		float pushDist = m_playerTank->GetPhysicsRadius() - distFromTankCenterToCorner;

		//To get length of push, we'd scale the push's pure (normalized) direction * distance between hit corner and tank center.
		pushDir.Normalize();
		pushDir *= pushDist;

		//Actual push-back occurs here.
		m_playerTank->SetPosition( m_playerTank->GetPosition() + pushDir );
	}
}


//--------------------------------------------------------------------------------------------------------------
void Map::PushEntityOutOfEntity( Entity* ent1, Entity* ent2 )
{
	if ( ent1->DoesOverlap( *ent2 ) == false ) return;

	Vector2 overlapDir = ent1->GetPosition() - ent2->GetPosition();
	if (overlapDir.CalcLength() != 0) overlapDir.Normalize();
	
	Vector2 edgeOfOverlap1 = ent1->GetPosition() + overlapDir * ent1->GetPhysicsRadius();
	Vector2 edgeOfOverlap2 = ent2->GetPosition() + overlapDir * ent2->GetPhysicsRadius();
	Vector2 amountOverlap = edgeOfOverlap1 - edgeOfOverlap2;
	Vector2 amountPushOut = amountOverlap / 64.f; //Soft pushout.
	ent1->SetPosition( ent1->GetPosition() + amountPushOut );
	ent2->SetPosition( ent2->GetPosition() - amountPushOut );
}

//--------------------------------------------------------------------------------------------------------------
TileCoords Map::GetTileCoordsForTile( const Tile& tile ) const
{
	return tile.GetTileCoords();
}


//--------------------------------------------------------------------------------------------------------------
Tile& Map::GetTileFromTileCoords( const TileCoords& tileCoords )
{
	ASSERT_OR_DIE( tileCoords.x >= 0 && tileCoords.x < m_size.x 
				&& tileCoords.y >= 0 && tileCoords.y < m_size.y,
		"TileCoords Outside Map Bounds!\n");

	return m_tiles[ GetTileIndexForTileCoords( tileCoords ) ];
}


//--------------------------------------------------------------------------------------------------------------
Tile& Map::GetTileAtIndex(TileIndex tileIndex)
{
	ASSERT_OR_DIE( tileIndex >= 0 && tileIndex < (int) m_tiles.size(),
		"TileIndex Outside Map Bounds!\n" );

	return m_tiles[ tileIndex ];
}


//--------------------------------------------------------------------------------------------------------------
Tile& Map::GetTileFromWorldCoords( const WorldCoords& worldCoords )
{
	ASSERT_OR_DIE( worldCoords.x >= 0.f && worldCoords.x < (float) m_size.x 
				&& worldCoords.y >= 0.f && worldCoords.y < (float) m_size.y,
		"WorldCoords Outside Map Bounds!\n" );

	return m_tiles[ GetTileIndexForTileCoords( GetTileCoordsFromWorldCoords( worldCoords ) ) ];
}


//--------------------------------------------------------------------------------------------------------------
TileCoords Map::GetTileCoordsFromWorldCoords( const WorldCoords& worldCoords ) const
{
	ASSERT_OR_DIE( worldCoords.x >= 0.f && worldCoords.x < (float)m_size.x
				&& worldCoords.y >= 0.f && worldCoords.y < (float)m_size.y,
		"WorldCoords Outside Map Bounds!\n" );

	return TileCoords( (int)floor( worldCoords.x ), (int)floor( worldCoords.y ) );
}


//--------------------------------------------------------------------------------------------------------------
WorldCoords Map::GetWorldCoordsForTileCoordsCenter( const TileCoords& tileCoords ) const
{
	ASSERT_OR_DIE( tileCoords.x >= 0 && tileCoords.x < m_size.x
				&& tileCoords.y >= 0 && tileCoords.y < m_size.y,
		"TileCoords Outside Map Bounds!\n" );

	return GetWorldCoordsForTileCoordsMins( tileCoords ) + WorldCoords( .5f, .5f );
}


//--------------------------------------------------------------------------------------------------------------
WorldCoords Map::GetWorldCoordsForTileCoordsMins( const TileCoords& tileCoords ) const
{
	ASSERT_OR_DIE( tileCoords.x >= 0 && tileCoords.x < m_size.x
				&& tileCoords.y >= 0 && tileCoords.y < m_size.y,
		"TileCoords Outside Map Bounds!\n" );

	return WorldCoords( (float)floor( tileCoords.x ), (float)floor( tileCoords.y ) );
}


//--------------------------------------------------------------------------------------------------------------
WorldCoords Map::GetWorldCoordsForTileCoordsMaxs( const TileCoords& tileCoords ) const
{
	ASSERT_OR_DIE( tileCoords.x >= 0 && tileCoords.x < m_size.x
				&& tileCoords.y >= 0 && tileCoords.y < m_size.y,
		"TileCoords Outside Map Bounds!\n" );

	return GetWorldCoordsForTileCoordsMins( tileCoords ) + WorldCoords( 1.f, 1.f );
}


//--------------------------------------------------------------------------------------------------------------
TileCoords Map::GeTileCoordsForIndex( TileIndex tileIndex ) const
{
	ASSERT_OR_DIE( tileIndex >= 0 && tileIndex < (int)m_tiles.size(),
		"TileIndex Outside Map Bounds!\n" );

	int tileY = tileIndex / m_size.x;
	int tileX = tileIndex % m_size.x;

	return TileCoords( tileX, tileY );
}


//--------------------------------------------------------------------------------------------------------------
TileIndex Map::GetTileIndexForTileCoords( const TileCoords& tileCoords ) const
{
	ASSERT_OR_DIE( tileCoords.x >= 0 && tileCoords.x < m_size.x
				&& tileCoords.y >= 0 && tileCoords.y < m_size.y,
		"TileCoords Outside Map Bounds!\n" );

	return ( tileCoords.y * m_size.x ) + ( tileCoords.x );
}