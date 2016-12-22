#pragma once


#include "Game/Enemy.hpp"
#include "Game/GameCommon.hpp"


class EnemyTurret : public Enemy
{
public:
	EnemyTurret();

	virtual void ReactToPlayer( const Entity& player ) override;
	virtual void ReactToNothing() override;
	virtual bool IsWaitingForFiringCooldown() override;
	virtual void Die() override;

	static const float TURN_SPEED_KNOB;
	static const float ENEMY_TURRET_FIRE_COOLDOWN_SECONDS;
	static const int INITIAL_TURRET_HEALTH;

	static SoundID m_turretDeathSoundID;
};