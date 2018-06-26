#include "header.hpp"
using namespace amy;
extern PlayData DATA;

World::World()
{
	World::clear();
}
World::~World() {}

void World::clear()
{
	World::ready       = false;
	World::bg_ready    = false;
	World::fg_ready    = false;
	World::dbg_ready   = false;
	World::data_ready  = false;
	World::map_w  = 0;
	World::map_h  = 0;
	World::tile_w = 0;
	World::tile_h = 0;
}

void World::set_dbg()
{
	World::dbg_ready = true;
}

void World::set_bglayer( const std::string &file )
{
	printf(">> World::set_bglayer( %s )\n", file.c_str() );
	World::bgimage = DATA.file.data[ file ].get_sfimage();
	World::bg_ready = true;
}

void World::set_fglayer( const std::string &file )
{
	printf(">> World::set_fglayer( %s )\n", file.c_str() );
	World::fgimage = DATA.file.data[ file ].get_sfimage();
	World::fg_ready = true;
}

void World::set_mapdata( const std::string &file, uint map_w, uint map_h, uint tile_w, uint tile_h )
{
	printf(">> World::set_tiledata( %s, %i, %i, %i, %i )\n", file.c_str(), map_w, map_h, tile_w, tile_h );
	World::map_w  = map_w;
	World::map_h  = map_h;
	World::tile_w = tile_w;
	World::tile_h = tile_h;

	uint i;
	uint size = map_w * map_h;

	World::mapdata.clear();
	World::mapdata.resize( size );

	for ( i=0; i < size; i++ )
	{
		World::mapdata[i] = (int)DATA.file.data[ file ].buffer[i];
	}

	World::data_ready = true;
}

void World::draw_dbg( sf::RenderTexture &scr_d )
{
	if ( ! World::dbg_ready ) return;
}

void World::draw_bg( sf::RenderTexture &scr_n, sf::RenderTexture &scr_d )
{
	if ( ! World::bg_ready ) return;

	sf::Texture  tmptex;
	tmptex.setSmooth(false);
	tmptex.loadFromImage( World::bgimage, sf::IntRect( DATA.camera.pos_x, DATA.camera.pos_y, WIDTH, HEIGHT) );

	sf::Sprite  tmpspr;
	tmpspr.setTexture( tmptex );
	tmpspr.setPosition( 0,0 );
	scr_n.draw( tmpspr );

	World::draw_dbg( scr_d );
}

void World::draw_fg( sf::RenderTexture &scr_n )
{
	if ( ! World::fg_ready ) return;

	sf::Texture tmptex;
	tmptex.setSmooth(false);
	tmptex.loadFromImage( World::fgimage, sf::IntRect( DATA.camera.pos_x, DATA.camera.pos_y, WIDTH, HEIGHT) );

	sf::Sprite  tmpspr;
	tmpspr.setTexture( tmptex );
	tmpspr.setPosition( 0,0 );
	scr_n.draw( tmpspr );
}

void World::clear()
{
	World::ready           = false;
	World::mapobj_ready    = false;
	World::mapfile_ready   = false;
	World::tileset_ready   = false;
	World::tiledebug_ready = false;
	World::map_id = -1;
	World::map_x  = 0;
	World::map_y  = 0;
	World::tile_w = 0;
	World::tile_h = 0;

	World::mapfile.clear();
	World::mapobj.clear();
	World::tileset.clear();
	World::tileset_d.clear();
	World::tiletype.clear();
	World::map_n.clear();
	World::map_d.clear();
}

void World::set_tileset( const std::string &tile, uint col, uint row, uint tile_w, uint tile_h )
{
	uint x, y;
	printf(">> World::set_tileset( %s, %i, %i, %i, %i )\n", tile.c_str(), col, row, tile_w, tile_h );
	World::tile_w = tile_w;
	World::tile_h = tile_h;
	DATA.sfimage = DATA.file.data[ tile ].get_sfimage();

	// normal tile
	World::tileset.clear();
	World::tileset.reserve( col * row );
	for ( y=0; y < row; y++ )
	{
		for ( x=0; x < col; x++ )
		{
			sf::Texture tmptex;
			tmptex.setSmooth(false);
			tmptex.loadFromImage( DATA.sfimage, sf::IntRect( x*tile_w, y*tile_h, tile_w, tile_h) );
			World::tileset.push_back(tmptex);
		}
	}

	printf(">> World::tileset.size() = %i\n", World::tileset.size() );
	World::tileset_ready = true;
}

