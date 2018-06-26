namespace amy
{
	class Rect
	{
		public:
			Rect();
			virtual ~Rect();
			int x;
			int y;
			int width;
			int height;
			int top;
			int bottom;
			int left;
			int right;
			bool collided;
			bool collide_top;
			bool collide_bottom;
			bool collide_left;
			bool collide_right;

			void set_rect(int left, int top, int right, int bottom);
			bool within(int, int);
			bool collision( const amy::Rect& );
		protected:
		private:
	};
}
