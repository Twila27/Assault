#include "Game/Enemy.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/TheRenderer.hpp"


STATIC const float Enemy::ENEMY_GOAL_UPDATE_COOLDOWN_SECONDS		= 3.f;
STATIC const float Enemy::ENEMY_FIRING_COOLDOWN_SECONDS				= 1.f;

Enemy::Enemy( const std::string& imageFilePath )
	: Entity( imageFilePath )
	, m_goalOrientation( 0.f )
	, m_isShooting( false )
	, m_cooldownMotionTimer( 0.f )
	, m_cooldownFiringTimer( 0.f )
{
	m_orientation = GetRandomFloatInRange( 0.f, 359.f );
}

void Enemy::FindNewGoalOrientation()
{
	m_goalOrientation = GetRandomFloatInRange( 0.f, 359.f );
}


void Enemy::Render()
{
	if ( !m_isAlive ) return;

	g_theRenderer->PushView();

	//Change of Basis (detour) Theorem's triple product, from camera (world-to-cam happens before this is called!) to model space.
	g_theRenderer->TranslateView( m_position * 1.f );
	g_theRenderer->RotateViewByDegrees( m_orientation );
	g_theRenderer->TranslateView( m_position * -1.f );

	g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, m_renderBounds, *m_diffuseTexture );

	g_theRenderer->PopView();
}


void Enemy::Update( float deltaSeconds )
{
	m_cooldownMotionTimer += deltaSeconds;
	m_cooldownFiringTimer += deltaSeconds;

	Entity::Update( deltaSeconds );

	//Zero these, they are set when behaviors are called only.
	m_velocity = Vector2( 0.f, 0.f );
	m_angularVelocity = 0.f;
}


bool Enemy::IsWaitingForMotionCooldown()
{
	bool isWaiting = ( m_cooldownMotionTimer <= Enemy::ENEMY_GOAL_UPDATE_COOLDOWN_SECONDS );

	if ( !isWaiting ) m_cooldownMotionTimer = 0.f; 

	return isWaiting;
}


bool Enemy::IsWaitingForFiringCooldown()
{
	bool isWaiting = ( m_cooldownFiringTimer <= Enemy::ENEMY_FIRING_COOLDOWN_SECONDS );

	if ( !isWaiting ) m_cooldownFiringTimer = 0.f;

	return isWaiting;
}