#pragma once


#include "Game/GameCommon.hpp"
#include "Engine/Renderer/Rgba.hpp"


//-----------------------------------------------------------------------------
class TileDefinition;


//-----------------------------------------------------------------------------
class TileDefinition
{
public:
	TileDefinition( TileType tileType );

	Rgba GetColor() const { return m_color; }
	int GetSpriteTexIndex() const { return m_spriteTexIndex; }
	bool isSolid() const { return m_isSolid;  }


private:
	Rgba m_color;
	int m_spriteTexIndex; //I feel this is way less conflated than tex coords being normalized or not.
	bool m_isSolid;
};