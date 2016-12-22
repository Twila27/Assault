#include "Game/Entity.hpp"


#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Error/ErrorWarningAssert.hpp"
#include "Game/TheApp.hpp"
#include "Game/GameCommon.hpp"


//--------------------------------------------------------------------------------------------------------------
STATIC const float Entity::DEFAULT_ENTITY_RADIUS_SIZE						= .5f;


//--------------------------------------------------------------------------------------------------------------
Entity::Entity( const std::string& imageFilePath )
	: m_position( Vector2( 0.f, 0.f ) )
	, m_velocity( Vector2( 0.f, 0.f ) )
	, m_orientation( 0.f ) //Treated as degrees by other functions.
	, m_angularVelocity( 0.f )
	, m_acceleration( Vector2( 0.f, 0.f ) )
	, m_cosmeticRadius( DEFAULT_ENTITY_RADIUS_SIZE )
	, m_physicsRadius( DEFAULT_ENTITY_RADIUS_SIZE )
	, m_isAlive( true )
	, m_ageAliveInSeconds( 0.f )
	, m_ageDeadInSeconds( 0.f )
	, m_health( 0 )
	, m_renderBounds( 
		Vector2( m_position.x - m_cosmeticRadius, m_position.y - m_cosmeticRadius ),
		Vector2( m_position.x + m_cosmeticRadius, m_position.y + m_cosmeticRadius )
	)
{
	m_diffuseTexture = Texture::CreateOrGetTexture( imageFilePath );
}

Entity::~Entity()
{
	//Note texture deletion handled later on, as per Squirrel saying so in class.
}

//--------------------------------------------------------------------------------------------------------------
void Entity::Update( float deltaSeconds )
{
	if ( m_isAlive ) m_ageAliveInSeconds += deltaSeconds;
	else m_ageDeadInSeconds += deltaSeconds;

	m_position += m_velocity * deltaSeconds;
	m_renderBounds.mins = Vector2( m_position.x - m_cosmeticRadius, m_position.y - m_cosmeticRadius );
	m_renderBounds.maxs = Vector2( m_position.x + m_cosmeticRadius, m_position.y + m_cosmeticRadius );
	m_orientation += m_angularVelocity * deltaSeconds;
}


//--------------------------------------------------------------------------------------------------------------
void Entity::Render()
{
	if ( !m_isAlive ) return;

	g_theRenderer->PushView( );

	//Change of Basis (detour) Theorem's triple product, from camera (world-to-cam happens before this is called!) to model space.
	g_theRenderer->TranslateView( m_position * 1.f );
	g_theRenderer->RotateViewByDegrees( m_orientation );
	g_theRenderer->TranslateView( m_position * -1.f );

	g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, m_renderBounds, *m_diffuseTexture );

	g_theRenderer->PopView( );
}


//--------------------------------------------------------------------------------------------------------------
bool Entity::DoesOverlap( const Entity& ent ) const
{
	return DoDiscsOverlap( m_position, m_physicsRadius, ent.m_position, ent.m_physicsRadius );
}


//--------------------------------------------------------------------------------------------------------------
void Entity::DecreaseHealth( int numToLowerBy /*=1*/)
{
	ASSERT_OR_DIE( numToLowerBy > 0, "DecreaseHealth Does Not Heal!" );


	m_health -= numToLowerBy;
	if ( m_health <= 0 ) Die();
}

//--------------------------------------------------------------------------------------------------------------
Vector2 Entity::GetLocalTipPosition() const
{
	return GetDirection() * m_cosmeticRadius;
}


//--------------------------------------------------------------------------------------------------------------
Vector2 Entity::GetDirection() const
{
	Vector2 directionVector = Vector2( CosDegrees( m_orientation ), SinDegrees( m_orientation ) );

	directionVector.Normalize( );

	return directionVector;
}


//--------------------------------------------------------------------------------------------------------------
void Entity::SetPosition( float newX, float newY )
{
	m_position.x = newX;
	m_position.y = newY;
}


//--------------------------------------------------------------------------------------------------------------
void Entity::SetVelocity( float newX, float newY )
{
	m_velocity.x = newX;
	m_velocity.y = newY;
}


//--------------------------------------------------------------------------------------------------------------
void Entity::SetOrientation( float newOrientation )
{
	m_orientation = newOrientation;
}


//--------------------------------------------------------------------------------------------------------------
void Entity::SetAngularVelocity( float newAngularVelocity )
{
	m_angularVelocity = newAngularVelocity;
}


//--------------------------------------------------------------------------------------------------------------
void Entity::SetIsAlive( bool newStatus )
{
	m_isAlive = newStatus;
	if ( newStatus == true ) m_ageDeadInSeconds = 0.f;
}