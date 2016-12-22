#pragma once


class EulerAngles //IMPORTANT: this enforces a z-up orientation in the engine. Can't implement EulerAngles without imposing a fixed orientation like this.
{
public:

	inline EulerAngles( float rollDegreesAboutX, float pitchDegreesAboutY, float yawDegreesAboutZ );

	static const EulerAngles ZERO;

	float m_rollDegreesAboutX;
	float m_pitchDegreesAboutY;
	float m_yawDegreesAboutZ;
};


//--------------------------------------------------------------------------------------------------------------
inline EulerAngles::EulerAngles( float rollDegreesAboutX, float pitchDegreesAboutY, float yawDegreesAboutZ )
	: m_rollDegreesAboutX( rollDegreesAboutX )
	, m_pitchDegreesAboutY( pitchDegreesAboutY )
	, m_yawDegreesAboutZ( yawDegreesAboutZ )
{
}
