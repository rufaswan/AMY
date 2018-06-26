#include "header.hpp"
using namespace amy;
extern PlayData DATA;

Recorder::Recorder()
{
	Recorder::started  = false;
	Recorder::stopped  = true;
	Recorder::done     = true;
	Recorder::mov_no   = 0;
	Recorder::width    = 0;
	Recorder::height   = 0;
	Recorder::mov_name = "amy";
}
Recorder::~Recorder() {}
void Recorder::init() {}
void Recorder::unset() {}
void Recorder::record_video( sf::Image&, int ) {}
void Recorder::record_audio() {}

void Recorder::trigger( sf::RenderTexture &screen )
{
	if ( Recorder::started )
		Recorder::stop( screen );
	else
		Recorder::start( screen );
}

void Recorder::start( sf::RenderTexture &screen )
{
	// skip if already started
	if ( Recorder::started )  return;
	if ( !Recorder::done   )  return;
	printf(">> Recorder::start()\n");

	sf::Image img = screen.getTexture().copyToImage();
	Recorder::width  = img.getSize().x;
	Recorder::height = img.getSize().y;

	// custom start from child class
	this->init();

	// for start, set all the flag after everything is done
	Recorder::enc_no  = 0;
	Recorder::started = true;
	Recorder::stopped = false;
	Recorder::done    = false;
}

void Recorder::stop( sf::RenderTexture &screen )
{
	// skip if already stopped
	if ( !Recorder::started ) return;
	if ( Recorder::stopped )  return;
	printf(">> Recorder::stop()\n");

	// for stop, set all the flag so followup can be scheduled
	Recorder::started = false;
	Recorder::stopped = true;
	Recorder::done    = false;
}

void Recorder::run()
{
	// nothing to do
	if ( Recorder::vbuf.empty() ) return;
	uint vs = Recorder::vbuf.size();

	// vbuf more than 1, not the last
	if ( vs > 1 )
	{
		//printf("vbuf : %i frames left...\n", vs );
		DATA.sfmutex.lock();
			sf::Image img = Recorder::vbuf.front().copyToImage();
		DATA.sfmutex.unlock();

		this->record_video( img, 0 );
		Recorder::rmframe();
	}

	// save the very last frame until stopped flag
	else
	{
		if ( Recorder::stopped )
		{
			printf("Saved %i frames\n", Recorder::enc_no + 1 );
			DATA.sfmutex.lock();
				sf::Image img = Recorder::vbuf.front().copyToImage();
			DATA.sfmutex.unlock();

			this->record_video( img, 1 );
			this->unset();
			Recorder::mov_fs.close();
			Recorder::done = true;
			Recorder::rmframe();
		}
	}
}

void Recorder::addframe( sf::RenderTexture &screen )
{
	// not started, do nothing
	if ( Recorder::started )
	{
		//printf("vbuf added...\n");
		DATA.sfmutex.lock();
			Recorder::vbuf.push( screen.getTexture() );
		DATA.sfmutex.unlock();
	}
}

void Recorder::rmframe()
{
	//printf("vbuf remove...\n");
	DATA.sfmutex.lock();
		Recorder::vbuf.pop();
	DATA.sfmutex.unlock();

Recorder::enc_no++;
}

void Recorder::rgba2yuv444( sf::Image &img )
{
	// 1 byte = 8-bit
	// RGBA or sf::Color = 4 byte per pixel
	// YUV444  = 3 bytes per pixel
	Recorder::yuv_y.clear();
	Recorder::yuv_u.clear();
	Recorder::yuv_v.clear();
	Recorder::yuv_y.resize( Recorder::width * Recorder::height );
	Recorder::yuv_u.resize( Recorder::width * Recorder::height );
	Recorder::yuv_v.resize( Recorder::width * Recorder::height );

	sf::Color pix;
	uint px, py, pos;
	for ( py=0; py < Recorder::height; py++ )
	{
		for ( px=0; px < Recorder::width; px++ )
		{
			pos = px + ( py * Recorder::width );
			pix = img.getPixel( px, py );

			Recorder::yuv_y[ pos ] = Recorder::clamp(
				( 65481 * pix.r + 128553 * pix.g + 24966 * pix.b +  4207500) / 255000
			);
			Recorder::yuv_u[ pos ] = Recorder::clamp(
				(-33488 * pix.r -  65744 * pix.g + 99232 * pix.b + 29032005) / 225930
			);
			Recorder::yuv_v[ pos ] = Recorder::clamp(
				(157024 * pix.r - 131488 * pix.g - 25536 * pix.b + 45940035) / 357510
			);
		}
	}
}

