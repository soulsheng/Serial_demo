
#pragma once

#include <string>

enum FRAME_TYPE
{
	FRAME_POSITION,
	FRAME_ANGLE,
	FRAME_NULL
};

class	StringParser
{
protected:
	float	m_fAngle[3];
	float	m_fPosition[3];
	char	m_cDirection[2];

	std::string		m_strLeft;	// string left from last frame
	bool	m_bFrameValid[2];	// position or angle frame is valid or not
	FRAME_TYPE	m_bFrameTypeCurrent;

	void parseAngleValueFromString( std::string str );
	void parsePositionValueFromString( std::string& str );

public:
	StringParser();
	void parseValueFromString( std::string& str );
};