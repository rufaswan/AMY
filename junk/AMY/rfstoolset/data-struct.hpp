#ifndef RFS_STRUCT
#define RFS_STRUCT

#include "defs.hpp"

namespace rfs
{
	class Pos
	{
		public:
			Pos();
			Pos(int x, int y);
			~Pos();
			void set(int x=0, int y=0);
			int x;
			int y;
	};

	class Size
	{
		public:
			Size();
			Size(uint w, uint h);
			~Size();
			void set  (uint w=0, uint h=0);
			void set_w(uint w);
			void set_h(uint h);
			void swap();
			uint w;
			uint h;
			uint s;
	};

	class Zoom
	{
		public:
			Zoom();
			Zoom(uint x, uint y);
			~Zoom();
			void set  (uint x=100, uint y=100);
			void set_x(uint x);
			void set_y(uint y);
			void swap();
			uint x;
			uint y;
			uint z;
	};

	class Distance
	{
		public:
			Distance();
			Distance(int a, int b);
			~Distance();
			void set  (int a=0, int b=0);
			void set_a(int a);
			void set_b(int b);
			void swap();
			int a;
			int b;
			int d1; // relative distance, can be negative value
			int d2; // absolute distance, always positive
	};

}

#endif