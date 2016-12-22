#pragma once


#include "Engine/Input/XboxController.hpp"


//-----------------------------------------------------------------------------
class TheApp;


//-----------------------------------------------------------------------------
extern TheApp* g_theApp;
static const int NUM_KEYS = 250; //Can't move into class due to use as array sizes.
static const int NUM_CONTROLLERS = 4;


//-----------------------------------------------------------------------------
struct KeyButtonState
{
	bool m_isKeyDown;
	bool m_didKeyJustChange;
};


//-----------------------------------------------------------------------------
class TheApp
{
public:
	enum Controllers { CONTROLLER_ONE, CONTROLLER_TWO, CONTROLLER_THREE, CONTROLLER_FOUR };

	double m_screenWidth;
	double m_screenHeight;
	Vector2 m_screenCenter;
	XboxController* m_controllers[ NUM_CONTROLLERS ];

	TheApp( );
	TheApp( const double screenWidth, const double screenHeight );

	void SetKeyDownStatus( unsigned char asKey, bool isNowDown );
	bool isKeyDown( unsigned char keyID );
	bool WasKeyJustPressed( unsigned char keyID );
	void Update( );


private:
	KeyButtonState m_keys[ NUM_KEYS ];
};
