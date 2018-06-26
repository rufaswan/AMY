#include "header.hpp"
using namespace amy;
extern PlayData DATA;

Input::Input()
{
	Input::idle        = 0;
	Input::focused     = true;
	Input::joystick    = false;
	Input::joystick_id = 0;
	Input::sshot_name  = "amy";
	Input::sshot_no    = 0;
	Input::kb_scheme   = 1;
}
Input::~Input() {}

void Input::raw_input( sf::RenderWindow &screen, sf::RenderTexture &pre )
{
	// EVENT LOOP, KEYBOARD Input Delay applied
	sf::Event EV;
	while ( screen.pollEvent(EV) )
	{
		// when clicked the X (close) button on the title bar
		if ( EV.type == EV.Closed )
			Input::ev_close( screen );

		// when clicked the maximize/restore button on the title bar
		if ( EV.type == EV.Resized )
		{
			// update the view to the new size of the window
			sf::FloatRect visibleArea(0, 0, EV.size.width, EV.size.height);
			screen.setView( sf::View(visibleArea) );
		}

		// is the window active?
		if ( EV.type == EV.LostFocus )
			Input::focused = false;
		if ( EV.type == EV.GainedFocus )
			Input::focused = true;

		// joystick related
		if ( EV.type == EV.JoystickDisconnected )
		{
			Input::joystick = false;
			Input::joystick_id = 0;
		}
		if ( EV.type == EV.JoystickConnected )
		{
			Input::joystick = true;
			Input::joystick_id = EV.joystickConnect.joystickId;
		}
	}

	// REAL TIME INPUT, no delay
	// keyboard related
	Input::keyboard_handle( screen, pre, Input::key );

}

void Input::handle_input()
{
	Input::idle++;

	// for command moves
	Input::keylist.push_front( Input::key.save() );
	if ( Input::keylist.size() > 99 )
		Input::keylist.pop_back();

	Input::key.reset();
}

void Input::keyboard_handle( sf::RenderWindow &screen, sf::RenderTexture &pre, amy::KeyData &key )
{
	sf::Keyboard kb;

	if ( kb.isKeyPressed(kb.Escape) )  Input::ev_close( screen );
	if ( kb.isKeyPressed(kb.F12) )     Input::ev_sshot( pre );
	if ( kb.isKeyPressed(kb.F9 ) )     DATA.rec->start( pre );
	if ( kb.isKeyPressed(kb.F10) )     DATA.rec->stop ( pre );

	if ( Input::kb_scheme == 1 ) // arrow keys scheme
	{
		if ( kb.isKeyPressed(kb.Up) )        Input::set_key( key.U );
		if ( kb.isKeyPressed(kb.Down) )      Input::set_key( key.D );
		if ( kb.isKeyPressed(kb.Left) )      Input::set_key( key.L );
		if ( kb.isKeyPressed(kb.Right) )     Input::set_key( key.R );
		if ( kb.isKeyPressed(kb.Z) )         Input::set_key( key.a );
		if ( kb.isKeyPressed(kb.X) )         Input::set_key( key.b );
		if ( kb.isKeyPressed(kb.C) )         Input::set_key( key.c );
		if ( kb.isKeyPressed(kb.A) )         Input::set_key( key.d );
		if ( kb.isKeyPressed(kb.S) )         Input::set_key( key.e );
		if ( kb.isKeyPressed(kb.D) )         Input::set_key( key.f );
		if ( kb.isKeyPressed(kb.Space) )     Input::set_key( key.g );
		if ( kb.isKeyPressed(kb.LControl) )  Input::set_key( key.h );
	}
	else if ( Input::kb_scheme == 2 ) // WASD scheme
	{
		if ( kb.isKeyPressed(kb.W) )         Input::set_key( key.U );
		if ( kb.isKeyPressed(kb.S) )         Input::set_key( key.D );
		if ( kb.isKeyPressed(kb.A) )         Input::set_key( key.L );
		if ( kb.isKeyPressed(kb.S) )         Input::set_key( key.R );
		if ( kb.isKeyPressed(kb.Down) )      Input::set_key( key.a );
		if ( kb.isKeyPressed(kb.Left) )      Input::set_key( key.b );
		if ( kb.isKeyPressed(kb.Up) )        Input::set_key( key.c );
		if ( kb.isKeyPressed(kb.Right) )     Input::set_key( key.d );
		if ( kb.isKeyPressed(kb.LControl) )  Input::set_key( key.e );
		if ( kb.isKeyPressed(kb.LAlt) )      Input::set_key( key.f );
		if ( kb.isKeyPressed(kb.Return) )    Input::set_key( key.g );
		if ( kb.isKeyPressed(kb.Space) )     Input::set_key( key.h );
	}
}

void Input::set_key( bool &k )
{
	k = true;
	Input::idle = 0;
	if ( Input::idle > (FPS*2) )
		Input::keylist.clear();
}

void Input::ev_close( sf::RenderWindow &screen )
{
	// until the recording is done, disable close
	if ( DATA.rec->done )  screen.close();
}

void Input::ev_sshot( sf::RenderTexture &pre )
{
	std::fstream fs;
	std::string str;

	bool done = false;
	while ( !done )
	{
		str = Input::sshot_name + "-" + DATA.util.int2str( Input::sshot_no, 4 ) + ".png";

		DATA.util.fopen( fs, str, "rb" );
		// screenshot exists, skipped
		if ( fs.good() )
		{
			Input::sshot_no++;
			fs.close();
		}

		// screenshot not found, create it
		else
		{
			// sf::Texture TEX = pre.getTexture();
			// sf::Image IMG = TEX.copyToImage();
			// IMG.saveToFile( str );
			pre.getTexture().copyToImage().saveToFile( str );
			done = true;
		}
	}
}

bool Input::is_nopress( uint pos )
{
	if ( Input::keylist[ pos ] == 0 )
		return true;
	return false;
}

bool Input::is_pressed( const char key, uint pos )
{
	amy::KeyData kd;
	kd.load( Input::keylist[ pos ] );

	switch ( key )
	{
		case 'U': return kd.U;
		case 'D': return kd.D;
		case 'L': return kd.L;
		case 'R': return kd.R;
		case 'a': return kd.a;
		case 'b': return kd.b;
		case 'c': return kd.c;
		case 'd': return kd.d;
		case 'e': return kd.e;
		case 'f': return kd.f;
		case 'g': return kd.g;
		case 'h': return kd.h;
	}
	return false;
}

bool Input::is_holded( const char key, uint dur, uint pos )
{
	// no enough data in queue
	if ( Input::keylist.size() < (pos+1+dur) )
		return false;

	amy::KeyData kd;
	uint i;

	kd.load( Input::keylist[ pos+0 ] );
	for ( i=1; i < dur; i++ )
		kd.xmerge( Input::keylist[ pos+i ] );
	//printf("kd %i\n", kd.save() );

	switch ( key )
	{
		case 'U': return kd.U;
		case 'D': return kd.D;
		case 'L': return kd.L;
		case 'R': return kd.R;
		case 'a': return kd.a;
		case 'b': return kd.b;
		case 'c': return kd.c;
		case 'd': return kd.d;
		case 'e': return kd.e;
		case 'f': return kd.f;
		case 'g': return kd.g;
		case 'h': return kd.h;
	}
	return false;
}