void Recorder::rgba2yuv422( sf::Image &img )
{
	// YUV422  = 4 bytes per 2 pixels
	Recorder::yuv_y.clear();
	Recorder::yuv_u.clear();
	Recorder::yuv_v.clear();
	Recorder::yuv_y.resize( Recorder::width * Recorder::height );
	Recorder::yuv_u.resize( Recorder::width / 2 * Recorder::height );
	Recorder::yuv_v.resize( Recorder::width / 2 * Recorder::height );

	sf::Color pix0, pix1;
	uint px, py, pos;
	for ( py=0; py < Recorder::height; py++ )
	{
		// skipping a column
		for ( px=0; px < Recorder::width; px += 2 )
		{
			pix0 = img.getPixel( px+0, py );
			pix1 = img.getPixel( px+1, py );

			// y didn't skip pixel
			pos = px + ( py * Recorder::width );

			yuv_y[pos + 0] = Recorder::clamp(
				(65481 * pix0.r + 128553 * pix0.g + 24966 * pix0.b + 4207500) / 255000
			);
			yuv_y[pos + 1] = Recorder::clamp(
				(65481 * pix1.r + 128553 * pix1.g + 24966 * pix1.b + 4207500) / 255000
			);

			// u & v skipped pixel
			// ( px >> 1 ) is same as px = px / 2;
			pos = (px >> 1) + ( py * (Recorder::width / 2) );

			yuv_u[pos] = Recorder::clamp(
				(
					(-33488 * pix0.r - 65744 * pix0.g + 99232 * pix0.b + 29032005) / 2 +
					(-33488 * pix1.r - 65744 * pix1.g + 99232 * pix1.b + 29032005) / 2
				) / 225930
			);
			yuv_v[pos] = Recorder::clamp(
				(
					(157024 * pix0.r - 131488 * pix0.g - 25536 * pix0.b + 45940035) / 2 +
					(157024 * pix1.r - 131488 * pix1.g - 25536 * pix1.b + 45940035) / 2
				) / 357510
			);
		}
	}
}

void Recorder::rgba2yuv420( sf::Image &img )
{
	// YUV420p = 6 bytes per 4 pixels, reordered
	Recorder::yuv_y.clear();
	Recorder::yuv_u.clear();
	Recorder::yuv_v.clear();
	Recorder::yuv_y.resize( Recorder::width * Recorder::height );
	Recorder::yuv_u.resize( Recorder::width / 2 * Recorder::height / 2 );
	Recorder::yuv_v.resize( Recorder::width / 2 * Recorder::height / 2 );

	sf::Color pix0, pix1, pix2, pix3;
	uint px, py, pos, pos0, pos1;
	// skipping a row
	for ( py=0; py < Recorder::height; py += 2 )
	{
		// skipping a column
		for ( px=0; px < Recorder::width; px += 2 )
		{
			pix0 = img.getPixel( px+0, py+0 );
			pix1 = img.getPixel( px+1, py+0 );
			pix2 = img.getPixel( px+0, py+1 );
			pix3 = img.getPixel( px+1, py+1 );

			// y didn't skip pixel
			pos0 = px + ( (py + 0) * Recorder::width );
			pos1 = px + ( (py + 1) * Recorder::width );

			yuv_y[pos0 + 0] = Recorder::clamp((65481 * pix0.r + 128553 * pix0.g + 24966 * pix0.b + 4207500) / 255000);
			yuv_y[pos0 + 1] = Recorder::clamp((65481 * pix1.r + 128553 * pix1.g + 24966 * pix1.b + 4207500) / 255000);
			yuv_y[pos1 + 0] = Recorder::clamp((65481 * pix2.r + 128553 * pix2.g + 24966 * pix2.b + 4207500) / 255000);
			yuv_y[pos1 + 1] = Recorder::clamp((65481 * pix3.r + 128553 * pix3.g + 24966 * pix3.b + 4207500) / 255000);

			// u & v skipped pixel
			// ( px >> 1 ) is same as px = px / 2;
			pos = (px >> 1) + ( (py >> 1) * (Recorder::width / 2) );

			yuv_u[pos] = Recorder::clamp(
				(
					(-33488 * pix0.r - 65744 * pix0.g + 99232 * pix0.b + 29032005) / 4 +
					(-33488 * pix0.r - 65744 * pix0.g + 99232 * pix0.b + 29032005) / 4 +
					(-33488 * pix2.r - 65744 * pix2.g + 99232 * pix2.b + 29032005) / 4 +
					(-33488 * pix3.r - 65744 * pix3.g + 99232 * pix3.b + 29032005) / 4
				) / 225930
			);
			yuv_v[pos] = Recorder::clamp(
				(
					(157024 * pix0.r - 131488 * pix0.g - 25536 * pix0.b + 45940035) / 4 +
					(157024 * pix1.r - 131488 * pix1.g - 25536 * pix1.b + 45940035) / 4 +
					(157024 * pix2.r - 131488 * pix2.g - 25536 * pix2.b + 45940035) / 4 +
					(157024 * pix3.r - 131488 * pix3.g - 25536 * pix3.b + 45940035) / 4
				) / 357510
			);
		}
	}
}

std::string Recorder::get_filename( const std::string &ext )
{
	std::string str;
	std::fstream fs;

	bool done = false;
	while ( !done )
	{
		str = Recorder::mov_name + "-"
			+ DATA.file.int2str( Recorder::mov_no, 4 ) + "."
			+ ext;

		DATA.file.fopen( fs, str, "rb" );
		// video exists, skipped
		if ( fs.good() )
		{
			Recorder::mov_no++;
			fs.close();
		}

		// video not found, use it
		else
			done = true;
	}
	return str;
}

uchar Recorder::clamp(int d)
{
	if ( d < 0   ) return 0;
	if ( d > 255 ) return 255;
	return d;
}

void Recorder::binary_write(uchar *uc, uint len)
{
	uint i;
	for ( i=0; i<len; i++ )
		Recorder::mov_fs.put( (char)uc[i] );
}
