namespace amy
{
	class TileData
	{
		public:
			TileData(uint, uint);
			~TileData();
			uint tile;
			uint type;
			uint event;
			bool is_wall;
	};
}

