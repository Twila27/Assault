#pragma once


#include "Engine/Audio/TheAudio.hpp"
#include "Game/GameCommon.hpp"


//-----------------------------------------------------------------------------
class TheGame;
class Map;
class PlayerTank;


//-----------------------------------------------------------------------------
extern TheGame* g_theGame;


//-----------------------------------------------------------------------------
class TheGame
{
public:
	bool m_isQuitting;

	TheGame();
	~TheGame();

	void Update( float deltaSeconds );
	void Render();


private:
	enum TheGameState { ATTRACT, PLAYING, PAUSED, GAMEOVER, NUM_STATES };
	TheGameState m_currentGameState;

	Map* m_currentMap;
	PlayerTank* m_playerTank;

	float m_secondsPaused;
	float m_secondsLeftToContinue;

	SoundID m_attractModeMusicID;
	SoundID m_attractModeExitSoundID;
	SoundID m_pauseModeMusicID;
	SoundID m_pauseModeExitID;
	AudioChannelHandle m_attractModeMusicChannelHandle;
	AudioChannelHandle m_pauseModeMusicChannelHandle;

	static const int MAP_SIZE_SCALING_KNOB;
	static const TileCoords MAP_SIZE_IN_TILES;
	static const TileCoords MAP_SIZE_SCALED;
	static const Vector2 SCREEN_DIMENSIONS_IN_TILES; //Floats for SetOrtho.
	static const Vector2 SCREEN_DIMENSIONS_IN_PIXELS;
	static const float SECONDS_BEFORE_NOT_CONTINUING;
	static const float SECONDS_BETWEEN_PLAYER_DEATH_AND_PAUSE;
};
