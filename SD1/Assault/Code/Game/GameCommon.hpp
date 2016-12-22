#pragma once


#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"


#define STATIC //do-nothing macro for readability to "add" 'static' to function definitions

#define VK_SPACE 0x20
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_ESCAPE 0x1B

#define GL_ONE 1
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303

typedef unsigned int SoundID;

typedef Vector2 WorldCoords;
typedef IntVector2 TileCoords;
typedef int TileIndex;


enum TileType { GRASS, STONE, NUM_TILE_TYPES };
enum ExplosionType { SMALL, MEDIUM, LARGE, NUM_EXPLOSION_TYPES };


static const float VOLUME_ADJUST = .15f;

static const float NOT_REALLY_HALF_OF_FULL_HUE_BUT_WHO_CARES = 128.f / 255.f;