void World::set_tiledebug( const std::string &tile, uint col, uint row )
{
	if ( World::tileset_ready )
	{
		uint x, y;
		printf(">> World::set_tiledebug( %s, %i, %i )\n", tile.c_str(), col, row );

		World::tileset_d.clear();
		World::tileset_d.reserve( col * row );
		DATA.sfimage = DATA.file.data[ tile ].get_sfimage();

		for ( y=0; y < row; y++ )
		{
			for ( x=0; x < col; x++ )
			{
				sf::Texture tmptex;
				tmptex.setSmooth(false);
				tmptex.loadFromImage( DATA.sfimage, sf::IntRect( x * World::tile_w, y * World::tile_h, World::tile_w, World::tile_h) );
				World::tileset_d.push_back(tmptex);
			}
		}

		printf(">> World::tileset_d.size() = %i\n", World::tileset_d.size() );
		World::tiledebug_ready = true;
	}
}

void World::set_mapfile( const std::string &map, uint map_x, uint map_y )
{
	printf(">> World::set_mapfile( %s, %i, %i )\n", map.c_str(), map_x, map_y );
	World::map_x = map_x;
	World::map_y = map_y;

	World::tiletype.clear();
	World::mapfile.clear();
	World::mapfile.reserve( map_x * map_y );

	uint x, y, c;
	uint d = 0;
	for ( y=0; y < map_y; y++ )
	{
		for ( x=0; x < map_x; x++ )
		{
			c = (int) DATA.file.data[ map ].buffer[ d ];
			World::mapfile.push_back( c );
			World::tiletype[ c ] = 0;
			d++;
		}
	}

	printf(">> World::mapfile.size() = %i\n", World::mapfile.size() );
	printf(">> World::tiletype.size() = %i\n", World::tiletype.size() );
	World::mapfile_ready = true;
}

void World::set_tiletype( uint key, uint value )
{
	// not found
	if ( World::tiletype.find(key) == World::tiletype.end() )
	{
		printf(">> Failed to set_tiletype(), key [%i] not found\n", key);
		return;
	}
	//printf(">> set_tiletype( %i, %i )\n", key, value);
	World::tiletype[ key ] = value;
}

void World::print_tiletype()
{
	printf(">> World::print_tiletype()\n");
	if ( World::tiletype.empty() )
		printf(">> World::tiletype = empty\n" );
	else
	{
		std::map < uint, uint >::const_iterator it;

		printf(">> World::tiletype.size() = %i\n", World::tiletype.size() );
		for ( it = World::tiletype.begin(); it != World::tiletype.end(); ++it )
			printf("%i->%i ", it->first, it->second);
		printf("\n");
	}
}

uint World::get_tileint(uint x, uint y)
{
	if ( !World::mapfile_ready ) return 0; // the required var aren't ready
	if ( x >= World::map_x )     return 0; // out of bound
	if ( y >= World::map_y )     return 0;

	// map_x=16, map_y=16
	// (0,0) = 0, (1,0) = 1, (0,1) = 16, (1,1) = 17
	uint c = ( y * World::map_x) + x;
	return c;
}

void World::set_mapobj()
{
	if ( World::mapfile_ready && World::tileset_ready )
	{
		printf(">> World::render_map()\n");
		World::mapobj.clear();
		World::mapobj.reserve( World::map_x * World::map_y );

		// adding TileData objects
		uint x, y, c, t;
		for ( y=0; y < World::map_y; y++ )
		{
			for ( x=0; x < World::map_x; x++ )
			{
				c = World::get_tileint(x, y);
				t = World::tiletype[ World::mapfile[c] ];
				World::mapobj.push_back( TileData(World::mapfile[c], t) );
				//printf("x %i y %i TileData(%i, %i)\n", x, y, World::mapfile[c], t);
			}
		}

		World::map_n.create( WIDTH + World::tile_w, HEIGHT + World::tile_h );
		World::map_d.create( WIDTH + World::tile_w, HEIGHT + World::tile_h );

		printf(">> World::mapobj.size() = %i\n", World::mapobj.size() );
		World::mapobj_ready = true;
	}
}

