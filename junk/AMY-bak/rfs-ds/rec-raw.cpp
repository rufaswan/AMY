#include "header.hpp"
using namespace amy;
extern PlayData DATA;

/*************************************************
 *
 * Save data in loseless RAW video and RAW audio format
 *
 * WARNING : LARGE FILE OUTPUT!!!
 *
 * RAW Video = RGB888, YUV420, YUV422, YUV444
 * RAW Audio = PCM, WAV
 *
 */

RawRecorder::RawRecorder()
{
}
RawRecorder::~RawRecorder() {}

void RawRecorder::init()
{
	printf(">> RawRecorder::init()\n");
	Recorder::mov_file = Recorder::get_filename( "yuv" );
	Recorder::wav_file = Recorder::get_filename( "pcm" );

	DATA.util.fopen( Recorder::mov_fs, Recorder::mov_file, "wb+" );
	if ( !Recorder::mov_fs.good() )
		return;

	// write file header
	std::stringstream ss;
	std::string       str;

	ss.str("");

	// from http://wiki.multimedia.cx/index.php?title=YUV4MPEG2
	//
	//'F30:1' = 30 FPS
	//'F25:1' = 25 FPS (PAL/SECAM standard)
	//'F24:1' = 24 FPS (Film)
	//'F30000:1001' = 29.97 FPS (NTSC standard)
	//'F24000:1001' = 23.976 FPS (Film transferred to NTSC)
	//
	//'Ip' = Progressive
	//'It' = Top field first
	//'Ib' = Bottom field first
	//'Im' = Mixed modes (detailed in FRAME headers)
	//
	//'A0:0' = unknown
	//'A1:1' = square pixels
	//'A4:3' = NTSC-SVCD (480x480 stretched to 4:3 screen)
	//'A4:5' = NTSC-DVD narrow-screen (720x480 compressed to a 4:3 display)
	//'A32:27' = NTSC-DVD wide-screen (720x480 stretched to a 16:9 display)
	//
	//'C420jpeg' = 4:2:0 with biaxially-displaced chroma planes
	//'C420paldv' = 4:2:0 with vertically-displaced chroma planes
	//[x]'C420' = 4:2:0 with coincident chroma planes
	//'C422' = 4:2:2
	//'C444' = 4:4:4
	ss << "YUV4MPEG2"
		<< " W" << Recorder::width
		<< " H" << Recorder::height
		<< " F" << FPS << ":1"
		<< " Ip"
		<< " A0:0"
		<< " C420jpeg";
	str = ss.str();

	printf("Recorder::mov_fs.write( %s, %i )\n", str.c_str(), str.length() );
	Recorder::mov_fs.write( str.c_str(), str.length() );
	Recorder::mov_fs.put( 0xa );

	Recorder::mov_fs.flush(); // write data from fstream to the actual file!
}

void RawRecorder::unset()
{
	printf(">> RawRecorder::unset()\n");
}

void RawRecorder::record_video( sf::Image& img, int last )
{
	// write frame header
	Recorder::mov_fs.write( "FRAME", 5 );
	Recorder::mov_fs.put( 0xa );

	Recorder::rgba2yuv420( img );
	RawRecorder::write_yuv420();

	Recorder::mov_fs.flush(); // write data from fstream to the actual file!
}

void RawRecorder::record_audio()
{
}

void RawRecorder::write_yuv444()
{
	// YUV444p planar format
	// write all the y first, followed by all the u, then all the v
	// y = w*h, u = w*h, y = w*h
	uint i, size;
	size = Recorder::width * Recorder::height;

	for ( i=0; i<size; i++ )
		Recorder::mov_fs.put( (char)Recorder::yuv_y[i] );
	for ( i=0; i<size; i++ )
		Recorder::mov_fs.put( (char)Recorder::yuv_u[i] );
	for ( i=0; i<size; i++ )
		Recorder::mov_fs.put( (char)Recorder::yuv_v[i] );
}

void RawRecorder::write_yuv422()
{
	// YUV422p planar format
	// write all the y first, followed by all the u, then all the v
	// y = w*h, u = w/2*h, y = w/2*h
	uint i, size, usize;
	size = Recorder::width * Recorder::height;
	usize = Recorder::width / 2 * Recorder::height;

	for ( i=0; i < size; i++ )
		Recorder::mov_fs.put( (char)Recorder::yuv_y[i] );
	for ( i=0; i < usize; i++ )
		Recorder::mov_fs.put( (char)Recorder::yuv_u[i] );
	for ( i=0; i < usize; i++ )
		Recorder::mov_fs.put( (char)Recorder::yuv_v[i] );
}

void RawRecorder::write_yuv420()
{
	// from http://en.wikipedia.org/wiki/YUV
	// YUV420p is a planar format
	// write all the y first, followed by all the u, then all the v
	// YV12 is essential the same as YUV420p, with U and V switched
	// y = w*h, u = w/2*h/2, y = w/2*h/2
	uint i, size, usize;
	size = Recorder::width * Recorder::height;
	usize = Recorder::width / 2 * Recorder::height / 2;

	for ( i=0; i < size; i++ )
		Recorder::mov_fs.put( (char)Recorder::yuv_y[i] );
	for ( i=0; i < usize; i++ )
		Recorder::mov_fs.put( (char)Recorder::yuv_u[i] );
	for ( i=0; i < usize; i++ )
		Recorder::mov_fs.put( (char)Recorder::yuv_v[i] );
}
