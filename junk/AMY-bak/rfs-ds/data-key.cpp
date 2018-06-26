#include "header.hpp"
using namespace amy;

KeyData::KeyData()
{
	KeyData::reset();
}
KeyData::~KeyData() {}

void KeyData::reset()
{
	KeyData::U = false;
	KeyData::D = false;
	KeyData::L = false;
	KeyData::R = false;

	KeyData::a = false;
	KeyData::b = false;
	KeyData::c = false;
	KeyData::d = false;
	KeyData::e = false;
	KeyData::f = false;

	KeyData::g = false;
	KeyData::h = false;
}

uint KeyData::save()
{
	uint k = 0;

	if ( KeyData::U )  k += ( 1 << 0 );
	if ( KeyData::D )  k += ( 1 << 1 );
	if ( KeyData::L )  k += ( 1 << 2 );
	if ( KeyData::R )  k += ( 1 << 3 );

	if ( KeyData::a )  k += ( 1 << 4 );
	if ( KeyData::b )  k += ( 1 << 5 );
	if ( KeyData::c )  k += ( 1 << 6 );
	if ( KeyData::d )  k += ( 1 << 7 );
	if ( KeyData::e )  k += ( 1 << 8 );
	if ( KeyData::f )  k += ( 1 << 9 );

	if ( KeyData::g )  k += ( 1 << 10 );
	if ( KeyData::h )  k += ( 1 << 11 );

	return k;
}

void KeyData::load( int k )
{
	if ( k & KeyData::KU )  KeyData::U = true;
	if ( k & KeyData::KD )  KeyData::D = true;
	if ( k & KeyData::KL )  KeyData::L = true;
	if ( k & KeyData::KR )  KeyData::R = true;

	if ( k & KeyData::Ka )  KeyData::a = true;
	if ( k & KeyData::Kb )  KeyData::b = true;
	if ( k & KeyData::Kc )  KeyData::c = true;
	if ( k & KeyData::Kd )  KeyData::d = true;
	if ( k & KeyData::Ke )  KeyData::e = true;
	if ( k & KeyData::Kf )  KeyData::f = true;

	if ( k & KeyData::Kg )  KeyData::g = true;
	if ( k & KeyData::Kh )  KeyData::h = true;
}

void KeyData::merge( int k )
{
	if ( KeyData::U || ( k & KeyData::KU ) )  KeyData::U = true;
	if ( KeyData::D || ( k & KeyData::KD ) )  KeyData::D = true;
	if ( KeyData::L || ( k & KeyData::KL ) )  KeyData::L = true;
	if ( KeyData::R || ( k & KeyData::KR ) )  KeyData::R = true;

	if ( KeyData::a || ( k & KeyData::Ka ) )  KeyData::a = true;
	if ( KeyData::b || ( k & KeyData::Kb ) )  KeyData::b = true;
	if ( KeyData::c || ( k & KeyData::Kc ) )  KeyData::c = true;
	if ( KeyData::d || ( k & KeyData::Kd ) )  KeyData::d = true;
	if ( KeyData::e || ( k & KeyData::Ke ) )  KeyData::e = true;
	if ( KeyData::f || ( k & KeyData::Kf ) )  KeyData::f = true;

	if ( KeyData::g || ( k & KeyData::Kg ) )  KeyData::g = true;
	if ( KeyData::h || ( k & KeyData::Kh ) )  KeyData::h = true;
}

void KeyData::xmerge( int k )
{
	if ( !KeyData::U || !( k & KeyData::KU ) )  KeyData::U = false;
	if ( !KeyData::D || !( k & KeyData::KD ) )  KeyData::D = false;
	if ( !KeyData::L || !( k & KeyData::KL ) )  KeyData::L = false;
	if ( !KeyData::R || !( k & KeyData::KR ) )  KeyData::R = false;

	if ( !KeyData::a || !( k & KeyData::Ka ) )  KeyData::a = false;
	if ( !KeyData::b || !( k & KeyData::Kb ) )  KeyData::b = false;
	if ( !KeyData::c || !( k & KeyData::Kc ) )  KeyData::c = false;
	if ( !KeyData::d || !( k & KeyData::Kd ) )  KeyData::d = false;
	if ( !KeyData::e || !( k & KeyData::Ke ) )  KeyData::e = false;
	if ( !KeyData::f || !( k & KeyData::Kf ) )  KeyData::f = false;

	if ( !KeyData::g || !( k & KeyData::Kg ) )  KeyData::g = false;
	if ( !KeyData::h || !( k & KeyData::Kh ) )  KeyData::h = false;
}

