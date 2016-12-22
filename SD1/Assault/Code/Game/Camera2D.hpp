#pragma once


#include "Game/GameCommon.hpp"


struct Camera2D
{
	WorldCoords m_worldPosition;
	Vector2 m_centeredOnScreenPosition;
	float m_orientationDegrees;
	float m_numTilesInViewVertically; //For scaling.
};