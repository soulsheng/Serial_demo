


#include "StdAfx.h"
#include <vgkStringUtility.h>

#include <algorithm>

namespace vgKernel {
	
	
	
	//----------------------------------------------------------------
	void StringUtility::trim( String& str, bool left /*= true*/,
		bool right /*= true */, const String& delstr /*= " \t\r"*/ )
	{
		//assert(str.length() != 0 );
		//static const String delims = " \t\r";
		if(right)
			str.erase(str.find_last_not_of(delstr)+1); // trim right
		if(left)
			str.erase(0, str.find_first_not_of(delstr)); // trim left
	}
	//----------------------------------------------------------------
	void StringUtility::toLowerCase( String& str )
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			tolower);
	}
	//----------------------------------------------------------------
	void StringUtility::toUpperCase( String& str )
	{
		transform( str.begin() , str.end() , str.begin() , toupper);
	}
	//----------------------------------------------------------------
	void StringUtility::splitFilename( const String& qualifiedName, 
		String& outBasename, String& outPath )
	{
		String path = qualifiedName;
		// Replace \ with / first
		std::replace( path.begin(), path.end(), '\\', '/' );
		// split based on final /
		size_t i = path.find_last_of('/');

		if (i == String::npos)
		{
			outPath = "";
			outBasename = qualifiedName;
		}
		else
		{
			outBasename = path.substr(i+1, path.size() - i - 1);
			outPath = path.substr(0, i+1);
		}

	}
	//----------------------------------------------------------------
	bool StringUtility::match( const String& str, const String& pattern, 
		bool caseSensitive /*= true*/ )
	{
		String tmpStr = str;
		String tmpPattern = pattern;
		if (!caseSensitive)
		{
			StringUtility::toLowerCase(tmpStr);
			StringUtility::toLowerCase(tmpPattern);
		}

		String::const_iterator strIt = tmpStr.begin();
		String::const_iterator patIt = tmpPattern.begin();
		String::const_iterator lastWildCardIt = tmpPattern.end();
		while (strIt != tmpStr.end() && patIt != tmpPattern.end())
		{
			if (*patIt == '*')
			{
				lastWildCardIt = patIt;
				// Skip over looking for next character
				++patIt;
				if (patIt == tmpPattern.end())
				{
					// Skip right to the end since * matches the entire rest of the string
					strIt = tmpStr.end();
				}
				else
				{
					// scan until we find next pattern character
					while(strIt != tmpStr.end() && *strIt != *patIt)
						++strIt;
				}
			}
			else
			{
				if (*patIt != *strIt)
				{
					if (lastWildCardIt != tmpPattern.end())
					{
						// The last wildcard can match this incorrect sequence
						// rewind pattern to wildcard and keep searching
						patIt = lastWildCardIt;
						lastWildCardIt = tmpPattern.end();
					}
					else
					{
						// no wildwards left
						return false;
					}
				}
				else
				{
					++patIt;
					++strIt;
				}
			}

		}
		// If we reached the end of both the pattern and the string, we succeeded
		if (patIt == tmpPattern.end() && strIt == tmpStr.end())
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	//----------------------------------------------------------------
	std::wstring StringUtility::convertStringToWString( const std::string& str )
	{
		int wcs_len = str.length() * 2;
		wchar_t *pwcs = new wchar_t[ wcs_len ];

		//assert(0&& "这里要改");
		//setlocale(LC_CTYPE, "chs");
		mbstowcs(pwcs, str.c_str(), wcs_len);

		std::wstring newword( pwcs );
		delete pwcs;

		return newword;
	}
	//----------------------------------------------------------------
	String StringUtility::getFilenameFromAbsolutePath( 
		const String& abs_path )
	{
		String::size_type found = abs_path.find_last_of( "\\/" );
		if ( found == String::npos )
		{
			//assert(0&&"非绝对路径");
		}

		return abs_path.substr( found + 1, String::npos );
	}
	//----------------------------------------------------------------
	String StringUtility::getFilenameWithoutExtension( 
		const String& filename )
	{
		String::size_type found = filename.find_last_of( "\\/" );
		if ( found != String::npos )
		{
			;//assert(0&&"绝对路径");
		}

		std::string::size_type pos = filename.rfind('.');

#if _DEBUG
		if ( pos == String::npos )
		{
			//assert(0);
		}

#endif


		return filename.substr( 0 , pos );
	}
	//----------------------------------------------------------------
	String StringUtility::getFileExtension( const String& filename )
	{
		std::string::size_type pos = filename.rfind('.');
		std::string ext = filename.substr(pos == std::string::npos ? filename.length() : pos + 1);
		transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		ext.erase(ext.find_last_not_of(' ') + 1);
		return ext;
	}
	//----------------------------------------------------------------
	String StringUtility::getPathFromAbsoluteFilename( const String& abs_filename )
	{
		String::size_type found = abs_filename.find_last_of( "\\/" );
		if ( found == String::npos )
		{
			//assert(0&&"非绝对路径");
		}

		return abs_filename.substr( 0 , found + 1 );
	}
	//----------------------------------------------------------------
	String StringUtility::getFilenameWithoutExtFromAbsPath( const String& abs_path )
	{
		String filename = getFilenameFromAbsolutePath( abs_path );

		return getFilenameWithoutExtension( filename );
	}
	//----------------------------------------------------------------
	std::string WChar2Ansi(LPCWSTR pwszSrc)
	{
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
		if (nLen<= 0) return std::string("");
		char* pszDst = new char[nLen];
		if (NULL == pszDst) return std::string("");
		WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
		pszDst[nLen -1] = 0;
		std::string strTemp(pszDst);
		delete [] pszDst;
		return strTemp;
	}
	std::string StringUtility::convertWStringToString( const std::wstring& wstr )
	{
		return WChar2Ansi( wstr.c_str() );
	}

	int StringUtility::replace( String& str, const String& pattern, const String& newpat )
	{
		int count = 0; 
		const size_t nsize = newpat.size(); 
		const size_t psize = pattern.size(); 

		for(size_t pos = str.find(pattern, 0);  
			pos != String::npos; 
			pos = str.find(pattern,pos + nsize)) 
		{ 
			str.replace(pos, psize, newpat); 
			count++; 
		} 

		return count;
	}
	//----------------------------------------------------------------
	vgKernel::StringVector StringUtility::tokenize( const String& src,
		const String& tok, bool trim/*=false*/, String null_subst/*=""*/ )
	{
		if( src.empty() || tok.empty() ) 
		{
			return StringVector();
		}

		StringVector ret;   

		String::size_type pre_index = 0, index = 0, len = 0;   
		while( (index = src.find_first_of(tok, pre_index)) != String::npos )   
		{   
			if( (len = index-pre_index)!=0 )   
				ret.push_back(src.substr(pre_index, len));   
			else if(trim==false)   
				ret.push_back(null_subst);   
			pre_index = index+1;   
		}   
		String endstr = src.substr(pre_index);   

		if( trim==false )
			ret.push_back( endstr.empty()? null_subst:endstr );   
		else if( !endstr.empty() ) 
			ret.push_back(endstr);   

		return ret;
	}
	//----------------------------------------------------------------
	vgKernel::StringVector StringUtility::split( const String& src, const String& delimit, const String& null_subst/*=""*/ )
	{
		if( src.empty() || delimit.empty() )
		{
			return StringVector();
		}

		StringVector ret;   

		String::size_type deli_len = delimit.size();   
		String::size_type index = String::npos, last_search_position = 0;   

		while( (index = src.find(delimit, 
			last_search_position)) != String::npos )   
		{   
			if(index==last_search_position)   
				ret.push_back(null_subst);   
			else  
				ret.push_back( src.substr(last_search_position, index-   

				last_search_position) );   
			last_search_position = index + deli_len;   
		}   

		String last_one = src.substr(last_search_position);   

		ret.push_back( last_one.empty()? null_subst:last_one );   


		return ret;
	}
	//----------------------------------------------------------------


}// end of namespace vgKernel
