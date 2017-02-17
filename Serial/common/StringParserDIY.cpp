
#include "StdAfx.h"
#include "StringParserDIY.h"

#define		FRAME_SIZE		151

StringParserDIY::StringParserDIY()
{
	m_bFrameValid[0] = false;
	m_bFrameValid[1] = false;

	m_bFrameTypeCurrent = FRAME_NULL;

	m_nFrameCount = 0;

	setDefault();
}


void StringParserDIY::parseAngleValueFromString( std::string str )
{
	int nIndexFindBegin = str.find("AVR,");

	if( nIndexFindBegin == -1 )
		return;

	m_bFrameTypeCurrent = FRAME_ANGLE;

	str = str.substr( nIndexFindBegin+4 );	// remove substring "...AVR,"

	int nIndexFindEnd = str.find("\r\n");

	std::string strSub = str.substr( 0, nIndexFindEnd );
	
	m_strVecAngle.clear();

	m_strVecAngle = StringUtility::tokenize( strSub, "," );
	if( m_strVecAngle.size() >= 8 )
	{
		m_fAngle[0] = atof( m_strVecAngle[1].c_str() );
		m_fAngle[1] = atof( m_strVecAngle[3].c_str() );
		m_fAngle[2] = atof( m_strVecAngle[7].c_str() );
	}

}

void StringParserDIY::parsePositionValueFromString( std::string& str )
{
	int nIndexFindBegin = str.find("GPGGA,");

	if( nIndexFindBegin == -1 )
	{
		return;
	}

	m_bFrameTypeCurrent = FRAME_POSITION;

	str = str.substr( nIndexFindBegin+6 );	// remove substring "...GPGGA,"

	int nIndexFindEnd = str.find("\r\n", nIndexFindBegin);

	std::string strSub = str.substr( 0, nIndexFindEnd );

	// remove GPGGA sub-frame 
	str = str.substr( nIndexFindEnd+2 );

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


}


double* StringParserDIY::getPosition( char *pDir )
{
	strcpy_s( pDir, 2, m_cDirection );
	return m_fPosition;
}

float* StringParserDIY::getAngle()
{
	return m_fAngle;
}

vgKernel::StringVector* StringParserDIY::getStringVectorPosition()
{
	return &m_strVecPosition;
}

vgKernel::StringVector* StringParserDIY::getStringVectorAngle()
{
	return &m_strVecAngle;
}

FRAME_TYPE StringParserDIY::getFrameType()
{
	return m_bFrameTypeCurrent;
}

bool StringParserDIY::isFrameValid()
{
	return m_bFrameTypeCurrent != FRAME_NULL;
}

std::string StringParserDIY::formatCurrentFrameToString()
{
	// format frame value back to string
	std::string str;

	char buf[256];

	std::sprintf(buf, "position(%.8lf %c,%.8lf %c,%.3f) \n", 
		m_fPosition[0], m_cDirection[0], m_fPosition[1], m_cDirection[1], m_fPosition[2] );

	str = buf;

	std::sprintf(buf, "angle(%.4f,%.4f,%.3f) \n",
		m_fAngle[0], m_fAngle[1], m_fAngle[2] );

	str += buf;

	return str;
}

void StringParserDIY::setDefault()
{
	m_fAngle[0] = m_fAngle[1] = m_fAngle[2] = 0.0f;
	m_fPosition[0] = m_fPosition[1] = m_fPosition[2] = 0.0f;
	m_cDirection[0] = m_cDirection[1] = 0;
}

void StringParserDIY::parseValueFromString( )
{

	parsePositionValueFromString( strCurrentFrame );

	parseAngleValueFromString( strCurrentFrame );

}

int StringParserDIY::parseValueFromString( std::string& str )
{
	int nBegin = str.find("GPGGA");

	if ( nBegin != -1 )
		printf("GPS frame begin at = %d \n", nBegin );


	parseValueFromString(  );

	str = str.substr( nBegin+151, std::string::npos );

	m_nFrameCount ++;

	return nBegin;
}

bool StringParserDIY::isFrameComplete( std::string& str )
{
	int nBegin = str.find("GPGGA");

	if ( nBegin == -1 )
		return false;

	strCurrentFrame = str.substr( nBegin, FRAME_SIZE );

	if( strCurrentFrame.length() < FRAME_SIZE )
		return false;
	
	return true;
}
