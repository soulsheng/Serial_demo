
#pragma once

#include <string>

#include "vgkStringUtility.h"
using namespace vgKernel;

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
	StringVector	m_strVecPosition;		// position strings separated by ,
	StringVector	m_strVecAngle;			// angle strings separated by ,

	void parseAngleValueFromString( std::string str );
	void parsePositionValueFromString( std::string& str );

public:
	StringParser();
	void parseValueFromString( std::string& str );
	float*	getPosition(char *pDir);
	float*	getAngle();
	StringVector*	getStringVectorPosition();
	StringVector*	getStringVectorAngle();
	FRAME_TYPE		getFrameType();
	bool			isFrameValid();
};