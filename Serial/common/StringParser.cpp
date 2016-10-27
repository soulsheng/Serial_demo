
#include "StdAfx.h"
#include "StringParser.h"

#include "vgkStringUtility.h"
using namespace vgKernel;


StringParser::StringParser()
{
	m_bFrameValid[0] = false;
	m_bFrameValid[1] = false;

	m_bFrameTypeCurrent = FRAME_NULL;
}


void StringParser::parseAngleValueFromString( std::string str )
{
	int nIndexFindBegin = str.find("AVR,");

	if( nIndexFindBegin == -1 )
		return;

	m_bFrameTypeCurrent = FRAME_ANGLE;

	str = str.substr( nIndexFindBegin+4 );	// remove substring "...AVR,"

	int nIndexFindEnd = str.find_last_of("\r\n");

	std::string strSub = str.substr( 0, nIndexFindEnd );

	StringVector strVec = StringUtility::tokenize( strSub, "," );
	if( strVec.size() >= 8 )
	{
		m_fAngle[0] = atof( strVec[1].c_str() );
		m_fAngle[1] = atof( strVec[3].c_str() );
		m_fAngle[2] = atof( strVec[7].c_str() );
	}


	if ( str.size() > strSub.size()+2 )	// strSub have no "\r\n"
		m_strLeft = str.substr( nIndexFindEnd + 2 );
	else
		m_strLeft.clear();
}

void StringParser::parsePositionValueFromString( std::string& str )
{
	int nIndexFindBegin = str.find("GPGGA,");

	if( nIndexFindBegin == -1 )
	{
		m_strLeft = str;
		return;
	}

	m_bFrameTypeCurrent = FRAME_POSITION;

	str = str.substr( nIndexFindBegin+6 );	// remove substring "...GPGGA,"

	int nIndexFindEnd = str.find_last_of("\r\n");

	std::string strSub = str.substr( 0, nIndexFindEnd );

	StringVector strVec = StringUtility::tokenize( strSub, "," );
	if( strVec.size() >= 9 )
	{
		m_fPosition[0] = atof( strVec[1].c_str() );
		m_fPosition[1] = atof( strVec[3].c_str() );
		m_fPosition[2] = atof( strVec[8].c_str() );

		m_cDirection[0] = strVec[2].at(0);
		m_cDirection[1] = strVec[4].at(0);
	}

	if ( str.size() > strSub.size()+2 )	// strSub have no "\r\n"
		m_strLeft = str.substr( nIndexFindEnd + 2 );
	else
		m_strLeft.clear();
}

void StringParser::parseValueFromString( std::string& str )
{
	int nIndexFind = str.find_last_of("\r\n");

	if( nIndexFind == -1)
		return;

	str = m_strLeft + str;

	parsePositionValueFromString( str );
	parseAngleValueFromString( m_strLeft );

	char buf[256];

	if ( FRAME_POSITION == m_bFrameTypeCurrent )
		std::sprintf(buf, "position(%f %c,%f %c,%f) \n", 
		m_fPosition[0], m_cDirection[0], m_fPosition[1], m_cDirection[1], m_fPosition[2] );
	else if ( FRAME_ANGLE == m_bFrameTypeCurrent )
		std::sprintf(buf, "angle(%f,%f,%f) \n",
		m_fAngle[0], m_fAngle[1], m_fAngle[2] );

	if ( FRAME_NULL != m_bFrameTypeCurrent )
		str = buf;
}