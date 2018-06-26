#ifndef RFSTK_TXT_HPP
#define RFSTK_TXT_HPP

#include <string>
#include <vector>
typedef std::vector < std::string >  vecstr;

namespace rfs {

	bool is_same( std::string &str1, std::string &str2 )
	{
		return ( str1 == str2 );
	}

	void strtoupper( std::string &str )
	{
		int i = 0;
		while ( i < str.size() )
		{
			if ( str[i] >= 0x61 && str[i] <= 0x7a )
				str[i] -= 0x20;
			i++;
		}
	}

	void strtolower( std::string &str )
	{
		int i = 0;
		while ( i < str.size() )
		{
			if ( str[i] >= 0x41 && str[i] <= 0x5a )
				str[i] += 0x20;
			i++;
		}
	}

	int strpos( std::string &str, char find )
	{
		int i = 0;
		while ( i < str.size() )
		{
			if ( str[i] == find )
				return i;
			i++;
		}
		return 0;
	}

	int strrpos( std::string &str, char find )
	{
		int i = 0;
		int pos;
		while ( i < str.size() )
		{
			if ( str[i] == find )
				pos = i;
			i++;
		}
		return pos;
	}

	void str_replace( std::string &str, char find, char replace )
	{
		int i = 0;
		while ( i < str.size() )
		{
			if ( str[i] == find )
				str[i] = replace;
			i++;
		}
	}

	void explode( std::string &str, char sep, vecstr &array )
	{
		array.clear();
		int i = 0;
		std::string dummy = "";

		array.push_back(dummy);
		while ( i < str.size() )
		{
			if ( str[i] == sep )
				array.push_back(dummy);
			else
				array.back() += str[i];
			i++;
		}
	}
	void split( std::string &str, char sep, vecstr &array )
	{
		explode( str, sep, array );
	}

	void implode( std::string &str, char sep, vecstr &array )
	{
		str = array[0];
		int i;
		for ( i=1; i < array.size(); i++ )
		{
			str += sep;
			str += array[i];
		}
	}
	void join( std::string &str, char sep, vecstr &array )
	{
		implode( str, sep, array );
	}

} // namespace rfs
#endif // RFSTK_TXT_HPP

