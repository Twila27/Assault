#pragma once


#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteAnimation.hpp"
#include "Engine/Math/Vector2.hpp"


class Explosion
{
public:
	Explosion( const Vector2& position, float radius, float durationSeconds );
	
	void Update( float deltaSeconds ) { m_animation.Update( deltaSeconds ); }
	void Render();
	bool IsFinished() const { return m_animation.IsFinished(); }

private:
	SpriteAnimation m_animation;
	SpriteSheet m_atlas;
	Vector2 m_position;
	AABB2 m_renderBounds;
};