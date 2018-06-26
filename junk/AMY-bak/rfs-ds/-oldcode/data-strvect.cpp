#include "data-strvect.hpp"

namespace rfs
{
	StrVect::StrVect() {}
	StrVect::~StrVect() {}

	void StrVect::insert( const ascii &d, uint offset )
	{
		if ( offset >= StrVect::data.size() )  StrVect::append(d);
		StrVect::data.insert( StrVect::data.begin() + offset, d );
	}

	void StrVect::insert( strvect &d, uint offset )
	{
		if ( offset >= StrVect::data.size() )  StrVect::append(d);
		StrVect::data.insert( StrVect::data.begin() + offset, d.begin(), d.end() );
	}

	void StrVect::insert( StrVect &d, uint offset )
	{
		if ( offset >= StrVect::data.size() )  StrVect::append(d);
		strvect e = d.getdata();
		StrVect::insert( e, offset );
	}

	void StrVect::append( strvect &d )
	{
		uint i;
		for ( i=0; i < d.size(); i++ )
			StrVect::data.push_back( d[i] );
	}

	void StrVect::append( StrVect &d )
	{
		strvect e = d.getdata();
		StrVect::append( e );
	}

	void StrVect::append( const ascii &d )  { StrVect::data.push_back(d); }
	void StrVect::clear()                   { StrVect::data.clear(); }

	bool StrVect::del( uint index )
	{
		if ( index >= StrVect::data.size() )
			return false;
		StrVect::data.erase( StrVect::data.begin() + index );
		return true;
	}


	int StrVect::find( const ascii &d )
	{
		uint i;
		for ( i=0; i < StrVect::data.size(); i++ )
		{
			if ( StrVect::data[i] == d )
				return i;
		}
		return -1;
	}

	strvect StrVect::getdata()
	{
		strvect  ret;
		if ( ! StrVect::data.empty() )
		{
			uint i;
			uint s = StrVect::data.size();
			ret.resize( s );
			for ( i=0; i < s; i++ )
				ret[i] = StrVect::data[i];
		}
		return ret;
	}

}
