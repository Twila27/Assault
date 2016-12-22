#include "Game/PlayerTank.hpp"


#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Audio/TheAudio.hpp"

#include "Game/TheApp.hpp"


//--------------------------------------------------------------------------------------------------------------
STATIC const float PlayerTank::TANK_SPEED_MULTIPLIER			= 1.5f;
STATIC const float PlayerTank::TANK_ROTATION_KNOB				= 72.f;
STATIC const int PlayerTank::INITIAL_TANK_HEALTH = 10;
STATIC SoundID PlayerTank::m_playerTankDeathSoundID = 0;


//--------------------------------------------------------------------------------------------------------------
PlayerTank::PlayerTank()
	: Entity( "Data/Images/Tank.png" )
{
	m_health = INITIAL_TANK_HEALTH;
	PlayerTank::m_playerTankDeathSoundID = g_theAudio->CreateOrGetSound( "Data/Audio/Explo_Large_Atomic.wav" );
}


//--------------------------------------------------------------------------------------------------------------
void PlayerTank::MoveBackward( float deltaSeconds )
{
	m_position += GetDirection() * TANK_SPEED_MULTIPLIER * -1.f * deltaSeconds;
}


//--------------------------------------------------------------------------------------------------------------
void PlayerTank::MoveForward( float deltaSeconds )
{
	m_position += GetDirection() * TANK_SPEED_MULTIPLIER * 1.f * deltaSeconds;
}


//--------------------------------------------------------------------------------------------------------------
void PlayerTank::TurnLeft( float deltaSeconds )
{
	m_orientation += TANK_ROTATION_KNOB * deltaSeconds;
}


//--------------------------------------------------------------------------------------------------------------
void PlayerTank::TurnRight( float deltaSeconds )
{
	m_orientation -= TANK_ROTATION_KNOB * deltaSeconds;
}


//--------------------------------------------------------------------------------------------------------------
void PlayerTank::Update( float deltaSeconds )
{
	if ( g_theApp->isKeyDown( 'W' ) || g_theApp->isKeyDown( VK_UP ) ) MoveForward( deltaSeconds );
	else if ( g_theApp->isKeyDown( 'S' ) || g_theApp->isKeyDown( VK_DOWN ) ) MoveBackward( deltaSeconds );

	if ( g_theApp->isKeyDown( 'A' ) || g_theApp->isKeyDown( VK_LEFT ) ) TurnLeft( deltaSeconds );
	else if ( g_theApp->isKeyDown( 'D' ) || g_theApp->isKeyDown( VK_RIGHT ) ) TurnRight( deltaSeconds );
	else m_angularVelocity = 0;


	if ( g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ] != nullptr )
	{
		Vector2 leftStickPos = g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ]->GetLeftStickPosition();
		Vector2 rightStickPos = g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ]->GetRightStickPosition();

		if ( leftStickPos.y > 0.f && rightStickPos.y > 0.f ) MoveForward( deltaSeconds );
		else if ( leftStickPos.y < 0.f && rightStickPos.y < 0.f ) MoveBackward( deltaSeconds );

		if ( leftStickPos.y > 0.f && rightStickPos.y < 0.f ) TurnRight( deltaSeconds );
		else if ( leftStickPos.y < 0.f && rightStickPos.y > 0.f ) TurnLeft( deltaSeconds );
	}


	Entity::Update( deltaSeconds );
}


//--------------------------------------------------------------------------------------------------------------
void PlayerTank::Die()
{
	SetIsAlive( false );
	g_theAudio->PlaySound( m_playerTankDeathSoundID, VOLUME_ADJUST );
}
