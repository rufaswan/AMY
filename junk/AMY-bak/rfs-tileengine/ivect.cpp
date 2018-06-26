#include "define.h"
using namespace amy;

iVect::iVect()
{
	iVect::locked = false;
	iVect::clear();
}
iVect::~iVect() {}

int iVect::at(unsigned int e)	{	return ( iVect::size > e ) ? iVect::ivec[e] : 0; }
int iVect::first()				{	return ( iVect::size > 0 ) ? iVect::ivec[0] : 0; }
int iVect::last()				{	return ( iVect::size > 0 ) ? iVect::ivec[ iVect::size - 1 ] : 0; }
bool iVect::empty()				{	return ( iVect::size > 0 ) ? false : true; }

void iVect::assign(int array[], unsigned int size, bool lock)
{
	if ( ! iVect::locked )
	{
		unsigned int i;
		for ( i=0; i<size; i++ )
			iVect::ivec.push_back( array[i] );

		iVect::size = size;
		iVect::locked = lock;
	}
}

void iVect::clear()
{
	if ( ! iVect::locked )
	{
		iVect::size = 0;
		iVect::ivec.clear();
	}
}

