#pragma once


#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"


class PlayerTank : public Entity
{

public:
	PlayerTank();

	void Update( float deltaSeconds ) override;
	virtual void Die() override;

	static const int INITIAL_TANK_HEALTH; //Needed by Map for tank respawn.


private:
	void MoveForward( float deltaSeconds );
	void MoveBackward( float deltaSeconds );
	void TurnLeft( float deltaSeconds );
	void TurnRight( float deltaSeconds );

	static const float TANK_SPEED_MULTIPLIER;
	static const float TANK_ROTATION_KNOB;

	static SoundID m_playerTankDeathSoundID;
};