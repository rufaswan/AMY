#include "data-intvect.hpp"

namespace rfs
{
	IntVect::IntVect() {}
	IntVect::~IntVect() {}

	void IntVect::insert( int d, uint offset )
	{
		if ( offset >= IntVect::data.size() )  IntVect::append(d);
		IntVect::data.insert( IntVect::data.begin() + offset, d );
	}

	void IntVect::insert( intvect &d, uint offset )
	{
		if ( offset >= IntVect::data.size() )  IntVect::append(d);
		IntVect::data.insert( IntVect::data.begin() + offset, d.begin(), d.end() );
	}

	void IntVect::insert( IntVect &d, uint offset )
	{
		if ( offset >= IntVect::data.size() )  IntVect::append(d);
		intvect e = d.getdata();
		IntVect::insert( e, offset );
	}

	void IntVect::append( intvect &d )
	{
		uint i;
		for ( i=0; i < d.size(); i++ )
			IntVect::data.push_back( d[i] );
	}

	void IntVect::append( IntVect &d )
	{
		intvect e = d.getdata();
		IntVect::append( e );
	}

	void IntVect::append( int d )  { IntVect::data.push_back(d); }
	void IntVect::clear()          { IntVect::data.clear(); }

	bool IntVect::del( uint index )
	{
		if ( index >= IntVect::data.size() )
			return false;
		IntVect::data.erase( IntVect::data.begin() + index );
		return true;
	}

	int IntVect::find( int d )
	{
		uint i;
		for ( i=0; i < IntVect::data.size(); i++ )
		{
			if ( IntVect::data[i] == d )
				return i;
		}
		return -1;
	}

	intvect IntVect::getdata()
	{
		intvect  ret;
		if ( ! IntVect::data.empty() )
		{
			uint i;
			uint s = IntVect::data.size();
			ret.resize( s );
			for ( i=0; i < s; i++ )
				ret[i] = IntVect::data[i];
		}
		return ret;
	}
}
