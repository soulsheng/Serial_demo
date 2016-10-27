

#ifndef __VGKSTRINGUTILITY_H__
#define __VGKSTRINGUTILITY_H__

#include <string>
#include <vector>
#include <map>

namespace vgKernel {

	typedef	std::string String;
	typedef std::vector<std::string> StringVector;
	typedef std::vector< std::pair<String,StringVector> > StringVectorArr;


	class  StringUtility
	{
	private:
		StringUtility()
		{

		}
	public:
		~StringUtility()
		{

		}
	public:

		template< typename Source >
		static String textCast(const Source &arg)
		{
			std::ostringstream result;
			result << arg;

			return result.str();
		}

		/// 将数据左右的诸如空格等信息删除.
		static void trim( String& str, bool left = true, 
			bool right = true , const String& delstr = " \t\r");

		static int replace( String& str,  const String& pattern, 
			const String& newpat); 

		/// 将字符串变为小写.
		static void toLowerCase( String& str );

		 /// 将字符串变为大写.
		static void toUpperCase( String& str );

		static std::wstring convertStringToWString( const std::string& str );

		static std::string convertWStringToString( const std::wstring& wstr );

		/**
			Method for splitting a fully qualified filename into the base name
			and path.
			@remarks
			Path is standardised as in standardisePath
		*/
		static void splitFilename(const String& qualifiedName,
			String& outBasename, String& outPath);

		/** 
			Simple pattern-matching routine allowing a wildcard pattern.
			@param str String to test
			@param pattern Pattern to match against; can include simple '*' wildcards
			@param caseSensitive Whether the match is case sensitive or not
		*/
		static bool match(const String& str, const String& pattern,
			bool caseSensitive = true);


		static String getFilenameFromAbsolutePath( const String& abs_path );

		static String getFilenameWithoutExtFromAbsPath( const String& abs_path );

		/**
			返回的保证以\或者/结尾
		*/
		static String getPathFromAbsoluteFilename( const String& abs_filename );

		static String getFilenameWithoutExtension( const String& filename );


		static String getFileExtension( const String& filename );



		/**
			tok 是分隔的字符集
			trim 提示是否保留空串,false为保留
			null_subst 提示对空串进行的替代字符.

			比如:
			string src = ",   ab,cde;,,fg,," ;   
			string tok = ",;" ;   

			vector<string> v2 = tokenize(src, tok ,false, "<null>");   

			v2输出:
			<null>
			ab
			cde
			<null>
			<null>
			fg
			<null>
			<null>

		*/
		static StringVector tokenize(const String& src, const String& tok,
								bool trim=false, String null_subst = "" );   


		/**
			与tokenize的区别在于, delimit查找是精确的.
			如:
			string s = "######123#4###56########789###";   
			string del = "###";   
			vector<string> v3 = split(s, del, "<null>");   

			输出:

			<null>
			<null>
			123#4
			56
			<null>
			##789
			<null>

		*/

		static StringVector split(const String& src, const String& delimit, 
						   const String& null_subst = "" );   


	};
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKSTRINGUTILITY_H__