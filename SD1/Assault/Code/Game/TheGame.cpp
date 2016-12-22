#include "Game/TheGame.hpp"


#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"

#include "Game/TheApp.hpp" //Input.
#include "Game/GameCommon.hpp" //Typedefs, enums.
#include "Game/Map.hpp"
#include "Game/PlayerTank.hpp"


//--------------------------------------------------------------------------------------------------------------
TheGame* g_theGame = nullptr;
const Vector2 SCREEN_SIZE = Vector2( 1600.f, 900.f );
STATIC const Vector2 TheGame::SCREEN_DIMENSIONS_IN_PIXELS				= Vector2( 1600.f, 900.f );
STATIC const Vector2 TheGame::SCREEN_DIMENSIONS_IN_TILES				= Vector2( 16.f, 9.f );
STATIC const float TheGame::SECONDS_BEFORE_NOT_CONTINUING				= 10.f;
STATIC const float TheGame::SECONDS_BETWEEN_PLAYER_DEATH_AND_PAUSE		= 5.f;


//--------------------------------------------------------------------------------------------------------------
TheGame::TheGame()
	: m_currentGameState( TheGame::ATTRACT )
	, m_secondsPaused( 0.f )
	, m_secondsLeftToContinue( 0.f )
	, m_isQuitting( false )
{
	m_playerTank = new PlayerTank();
	m_currentMap = new Map( m_playerTank, "Data/Images/Maps/Level1.png", SCREEN_DIMENSIONS_IN_TILES );

	//m_attractModeExitSoundID = g_theAudio->CreateOrGetSound( "Data/Audio/Explo_Large_Atomic.wav" );
	m_attractModeMusicID = g_theAudio->CreateOrGetSound( "Data/Audio/Explo_GeneratorPowerupExplo.wav" );
	m_attractModeMusicChannelHandle = g_theAudio->PlaySound( m_attractModeMusicID, VOLUME_ADJUST );
	m_pauseModeExitID = g_theAudio->CreateOrGetSound( "Data/Audio/bell.wav" );
	m_pauseModeMusicID = g_theAudio->CreateOrGetSound( "Data/Audio/bell.wav" );
}


//--------------------------------------------------------------------------------------------------------------
TheGame::~TheGame()
{
	delete m_currentMap;
	delete m_playerTank;
}

