#ifndef RFS_GOBJECT
#define RFS_GOBJECT

#include "defs.hpp"
#include "data-struct.hpp"
#include "data-rect.hpp"

namespace rfs
{
	class GObject
	{
		public:
			GObject();
			~GObject();

			void init();
			void setrect( int x, int y, int w, int h );

			int  command_id;
			bool is_alive;
			rfs::Pos  pos;
			rfs::Size size;
			rfs::Rect  rect;
	};
}

#endif

/*
	init()
		animation.register_command( int id, bool loop, ... )
		animation.register_command( int id, bool loop, ... )
		animation.register_command( int id, bool loop, ... )
		animation.register_command( int id, bool loop, ... )
		animation.register_command( int id, bool loop, ... )
		ai.register_command( int no, ... )

	control() // ver player
		int new_command = 0;
		if ginput.key['j']  new_command = 1;
		if ginput.key['d']  new_command = 2;
		if ginput.key['s']  new_command = new_command + 1;

		if ( command_id != new_command )
			command_id = new_command

	control() // ver enemy
		if ( ai.is_done )
			// get a new command
			command_id = ai.new_command()

	control() // ver cutscene


	update()
		control()
		if animation.id == command_id
			animation.resume()
		else
			animation.change( command_id )

	set_command()
	change_command()
	resume_command()

*/
