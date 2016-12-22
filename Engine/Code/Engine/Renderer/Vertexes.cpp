#include "Engine/Renderer/Vertexes.hpp"


Vertex3D_PCT::Vertex3D_PCT( const Vector3& position, const Vector2& texCoords /*= Vector2::ZERO*/, const Rgba& color /*= Rgba::WHITE */ )
	: m_position( position )
	, m_color( color )
	, m_texCoords( texCoords )
{
}


Vertex3D_PCT::Vertex3D_PCT( const Vector3& position, const Rgba& color /*= Rgba::WHITE */, const Vector2& texCoords /*= Vector2::ZERO*/ )
	: m_position( position )
	, m_color( color )
	, m_texCoords( texCoords )
{
}

Vertex3D_PCT::Vertex3D_PCT()
	: m_position()
	, m_color()
	, m_texCoords()
{

}
