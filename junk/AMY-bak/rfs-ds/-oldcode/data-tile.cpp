#include "header.hpp"
using namespace amy;

TileData::TileData( uint tile, uint type )
{
	TileData::tile    = tile;
	TileData::type    = type;
	TileData::event   = 0;
	TileData::is_wall = false;
}
TileData::~TileData() {}
