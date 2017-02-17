
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

class	StringParserDIY
{
protected:
	float	m_fAngle[3];
	double	m_fPosition[3];
	char	m_cDirection[2];

	std::string		strCurrentFrame;	// string current frame
	bool	m_bFrameValid[2];	// position or angle frame is valid or not
	FRAME_TYPE	m_bFrameTypeCurrent;
	StringVector	m_strVecPosition;		// position strings separated by ,
	StringVector	m_strVecAngle;			// angle strings separated by ,

	int		m_nFrameCount;

	void parseAngleValueFromString( std::string str );
	void parsePositionValueFromString( std::string& str );
	void parseValueFromString( );
	void setDefault();

public:
	StringParserDIY();
	int		parseValueFromString( std::string& str );
	double*	getPosition(char *pDir);
	float*	getAngle();
	StringVector*	getStringVectorPosition();
	StringVector*	getStringVectorAngle();
	FRAME_TYPE		getFrameType();
	bool			isFrameValid();
	std::string		formatCurrentFrameToString();
	bool			isFrameComplete( std::string& str );
};