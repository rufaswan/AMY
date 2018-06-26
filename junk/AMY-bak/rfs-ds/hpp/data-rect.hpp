namespace amy
{
	class RectData
	{
		public:
			RectData();
			~RectData();
			int  top;
			int  bottom;
			int  left;
			int  right;
			uint width;
			uint height;

			void setsize(uint w, uint h);
			void tl_setsize(int x, int y, uint w, uint h);
			void tr_setsize(int x, int y, uint w, uint h);
			void bc_setsize(int x, int y, uint w, uint h);
			void cc_setsize(int x, int y, uint w, uint h);

			bool within( int x, int y );
			bool within( amy::RectData& );
			bool contact( amy::RectData& );
	};
}

