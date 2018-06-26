#include "in-display.hpp"

namespace amy
{
	Display::Display()
	{
		Display::is_created = false;
		Display::is_open    = true;
	}
	Display::~Display() {}

	void Display::create( uint w, uint h, const ascii &title, uint zoom )
	{
		if ( Display::is_created )  return;

		Display::sfwin.create( sf::VideoMode( w, h ), title );
		Display::sfwin.setMouseCursorVisible( true );
		Display::sfwin.setPosition( sf::Vector2i(0,0) );
		Display::sfwin.setVerticalSyncEnabled( true );

		Display::sfrend.create( w, h );
		Display::zoom.set( zoom, zoom );
		Display::size.set( w, h );
		Display::is_created = true;
	}

	void Display::draw()
	{
		Display::is_open = Display::sfwin.isOpen();

		sf::Sprite  sfspr;

		sfspr.setTexture( Display::sfrend.getTexture() );
		sfspr.setScale( 0.01 * Display::zoom.x, 0.01 * Display::zoom.y );

		Display::sfwin.clear();
		Display::sfwin.draw( sfspr );
		Display::sfwin.display();
	}
}
