#include "data-frame.hpp"

namespace rfs
{
	Frame::Frame()
	{
		Frame::size   = 0;
		Frame::now    = 1;
		Frame::repeat = true;
	}
	Frame::~Frame() {}
	void Frame::setframes( bool repeat, uint no, ... )
	{
		Frame::now    = no;
		Frame::size   = no;
		Frame::repeat = repeat;
		Frame::frame.clear();
		Frame::frame.resize(no);

		va_list vl;
		uint i;

		va_start(vl, no);
		for ( i=0; i < no; i++ )
			Frame::frame[i] = va_arg(vl, int);
		va_end(vl);
	}

	int Frame::play()
	{
		if ( Frame::size == 0 )  return 0;

		Frame::now++;
		if ( Frame::now >= Frame::size )
		{
			if ( Frame::repeat )
				Frame::now = 0;
			else
				Frame::now = Frame::size - 1;
		}
		return Frame::frame[ Frame::now ];
	}

	void Frame::stop()
	{
		Frame::now = Frame::size;
	}
}