void World::draw_map( sf::RenderTexture &pre_n, sf::RenderTexture &pre_d )
{
	if ( World::mapobj_ready )
	{
		sf::Sprite SPR;
		uint tv, tw;
		int  tx, ty, cid;

		// use the top-left tile offset as map id
		// and redraw the pre-render map only when necessary
		tv = DATA.camera.pos_x / World::tile_w;
		tw = DATA.camera.pos_y / World::tile_h;
		cid = World::get_tileint( tv, tw );
		if ( World::map_id != cid )
			World::redraw_map( cid );

		// draw the pre-render map
		tx = ( DATA.camera.pos_x % World::tile_w ) * -1;
		ty = ( DATA.camera.pos_y % World::tile_h ) * -1;
		//printf("camera x %i y %i tx %i ty %i\n", DATA.camera.pos_x, DATA.camera.pos_y, tx, ty);

		// render normal tiles
		SPR.setTexture( World::map_n.getTexture() );
		SPR.setPosition( tx, ty );
		pre_n.draw( SPR );

		// render debug tiles
		if ( World::tiledebug_ready )
		{
			SPR.setTexture( World::map_d.getTexture() );
			SPR.setPosition( tx, ty );
			pre_d.draw( SPR );
		}
	}
}

void World::redraw_map( int cid )
{
	//printf(">> World::redraw_map( %i )\n", cid);
	uint tx, ty, x, y, c;
	int  cam_x, cam_y;
	sf::Sprite SPR_N, SPR_D, SPR;

	World::map_n.clear( sf::Color(0,0,0,0) );
	World::map_d.clear( sf::Color(0,0,0,0) );

		// say camera is at (40,40)
		// get the tile nummber: (3,3) -> (20,20)
		// draw the result to (-8,-8) on screen

		// ( 256 / 16 ) + 1 = 17
		cam_x = DATA.camera.pos_x / World::tile_w;
		cam_y = DATA.camera.pos_y / World::tile_h;
		tx = ( WIDTH  / World::tile_w ) + 1;
		ty = ( HEIGHT / World::tile_h ) + 1;
		//printf("cam_x %i cam_y %i\n", cam_x, cam_y);
		//printf("tx %i ty %i\n", tx, ty);

		// draw grid lines onto the debug map
		if ( World::tiledebug_ready )
		{
			World::tileset_d[0].setRepeated(true);
			SPR.setTexture( World::tileset_d[0] );
			SPR.setTextureRect( sf::IntRect(0, 0, WIDTH + World::tile_w, HEIGHT + World::tile_h) );
			World::map_d.draw( SPR );
		}

		// render each tile on their map
		for ( y=0; y < ty; y++ )
		{
			// out of bound check
			if ( (cam_y+y) >= World::map_y )
				continue;

			for ( x=0; x < tx; x++ )
			{
				// out of bound check
				if ( (cam_x+x) >= World::map_x )
					continue;

				c = World::get_tileint( cam_x + x, cam_y + y );
				//printf(">> World::get_tileint( %i, %i )\n", cam_x + x, cam_y + y);
				//printf("c %i mapobj[c].tile %i mapobj[c].type %i\n", c, World::mapobj[c].tile, World::mapobj[c].type);

				// render normal tiles
				if ( World::mapobj[c].tile > 0 )
				{
					SPR_N.setTexture( World::tileset[ World::mapobj[c].tile ] );
					SPR_N.setPosition( x * World::tile_w, y * World::tile_h );
					World::map_n.draw( SPR_N );
				}

				// render debug tiles
				if ( World::tiledebug_ready )
				{
					if ( World::mapobj[c].type > 0 )
					{
						SPR_D.setTexture( World::tileset_d[ World::mapobj[c].type ] );
						SPR_D.setPosition( x * World::tile_w, y * World::tile_h );
						World::map_d.draw( SPR_D );
					}
				}
			}
		}

	World::map_n.display();
	World::map_d.display();
	World::map_id = cid;
}

