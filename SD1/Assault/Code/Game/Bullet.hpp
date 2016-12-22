#pragma once


#include "Game/Entity.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteAnimation.hpp"


//-----------------------------------------------------------------------------
class Bullet : public Entity
{
public:
	enum BulletType { PLAYER, ENEMY };

	Bullet( const Vector2& position, const float orientation, const BulletType type );
	~Bullet() override {}

	float GetTimeToLive() const { return m_timeToLive; }
	BulletType GetBulletType() const { return m_type; }
	void Update( float deltaSeconds ) override;
	void Render() override;

private:
	BulletType m_type;
	float m_timeToLive;
	SpriteSheet m_atlas;
	SpriteAnimation m_animation;

	static const float SECONDS_BEFORE_DISAPPEARING;
	static const float BULLET_SIZE;
	static const float BULLET_COSMETIC_COLLIDER_RATIO;
	static const float BULLET_SPEED;
	static const int INITIAL_BULLET_HEALTH;
};