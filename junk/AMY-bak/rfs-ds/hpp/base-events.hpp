namespace amy
{
	class Events
	{
		public:
			Events();
			~Events();

			void inputs();
			virtual void mk_objs( int type, int pos_x, int pos_y, bool is_left=false, int hp=0 );

		private:
			bool is_dialog;
			int checkpoint;

	};
}
