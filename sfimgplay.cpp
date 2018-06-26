/*
 * To play a sequence of images like a movie
 *
 * -> RenderTexture -> RenderWindow =  6-8  fps + 100% CPU
 * -> RenderWindow                  = 58-60 fps
 *
 * This is the direct draw to RenderWindow ver.
 */
#include "zsf-common.hpp"
#include <vector>

typedef std::vector < std::string >  vecstr;

struct imgop
{
	bool    reverse;
	int     rate;
	int     no;
	vecstr  pics;
};

void set_opts( gamesys* sys, imgop* op, int argc, char* argv[] )
{
	sf::Image     sfimg;
	sf::Texture   sftex;
	int i = 1;
	int c = 0;
	while ( i < argc )
	{
		if ( argv[i][1] == 0 ) // single letter only , must be option
		{
			if ( argv[i][0] == 'x' )
			{
				printf("SET reverse\n");
				op->reverse = true;
				i++;
			}
			else if ( argv[i][0] == 'r' )
			{
				op->rate = strtol( argv[i+1], NULL, 0 );
				printf("SET rate %d\n", op->rate);
				i += 2;
			}
		}
		else
		{
			op->pics.push_back( argv[i] );
				c++;
			i++;
		}
	}
	printf("added %d images\n", c);
}

void game_update( gamesys* sys, imgop* op )
{
	int tex_no = op->pics.size();
	sf::Vector2u   siz;
	sf::Sprite     spr;
	sf::Texture    sftex;

	if ( op->no < 0 )
		op->no = tex_no - 1;
	else
		op->no = op->no % tex_no;

	sftex.loadFromFile( op->pics[op->no] );
	siz = sftex.getSize();
		//printf("img siz %d x %d\n", siz.x, siz.y);
		spr.setTexture( sftex );
		spr.setScale( (float)sys->m_w / siz.x, (float)sys->m_h / siz.y );

	sys->m_win.draw(spr);
	op->no = ( op->reverse ) ? (op->no)-1 : (op->no)+1;
}

int main(int argc, char* argv[])
{
	if ( argc == 1 )   return -1;

	gamesys* sys = new gamesys;
		sys->create_win( 800, 600, argv[0] );
	imgop  opt;
	imgop* op = &opt;
		op->reverse = false;
		op->rate    = 4;
		op->no      = 0;
		op->pics.reserve(argc);
	set_opts  ( sys, op, argc, argv );

	sf::Thread thd_fps( &fps_watch, sys);
		thd_fps.launch();

	while( sys->m_win.isOpen() )
	{
		sys->input_handler();
		sys->event_handler();

		if ( ! sys->m_paused )
		{
			sys->m_win.setFramerateLimit( op->rate );
			game_update( sys, op );
		}

		sys->m_win.display();
		sys->m_fps++;
	}

	printf("Bye!\n");
	return 0;
}
