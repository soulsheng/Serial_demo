
#include "StdAfx.h"
#include "StringParser.h"



StringParser::StringParser()
{
	m_bFrameValid[0] = false;
	m_bFrameValid[1] = false;

	m_bFrameTypeCurrent = FRAME_NULL;

	m_nFrameCount = 1;
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
	
	m_strVecAngle.clear();

	m_strVecAngle = StringUtility::tokenize( strSub, "," );
	if( m_strVecAngle.size() >= 8 )
	{
		m_fAngle[0] = atof( m_strVecAngle[1].c_str() );
		m_fAngle[1] = atof( m_strVecAngle[3].c_str() );
		m_fAngle[2] = atof( m_strVecAngle[7].c_str() );
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

	m_strVecPosition.clear();

	m_strVecPosition = StringUtility::tokenize( strSub, "," );
	if( m_strVecPosition.size() >= 9 )
	{
		m_fPosition[0] = atof( m_strVecPosition[1].c_str() );
		m_fPosition[1] = atof( m_strVecPosition[3].c_str() );
		m_fPosition[2] = atof( m_strVecPosition[8].c_str() );

		m_cDirection[0] = m_strVecPosition[2].at(0);
		m_cDirection[1] = m_strVecPosition[4].at(0);
	}

	if ( str.size() > strSub.size()+2 )	// strSub have no "\r\n"
		m_strLeft = str.substr( nIndexFindEnd + 2 );
	else
		m_strLeft.clear();
}

void StringParser::parseValueFromString( std::string& str )
{
	m_bFrameTypeCurrent = FRAME_NULL;

	int nIndexFind = str.find_last_of("\r\n");

	if( nIndexFind == -1)
		return;

	str = m_strLeft + str;

	parsePositionValueFromString( str );

	nIndexFind = m_strLeft.find_last_of("\r\n");

	if( nIndexFind != -1)
		parseAngleValueFromString( m_strLeft );

	char buf[256];

	if ( FRAME_POSITION == m_bFrameTypeCurrent )
		std::sprintf(buf, "position(%.8lf %c,%.8lf %c,%.3f) \n", 
		m_fPosition[0], m_cDirection[0], m_fPosition[1], m_cDirection[1], m_fPosition[2] );
	else if ( FRAME_ANGLE == m_bFrameTypeCurrent )
		std::sprintf(buf, "angle(%.4f,%.4f,%.3f) \n",
		m_fAngle[0], m_fAngle[1], m_fAngle[2] );

	if( m_nFrameCount % 20 == 0)
		std::sprintf(buf, "%d frames:\n", m_nFrameCount );
	m_nFrameCount ++;

	if ( FRAME_NULL != m_bFrameTypeCurrent )
		str = buf;
}

double* StringParser::getPosition( char *pDir )
{
	strcpy_s( pDir, 2, m_cDirection );
	return m_fPosition;
}

float* StringParser::getAngle()
{
	return m_fAngle;
}

vgKernel::StringVector* StringParser::getStringVectorPosition()
{
	return &m_strVecPosition;
}

vgKernel::StringVector* StringParser::getStringVectorAngle()
{
	return &m_strVecAngle;
}

FRAME_TYPE StringParser::getFrameType()
{
	return m_bFrameTypeCurrent;
}

bool StringParser::isFrameValid()
{
	return m_bFrameTypeCurrent != FRAME_NULL;
}
