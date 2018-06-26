namespace amy
{
	class KeyData
	{
		public:
			KeyData();
			~KeyData();
			enum keys
			{
				KU = 1 << 0,  KD = 1 << 1,  KL = 1 << 2,  KR = 1 << 3,
				Ka = 1 << 4,  Kb = 1 << 5,  Kc = 1 << 6,  Kd = 1 << 7,
				Ke = 1 << 8,  Kf = 1 << 9,
				Kg = 1 << 10, Kh = 1 << 11,
			};

			bool U; // arrow UP
			bool D; // arrow DOWN
			bool L; // arrow LEFT
			bool R; // arrow RIGHT

			bool a; // jump
			bool b; // shot, charge
			bool c; // shot, rapid
			bool d; // dash
			bool e; // weapon switch L
			bool f; // weapon switch R

			bool g; // start, run
			bool h; // select

			void reset();
			uint save();
			void load( int );
			void merge ( int );
			void xmerge( int );
	};
}

