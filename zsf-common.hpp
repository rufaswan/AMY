#ifndef ZSF_COM_HPP
#define ZSF_COM_HPP
/*
 * Useful funcs to build SFML apps
 *
 */
#include "rfstk.hpp"
#include "class_gamesys.hpp"
#include <SFML/Graphics.hpp>

void draw_fps_no( gamesys* sys, sf::Font &font )
{
	char fps[10];
		rfs::memset( fps, 10 );
		sprintf(fps, "%d FPS", sys->m_fps_no);

	sf::Text   text(fps, font);
		text.setCharacterSize( sys->m_h / 16 );
	sys->m_win.draw(text);
}

void fps_watch( gamesys* sys )
{
	sf::Time msec;
	for(;;)
	{
		printf("FPS %d\n", sys->m_fps);

		sys->m_fps_no = sys->m_fps;
		sys->m_fps    = 0;
		msec = sf::milliseconds(1000);
			sf::sleep(msec);

		if ( sys->m_closed )
			return;
	}
}

char get_sfkey( int sfkey )
{
	if ( rfs::in_range(sfkey, (int)sf::Keyboard::A, (int)sf::Keyboard::Z) )
		return 'A' + (sfkey - sf::Keyboard::A);
	if ( rfs::in_range(sfkey, (int)sf::Keyboard::Num0, (int)sf::Keyboard::Num9) )
		return '0' + (sfkey - sf::Keyboard::Num0);
/*
	if ( key == "SPACE" )    return (int)sf::Keyboard::Space;
	if ( key == "ENTER" )    return (int)sf::Keyboard::Return;
	if ( key == "UP"    )    return (int)sf::Keyboard::Up;
	if ( key == "DOWN"  )    return (int)sf::Keyboard::Down;
	if ( key == "LEFT"  )    return (int)sf::Keyboard::Left;
	if ( key == "RIGHT" )    return (int)sf::Keyboard::Right;
 */
}

int sfkey( char key )
{
	if ( rfs::in_range(key, 'a', 'z') )
		return (int)sf::Keyboard::A + (key - 'a');
	if ( rfs::in_range(key, 'A', 'Z') )
		return (int)sf::Keyboard::A + (key - 'A');
	if ( rfs::in_range(key, '0', '9') )
		return (int)sf::Keyboard::Num0 + (key - '0');

	if ( key == ' ' )   return sf::Keyboard::Space;
	//if ( key == '' )   return sf::Keyboard::;
	return -1;
}

int sfkey( std::string &key )
{
	if ( key[1] == 0 )
		return sfkey( key[0] );
	if ( key[0] == 'N' )
	{
		if ( rfs::in_range(key[1], '0', '9') )
			return sf::Keyboard::Numpad0 + (key[1] - '0');
	}
	if ( key == "SPACE" )    return (int)sf::Keyboard::Space;
	if ( key == "ENTER" )    return (int)sf::Keyboard::Return;
	if ( key == "UP"    )    return (int)sf::Keyboard::Up;
	if ( key == "DOWN"  )    return (int)sf::Keyboard::Down;
	if ( key == "LEFT"  )    return (int)sf::Keyboard::Left;
	if ( key == "RIGHT" )    return (int)sf::Keyboard::Right;
	//if ( key == "" )    return sf::Keyboard::;
	return -1;
}
int sfkey( const char key[] )
{
	std::string k = key;
	return sfkey(k);
}

bool is_sfkey( std::string &key )
{
	return sf::Keyboard::isKeyPressed( (sf::Keyboard::Key)sfkey(key) );
}

bool is_sfkey( char key )
{
	return sf::Keyboard::isKeyPressed( (sf::Keyboard::Key)sfkey(key) );
}
#endif // ZSF_COM_HPP
