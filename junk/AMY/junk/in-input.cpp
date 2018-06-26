#include "in-input.hpp"

namespace amy
{
	Input::Input()  {}
	Input::~Input() {}

	void Input::reg_kb( uchar key, sf::Keyboard::Key  sfkey )
	{
		// register new key
		if ( Input::kbmap.find(key) == Input::kbmap.end() )
		{
			Input::kbmap[ key ] = new amy::Key();
		}

		// update the key
		Input::kbmap[ key ]->kb = sfkey;

	}

	void Input::tick( amy::Display &display )
	{
		Input::tick_ev( display );
		Input::tick_kb();
	}

	void Input::tick_ev( amy::Display &display )
	{
		// EVENT LOOP, KEYBOARD Input Delay applied
		sf::Event EV;
		while ( display.sfwin.pollEvent(EV) )
		{
			// when clicked the X (close) button on the title bar
			if ( EV.type == EV.Closed )
				display.sfwin.close();

			// when clicked the maximize/restore button on the title bar
			if ( EV.type == EV.Resized )
			{
				// update the view to the new size of the window
				sf::FloatRect  visibleArea(0, 0, EV.size.width, EV.size.height);
				display.sfwin.setView( sf::View(visibleArea) );
				display.zoom.set( 100 * EV.size.width / display.size.w, 100 * EV.size.height / display.size.h );
			}

			if ( EV.type == EV.MouseButtonPressed )
				Input::mouse.set( EV.mouseButton.x, EV.mouseButton.y );
		}
	}

	void Input::tick_kb()
	{
	}
}
