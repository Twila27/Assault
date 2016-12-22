#include "Game/TileDefinition.hpp"


#include "Engine/Renderer/Rgba.hpp"


//--------------------------------------------------------------------------------------------------------------
TileDefinition::TileDefinition( TileType tileType )
{
	switch ( tileType )
	{
		case TileType::GRASS: 

			m_color = Rgba( 0.f, .5f, 0.f );
			m_isSolid = false;
			m_spriteTexIndex = 0; //Note hardcoding.

			break;

		case TileType::STONE: 

			m_color = Rgba( .5f, .25f, .25f );
			m_isSolid = true;
			m_spriteTexIndex = 1; //Note hardcoding.

			break;
	}
}