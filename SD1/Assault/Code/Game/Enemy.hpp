#pragma once


#include "Game/Entity.hpp"


class Enemy : public Entity
{
public: 
	bool IsShooting( ) const { return m_isShooting; }
	bool IsWaitingForMotionCooldown();
	virtual bool IsWaitingForFiringCooldown();
	virtual void ReactToPlayer( const Entity& player ) = 0;
	void FindNewGoalOrientation();
	virtual void ReactToNothing() = 0;
	virtual void Render() override;
	virtual void Update( float deltaSeconds ) override;


protected:
	Enemy( const std::string& imageFilePath );

	float m_cooldownMotionTimer;
	float m_cooldownFiringTimer;
	float m_goalOrientation;
	bool m_isShooting;

	static const float ENEMY_GOAL_UPDATE_COOLDOWN_SECONDS;
	static const float ENEMY_FIRING_COOLDOWN_SECONDS;
};