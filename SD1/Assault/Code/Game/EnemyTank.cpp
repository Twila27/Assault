#include "Game/EnemyTank.hpp"


#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Audio/TheAudio.hpp"


//--------------------------------------------------------------------------------------------------------------
STATIC const float EnemyTank::TURN_SPEED_KNOB						= .72f;
STATIC const float EnemyTank::DRIVE_SPEED_KNOB						= .72f;
STATIC const float EnemyTank::ENEMY_TANK_FIRE_COOLDOWN_SECONDS		= 1.f;
STATIC const int EnemyTank::INITIAL_TANK_HEALTH						= 3;
STATIC SoundID EnemyTank::m_tankDeathSoundID = 0;

//--------------------------------------------------------------------------------------------------------------
EnemyTank::EnemyTank()
	: Enemy( "Data/Images/Tank.png" )
{
	m_health = EnemyTank::INITIAL_TANK_HEALTH;
	EnemyTank::m_tankDeathSoundID = g_theAudio->CreateOrGetSound( "Data/Audio/Explo_EnergyFireball01.wav" );
}


//--------------------------------------------------------------------------------------------------------------
void EnemyTank::Die()
{
	SetIsAlive( false );
	g_theAudio->PlaySound( m_tankDeathSoundID, VOLUME_ADJUST );
}


//--------------------------------------------------------------------------------------------------------------
void EnemyTank::ReactToPlayer( const Entity& player )
{
	//Turn slowly to face player.
	m_goalOrientation = Atan2Degrees( player.GetPosition() - m_position );
	m_angularVelocity = EnemyTank::TURN_SPEED_KNOB * CalcShortestAngularDisplacement( m_orientation, m_goalOrientation );

	//Drive forward if player within +-45 degrees.
	float angleToPlayer = CalcShortestAngularDisplacement( m_orientation, m_goalOrientation ); //is this accurate for that name?
	if ( angleToPlayer <= 45.f ) //NEVER TRUE!
	{
		m_velocity = GetDirection( ) * EnemyTank::DRIVE_SPEED_KNOB;

		//Shoot at player if within +-5 degrees.
		m_isShooting = ( angleToPlayer <= 5.f );
	}
	else m_velocity = Vector2( 0.f, 0.f ); //Stop and turn until in angle range.
}


//--------------------------------------------------------------------------------------------------------------
void EnemyTank::ReactToNothing()
{
	m_angularVelocity = EnemyTank::TURN_SPEED_KNOB * CalcShortestAngularDisplacement( m_orientation, m_goalOrientation );

	m_velocity = GetDirection( ) * EnemyTank::DRIVE_SPEED_KNOB;
}


//--------------------------------------------------------------------------------------------------------------
bool EnemyTank::IsWaitingForFiringCooldown()
{
	bool isWaiting = ( m_cooldownFiringTimer <= EnemyTank::ENEMY_TANK_FIRE_COOLDOWN_SECONDS );

	if ( !isWaiting ) m_cooldownFiringTimer = 0.f;

	return isWaiting;
}