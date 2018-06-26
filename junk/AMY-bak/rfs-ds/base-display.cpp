#include "header.hpp"
using namespace amy;
extern PlayData DATA;

Display::Display()
{
	Display::next_screen = GameMaster::STG_DEMO;
	Display::is_running  = true;
	Display::preScreen.create(   WIDTH, HEIGHT*2 );
	Display::preScreen_n.create( WIDTH, HEIGHT );
	Display::preScreen_d.create( WIDTH, HEIGHT );
	DATA.camera.reset();
	DATA.camera.set_range(0, 0, WIDTH, HEIGHT);
}
Display::~Display() {}
std::string Display::next() { return Display::next_screen; }
void Display::on_enter( sf::RenderWindow &screen ) {}
void Display::updates ( sf::RenderWindow &screen ) {}
void Display::on_exit ( sf::RenderWindow &screen ) {}

void Display::fadein( sf::RenderWindow &screen )
{
	uint alpha;
	for ( alpha  = 0; alpha < 255; alpha += 5 )
	{
		Display::display( screen, alpha );
		screen.display();
	}
}

void Display::fadeout( sf::RenderWindow &screen )
{
	uint alpha;
	for ( alpha  = 255; alpha > 0; alpha -= 5 )
	{
		Display::display( screen, alpha );
		screen.display();
	}
}

void Display::running( sf::RenderWindow &screen, uint msec )
{
	printf(">> Display::running( %i )\n", msec );

	// init for fadein effect
	Display::handles( screen );
	this->updates( screen );

	Display::fadein( screen );
	this->on_enter( screen );

	// game while loop for running the game
	while ( Display::is_running )
	{
		Display::raw_input( screen );

		if ( Display::timer.can_msec_update( msec ) )
		{
			Display::handles( screen );
			this->updates( screen );
			Display::display( screen );

			if ( DATA.debug )
				Display::draw_debug( screen );
		}

		screen.display();
	}

	this->on_exit( screen );
	Display::fadeout( screen );
}

void Display::raw_input( sf::RenderWindow &screen )
{
	// exit the GameMaster loop
	Display::is_running = ( screen.isOpen() ) ? true : false;

	DATA.input.raw_input( screen, Display::preScreen_n );
}

void Display::handles( sf::RenderWindow &screen )
{
	DATA.input.handle_input();

	DATA.rec->addframe( Display::preScreen_n );
	DATA.rec->run();

	Display::preScreen_n.clear();
	Display::preScreen_d.clear( sf::Color(0x44,0x44,0x44) );
}

void Display::display( sf::RenderWindow &screen, uint alpha )
{
	if ( alpha > 255 ) alpha = 255;

	uint x, y;
	float fx, fy, fz;
	sf::Sprite SPR_N, SPR_D, SPR;

	// merge two rendertexture into one display
	Display::preScreen.clear();

		Display::preScreen_n.display();
		SPR_N.setTexture( Display::preScreen_n.getTexture() );
		SPR_N.setPosition( 0, 0 );
		SPR_N.setColor( sf::Color(255, 255, 255, alpha) );
		Display::preScreen.draw( SPR_N );

		Display::preScreen_d.display();
		SPR_D.setTexture( Display::preScreen_d.getTexture() );
		SPR_D.setPosition( 0, HEIGHT );
		SPR_D.setColor( sf::Color(255, 255, 255, alpha) );
		Display::preScreen.draw( SPR_D );

	Display::preScreen.display();

	// draw the preScreen according to the screen size
	// while keeping the aspect ratio
	screen.clear();

		sf::Vector2u ssize( screen.getSize() );
		//printf("ssize.x = %i, y = %i\n", ssize.x, ssize.y);

		fx = (float)ssize.x /  WIDTH;
		fy = (float)ssize.y / (HEIGHT*2);
		fz = ( fx > fy ) ? fy : fx;
		//printf("fx = %g, fy = %g, fz = %g\n", fx, fy, fz);

		x = ( ssize.x - ( WIDTH   * fz ) ) / 2;
		y = ( ssize.y - ( HEIGHT*2* fz ) ) / 2;
		//printf("x = %i, y = %i\n", x, y);

		SPR.setTexture( Display::preScreen.getTexture() );
		SPR.setPosition( x, y );
		SPR.setScale( fz, fz );
		screen.draw( SPR );

}

void Display::draw_debug( sf::RenderWindow &screen )
{
	sf::Vector2u ssize( screen.getSize() );
	std::stringstream ss;

	sf::Text text;
		text.setFont( DATA.sffont );
		text.setCharacterSize( 16 );
		text.setColor( WHITE );

	// show camera position
	// top-right corner
		ss.str("");
			ss << " cam_x : " << DATA.camera.pos_x;
			text.setString( ss.str() );
			text.setPosition( 8, 8);
			screen.draw( text );

		ss.str("");
			ss << " cam_y : " << DATA.camera.pos_y;
			text.setString( ss.str() );
			text.setPosition( 8, 32);
			screen.draw( text );

	// show FPS (Frame per Seconds) of current running game
	// bottom-right corner
		uint fps = Display::timer.get_fps();
		ss.str("");
				ss << fps << " fps";

		text.setString( ss.str() );
		sf::IntRect RECT( text.getGlobalBounds() );
			text.setPosition( ssize.x - RECT.width - 8, ssize.y - RECT.height - 8);
			screen.draw( text );

	// show recording / processing message until done
	// bottom-left corner
		if ( !DATA.rec->done )
		{
			// if stop command was sent
			if ( DATA.rec->stopped )
				text.setString( "Processing..." );
			else
				text.setString( "Recording..." );

			sf::IntRect RECT( text.getGlobalBounds() );
				text.setPosition( 8, ssize.y - RECT.height - 8);
				screen.draw( text );
		}

}
