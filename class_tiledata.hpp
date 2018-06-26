#ifndef TILEDATA_HPP
#define TILEDATA_HPP

#include <vector>

struct Tile {
	bool is_wall;
	bool is_spike;
	bool is_pit;
}; // struct Tile
typedef  std::vector < Tile >  Tiles;

class TileData {
	private:
		std::string  m_class_name;

		void tile_id( int id )
		{
			Tile t;
				t.is_wall    = false;
				t.is_spike   = false;
				t.is_pit     = false;
			while ( m_tile.size() < (id + 1) )
				m_tile.push_back(t);
		}

	protected:
	public:
		Tiles   m_tile;

		void set_pit( int id )
		{
			tile_id( id );
			m_tile[id].is_pit = true;
		}

		void set_spike( int id )
		{
			tile_id( id );
			m_tile[id].is_wall  = true;
			m_tile[id].is_spike = true;
		}

		void set_wall( int id )
		{
			tile_id( id );
			m_tile[id].is_wall = true;
		}

		TileData()
		{
			m_class_name = "TileData";
			m_tile.reserve(255);
		}
}; // class TileData

#endif // TILEDATA_HPP
