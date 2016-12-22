#pragma once


#include "Game/Enemy.hpp"
#include "Game/GameCommon.hpp"


class EnemyTank : public Enemy
{
public:
	EnemyTank();

	virtual void ReactToPlayer( const Entity& player ) override;
	virtual void ReactToNothing() override;
	virtual bool IsWaitingForFiringCooldown() override;
	virtual void Die() override;

private:
	static const float TURN_SPEED_KNOB;
	static const float DRIVE_SPEED_KNOB;
	static const float ENEMY_TANK_FIRE_COOLDOWN_SECONDS;
	static const int INITIAL_TANK_HEALTH;

	static SoundID m_tankDeathSoundID;
};