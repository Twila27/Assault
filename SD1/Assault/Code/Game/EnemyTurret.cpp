#include "Game/EnemyTurret.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Audio/TheAudio.hpp"


//--------------------------------------------------------------------------------------------------------------
STATIC const float EnemyTurret::TURN_SPEED_KNOB							= 1.27f;
STATIC const float EnemyTurret::ENEMY_TURRET_FIRE_COOLDOWN_SECONDS		= 2.f;
STATIC const int EnemyTurret::INITIAL_TURRET_HEALTH						= 1;
STATIC SoundID EnemyTurret::m_turretDeathSoundID = 0;


//--------------------------------------------------------------------------------------------------------------
EnemyTurret::EnemyTurret()
	: Enemy("Data/Images/Turret.png")
{
	m_health = EnemyTurret::INITIAL_TURRET_HEALTH;
	EnemyTurret::m_turretDeathSoundID = g_theAudio->CreateOrGetSound( "Data/Audio/Explo_EnergyFireball01.wav" );
}


//--------------------------------------------------------------------------------------------------------------
void EnemyTurret::Die()
{
	SetIsAlive( false );
	g_theAudio->PlaySound( m_turretDeathSoundID, VOLUME_ADJUST );
}


//--------------------------------------------------------------------------------------------------------------
void EnemyTurret::ReactToPlayer( const Entity& player )
{
	//Turn slowly to face player.
	m_goalOrientation = Atan2Degrees( player.GetPosition() - m_position );
	m_angularVelocity = EnemyTurret::TURN_SPEED_KNOB * CalcShortestAngularDisplacement( m_orientation, m_goalOrientation );

	//Shoot at player if within +-5 degrees.
	m_isShooting = ( CalcShortestAngularDisplacement( m_orientation, m_goalOrientation ) <= 5.f );
}


//--------------------------------------------------------------------------------------------------------------
void EnemyTurret::ReactToNothing( ) //Pure virtual in base class, here for potential elaboration in future.
{
}


//--------------------------------------------------------------------------------------------------------------
bool EnemyTurret::IsWaitingForFiringCooldown()
{
	bool isWaiting = ( m_cooldownFiringTimer <= EnemyTurret::ENEMY_TURRET_FIRE_COOLDOWN_SECONDS );

	if ( !isWaiting ) m_cooldownFiringTimer = 0.f;

	return isWaiting;
}