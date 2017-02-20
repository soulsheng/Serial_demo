
#include "StdAfx.h"
#include "StringParserPos.h"

#define		FRAME_SIZE		151
#define		FRAME_STRING_START	"$POS"
#define		FRAME_STRING_END	"\r\n"

StringParserPos::StringParserPos()
{
	m_bFrameValid[0] = false;
	m_bFrameValid[1] = false;

	m_bFrameTypeCurrent = FRAME_NULL;

	m_nFrameCount = 0;

	setDefault();
}



float* StringParserPos::getAngle()
{
	return m_fFrameItem;
}

vgKernel::StringVector* StringParserPos::getStringVector()
{
	return &m_sFrameItemValue;
}

FRAME_TYPE StringParserPos::getFrameType()
{
	return m_bFrameTypeCurrent;
}

bool StringParserPos::isFrameValid()
{
	return m_bFrameTypeCurrent != FRAME_NULL;
}

std::string StringParserPos::formatCurrentFrameToString()
{
	// format frame value back to string
	std::string str;

	char buf[256];

	std::sprintf(buf, "position(%.6f,%.6f,%.6f) \n", 
		m_fFrameItem[0+3],  m_fFrameItem[1+3], m_fFrameItem[2+3] );

	str = buf;

	std::sprintf(buf, "angle(%.4f,%.4f,%.3f) \n",
		m_fFrameItem[0],  m_fFrameItem[1], m_fFrameItem[2] );

	str += buf;

	m_bFrameTypeCurrent = FRAME_MIX;

	return str;
}

void StringParserPos::setDefault()
{
	m_sFrameItem.push_back( "yaw" );
	m_sFrameItem.push_back( "tilt" );
	m_sFrameItem.push_back( "roll" );
	m_sFrameItem.push_back( "latitude" );
	m_sFrameItem.push_back( "longitude" );
	m_sFrameItem.push_back( "altitude" );
	for (int i=0; i< ITEM_COUNT; i++)
		m_fFrameItem[i] = 0.0f;
}


void StringParserPos::parseValueFromString( )
{

	m_strVecAngle.clear();

	m_strVecAngle = StringUtility::tokenize( strCurrentFrame, "," );
	if( m_strVecAngle.size() >= 12 )
	{
		m_fFrameItem[YAW] = atof( m_strVecAngle[2].c_str() );
		m_fFrameItem[TILT] = atof( m_strVecAngle[4].c_str() );
		m_fFrameItem[ROLL] = atof( m_strVecAngle[6].c_str() );

		m_sFrameItemValue.push_back( m_strVecAngle[2].c_str() );
		m_sFrameItemValue.push_back( m_strVecAngle[4].c_str() );
		m_sFrameItemValue.push_back( m_strVecAngle[6].c_str() );

		m_fFrameItem[LATITUDE] = atof( m_strVecAngle[7].c_str() );
		m_fFrameItem[LONGITUDE] = atof( m_strVecAngle[8].c_str() );
		m_fFrameItem[ALTITUDE] = atof( m_strVecAngle[9].c_str() );

		m_sFrameItemValue.push_back( m_strVecAngle[7].c_str() );
		m_sFrameItemValue.push_back( m_strVecAngle[8].c_str() );
		m_sFrameItemValue.push_back( m_strVecAngle[9].c_str() );

	}
	
}

int StringParserPos::parseValueFromString( std::string& str )
{
	int nBegin = str.find(FRAME_STRING_START);

	if ( nBegin != -1 )
		printf("GPS frame begin at = %d \n", nBegin );


	parseValueFromString(  );

	int nEnd = str.find(FRAME_STRING_END);

	str = str.substr( nEnd+1, std::string::npos );

	m_nFrameCount ++;

	return nBegin;
}

bool StringParserPos::isFrameComplete( std::string& str )
{
	int nBegin = str.find(FRAME_STRING_START);

	if ( nBegin == -1 )
		return false;

	int nEnd = str.find(FRAME_STRING_END);

	if ( nEnd == -1 )
		return false;

	strCurrentFrame = str.substr( nBegin, nEnd );

	//if( strCurrentFrame.length() < FRAME_SIZE )
	//	return false;
	
	return true;
}
