
#include "StdAfx.h"
#include "StringParserStart.h"

#define		FRAME_SIZE		151

StringParserStart::StringParserStart()
{
	m_bFrameValid[0] = false;
	m_bFrameValid[1] = false;

	m_bFrameTypeCurrent = FRAME_NULL;

	m_nFrameCount = 0;

	setDefault();
}



float* StringParserStart::getAngle()
{
	return m_fFrameItem;
}

vgKernel::StringVector* StringParserStart::getStringVector()
{
	return &m_sFrameItemValue;
}

FRAME_TYPE StringParserStart::getFrameType()
{
	return m_bFrameTypeCurrent;
}

bool StringParserStart::isFrameValid()
{
	return m_bFrameTypeCurrent != FRAME_NULL;
}

std::string StringParserStart::formatCurrentFrameToString()
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

void StringParserStart::setDefault()
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


void StringParserStart::parseValueFromString( )
{
	std::string str = strCurrentFrame;

	for(int i=0; i<ITEM_COUNT; i++)
	{

		int nIndexFindBegin = str.find( m_sFrameItem[i] );

		if( nIndexFindBegin == -1 )
			return;
		nIndexFindBegin += m_sFrameItem[i].size() + 1;
		int nIndexFindEnd = str.find("\r\n", nIndexFindBegin);

		std::string strSub = str.substr( nIndexFindBegin, nIndexFindEnd - nIndexFindBegin );

		m_fFrameItem[i] = atof( strSub.c_str() );
	
		m_sFrameItemValue.push_back( strSub );

		str = str.substr( nIndexFindEnd + 2 );	// remove substring 

	}

}

int StringParserStart::parseValueFromString( std::string& str )
{
	int nBegin = str.find("start");

	if ( nBegin != -1 )
		printf("GPS frame begin at = %d \n", nBegin );


	parseValueFromString(  );

	int nEnd = str.find("end");

	str = str.substr( nEnd+1, std::string::npos );

	m_nFrameCount ++;

	return nBegin;
}

bool StringParserStart::isFrameComplete( std::string& str )
{
	int nBegin = str.find("start");

	if ( nBegin == -1 )
		return false;

	int nEnd = str.find("end");

	if ( nEnd == -1 )
		return false;

	strCurrentFrame = str.substr( nBegin, nEnd );

	//if( strCurrentFrame.length() < FRAME_SIZE )
	//	return false;
	
	return true;
}
