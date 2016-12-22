#include "Game/Bullet.hpp"


#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Game/GameCommon.hpp"


//--------------------------------------------------------------------------------------------------------------
STATIC const float Bullet::SECONDS_BEFORE_DISAPPEARING		= 2.f;
STATIC const float Bullet::BULLET_COSMETIC_COLLIDER_RATIO	= .35f;
STATIC const float Bullet::BULLET_SIZE						= .2f;
STATIC const float Bullet::BULLET_SPEED						= 4.f;
STATIC const int Bullet::INITIAL_BULLET_HEALTH				= 1;


//--------------------------------------------------------------------------------------------------------------
Bullet::Bullet( const Vector2& position, const float orientation, const BulletType type /*= STANDARD */ )
: Entity( "Data/Images/Bullet.png" )
, m_atlas( "Data/Images/BulletAnim.png", 5, 1, 5, 5 )
, m_animation( m_atlas, 1.f, SPRITE_ANIM_MODE_PINGPONG, 0, 5 )
{
	m_health = Bullet::INITIAL_BULLET_HEALTH;
	m_timeToLive = Bullet::SECONDS_BEFORE_DISAPPEARING;
	m_cosmeticRadius = Bullet::BULLET_SIZE;
	m_physicsRadius = m_cosmeticRadius * Bullet::BULLET_COSMETIC_COLLIDER_RATIO;


	m_position = position;
	m_orientation = orientation;
	m_velocity.x = Bullet::BULLET_SPEED * CosDegrees( m_orientation );
	m_velocity.y = Bullet::BULLET_SPEED * SinDegrees( m_orientation );

	m_type = type;
}


//--------------------------------------------------------------------------------------------------------------
void Bullet::Update( float deltaSeconds )
{
	Entity::Update( deltaSeconds );
	m_animation.Update( deltaSeconds );
}

//--------------------------------------------------------------------------------------------------------------
void Bullet::Render()
{
	if ( !m_isAlive ) return;

	g_theRenderer->PushView();

	//Change of Basis (detour) Theorem's triple product, from camera (world-to-cam happens before this is called!) to model space.
	g_theRenderer->TranslateView( m_position * 1.f );
	g_theRenderer->RotateViewByDegrees( m_orientation );
	g_theRenderer->TranslateView( m_position * -1.f );

	//g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, m_renderBounds, *m_diffuseTexture ); //How Entity does it, unanimated.
	g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, m_renderBounds, *m_animation.GetTexture(), m_animation.GetCurrentTexCoords() );

	g_theRenderer->PopView();
}