//--------------------------------------------------------------------------------------------------------------
void TheGame::Update( float deltaSeconds )
{
	//Respond to pause/transition key.
	if ( ( g_theApp->isKeyDown( 'P' ) && g_theApp->WasKeyJustPressed( 'P' ) )
		|| //Keyboard above, controller below.
		( ( g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ] != nullptr ) &&
		g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ]->IsStartButtonDown() &&
		g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ]->WasStartButtonJustPressed() )
		)
	{
		if ( m_currentGameState == TheGame::ATTRACT || m_currentGameState == TheGame::GAMEOVER) //Exit Attract state.
		{
			m_playerTank->SetAgeDeadInSeconds( 0.f );
			m_playerTank->SetIsAlive( true );
			m_playerTank->SetHealth( PlayerTank::INITIAL_TANK_HEALTH );

			g_theAudio->StopChannel( m_attractModeMusicChannelHandle );

			m_currentGameState = TheGame::PLAYING;
		}
		else if ( m_playerTank->IsAlive() ) //Pause and unpause unless player is dead.
		{
			if ( m_currentGameState == TheGame::PAUSED ) //Exit Pause state.
			{
				m_secondsPaused = 0.f;
				g_theAudio->StopChannel( m_pauseModeMusicChannelHandle );
				m_pauseModeMusicChannelHandle = g_theAudio->PlaySound( m_pauseModeMusicID, VOLUME_ADJUST );
				m_currentGameState = TheGame::PLAYING;
			}
			else if ( m_currentGameState == TheGame::PLAYING ) //Exit Playing state.
			{
				m_pauseModeMusicChannelHandle = g_theAudio->PlaySound( m_pauseModeMusicID, VOLUME_ADJUST );
				m_currentGameState = TheGame::PAUSED;
			}
		}
	}
	//Respond to back key.
	if ( ( g_theApp->isKeyDown( VK_ESCAPE ) && g_theApp->WasKeyJustPressed( VK_ESCAPE ) )
		|| //Keyboard above, controller below.
		( ( g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ] != nullptr ) &&
		g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ]->IsBackButtonDown() &&
		g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ]->WasBackButtonJustPressed() )
		)
	{
		switch ( m_currentGameState )
		{
			case TheGame::ATTRACT: 
				m_isQuitting = true; 
				break;
			case TheGame::GAMEOVER:
			case TheGame::PAUSED: 
				m_currentGameState = TheGame::ATTRACT;
				break;
			case TheGame::PLAYING:
				m_pauseModeMusicChannelHandle = g_theAudio->PlaySound( m_pauseModeMusicID, VOLUME_ADJUST );
				m_currentGameState = TheGame::PAUSED;
				break;
		}
	}
	//End player-triggered transitions.
		

	//Start game-triggered transitions.
	if ( m_currentGameState == TheGame::PLAYING && m_playerTank->IsAlive( ) == false && m_playerTank->GetAgeDeadInSeconds( ) > TheGame::SECONDS_BETWEEN_PLAYER_DEATH_AND_PAUSE )
	{
		m_currentGameState = TheGame::GAMEOVER;
	}
	if ( m_currentGameState == TheGame::GAMEOVER && m_playerTank->IsAlive() == true )
	{
		m_playerTank->SetAgeDeadInSeconds( 0.f );
		m_playerTank->SetIsAlive( true );
		m_playerTank->SetHealth( PlayerTank::INITIAL_TANK_HEALTH );

		m_currentGameState = TheGame::PLAYING;
	}
	if ( m_currentGameState == TheGame::GAMEOVER && m_playerTank->IsAlive( ) == false && m_secondsLeftToContinue > SECONDS_BEFORE_NOT_CONTINUING )
	{
		m_secondsLeftToContinue = 0.f;
		
		m_attractModeMusicChannelHandle = g_theAudio->PlaySound( m_attractModeMusicID, VOLUME_ADJUST );

		m_currentGameState = TheGame::ATTRACT;
	}
	//End game-triggered transitions.


	//Start game state-specific updates.
	if ( m_currentGameState == TheGame::PAUSED ) 
	{
		m_secondsPaused += deltaSeconds;
		
		m_currentMap->Update( 0.f );
	}

	if ( m_currentGameState == TheGame::GAMEOVER )
	{
		m_secondsLeftToContinue += deltaSeconds;
		
		if ( m_playerTank->IsAlive() ) m_currentGameState = TheGame::PLAYING; //Player continued.
	}
	
	if ( m_currentGameState == TheGame::PLAYING ) m_currentMap->Update( deltaSeconds );
	if ( !m_playerTank->IsAlive() ) m_playerTank->SetAgeDeadInSeconds( m_playerTank->GetAgeDeadInSeconds( ) + deltaSeconds );
}


//--------------------------------------------------------------------------------------------------------------
void TheGame::Render( )
{
	if ( m_currentGameState != TheGame::ATTRACT ) m_currentMap->Render();


	g_theRenderer->SetOrtho( Vector2( 0.f, 0.f ), Vector2( 1600.f, 900.f ) );


	if ( m_currentGameState == TheGame::PAUSED )
	{
		g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, AABB2( Vector2( 0.f, 0.f ), SCREEN_SIZE ), Rgba( 0.f, 0.f, 0.f, .8f ) ); //Draw black screen dim.

		g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, AABB2( Vector2( 0.f, 0.f ), SCREEN_SIZE ), //Draw 'Paused' texture.
			*Texture::CreateOrGetTexture( "Data/Images/Paused.png" ), AABB2(0.f, 0.f, 1.f, 1.f), Rgba( 1.f, 1.f, 1.f, .4f ) );
	}
	else if ( m_currentGameState == TheGame::ATTRACT )
	{
		g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, AABB2( Vector2( 0.f, 0.f ), SCREEN_SIZE ), *Texture::CreateOrGetTexture( "Data/Images/AttractMode.png" ) );
	}
	else if ( m_currentGameState == TheGame::GAMEOVER )
	{
		g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, AABB2( Vector2( 0.f, 0.f ), SCREEN_SIZE ), *Texture::CreateOrGetTexture( "Data/Images/GameOver.png" ) );
	}
	else if ( m_playerTank->IsAlive() == false )
	{
		float amtAlpha = Interpolate( 0.f, 1.f, m_playerTank->GetAgeDeadInSeconds( ) / TheGame::SECONDS_BETWEEN_PLAYER_DEATH_AND_PAUSE );
		g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, AABB2( Vector2( 0.f, 0.f ), SCREEN_SIZE ), Rgba( 0.f, 0.f, 0.f, amtAlpha ) );
	}
}