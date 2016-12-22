#include "Game/Explosion.hpp"


#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/TheRenderer.hpp"

#include "Game/GameCommon.hpp"


Explosion::Explosion( const Vector2& position, float cosmeticRadius, float durationSeconds )
	: m_position( position )
	, m_atlas( "Data/Images/Explosion_5x5.png", 5, 5, 64, 64 )
	, m_animation( m_atlas, durationSeconds, SPRITE_ANIM_MODE_PLAY_TO_END, 0, 25 )
	, m_renderBounds(
	Vector2( m_position.x - cosmeticRadius, m_position.y - cosmeticRadius ),
	Vector2( m_position.x + cosmeticRadius, m_position.y + cosmeticRadius )
	)
{
}


void Explosion::Render()
{
	g_theRenderer->SetBlendFunc( GL_SRC_ALPHA, GL_ONE );
	g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, m_renderBounds, *m_animation.GetTexture(), m_animation.GetCurrentTexCoords() );
	g_theRenderer->SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}