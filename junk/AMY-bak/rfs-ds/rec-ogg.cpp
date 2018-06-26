#include "header.hpp"
#include <theora/theoraenc.h>
#include <vorbis/vorbisenc.h>
#include <ogg/ogg.h>

using namespace amy;
extern PlayData DATA;

/*************************************************
 *
 * Save data in OGG file type (lossy)
 *
 * Video = Theora
 * Audio = Vorbis
 *
 */

OggRecorder::OggRecorder()
{
	OggRecorder::video_q       = 90;
	OggRecorder::colorspace    = TH_CS_ITU_REC_470M;
		// TH_CS_UNSPECIFIED
		// TH_CS_ITU_REC_470M  (NTSC)
		// TH_CS_ITU_REC_470BG (PAL)
	OggRecorder::chroma_format = TH_PF_420;
		// TH_PF_420
		// TH_PF_422
		// TH_PF_444

	OggRecorder::audio_q  = -1;
}
OggRecorder::~OggRecorder() {}

void OggRecorder::init()
{
	printf(">> OggRecorder::init()\n");
	Recorder::mov_file = Recorder::get_filename( "ogv" );
	Recorder::wav_file = Recorder::get_filename( "ogg" );

	DATA.util.fopen( Recorder::mov_fs, Recorder::mov_file, "wb+" );
	if ( !Recorder::mov_fs.good() )
		return;

	std::srand( std::time(NULL) );
	//--------------------------------------------
	// Init Theora
	//
	int video_quality = ( OggRecorder::video_q * 63 ) / 100;

	th_info_init( &(OggRecorder::ti) );
	OggRecorder::ti.frame_width        = Recorder::width;
	OggRecorder::ti.frame_height       = Recorder::height;
	OggRecorder::ti.pic_width          = Recorder::width;
	OggRecorder::ti.pic_height         = Recorder::height;
	OggRecorder::ti.pic_x              = 0;
	OggRecorder::ti.pic_y              = 0;
	OggRecorder::ti.colorspace         = OggRecorder::colorspace;
	OggRecorder::ti.pixel_fmt          = OggRecorder::chroma_format;
	OggRecorder::ti.target_bitrate     = 0; // non-zero for CBR, zero for VBR
	OggRecorder::ti.quality            = video_quality;
	OggRecorder::ti.fps_numerator      = FPS; // frame rate. if either is zero = undefined
	OggRecorder::ti.fps_denominator    = 1;
	OggRecorder::ti.aspect_numerator   = 0; // aspect ratio. if either is zero = undefined
	OggRecorder::ti.aspect_denominator = 0;

	OggRecorder::td = th_encode_alloc( &(OggRecorder::ti) );
	th_info_clear( &(OggRecorder::ti) );

	th_comment_init( &(OggRecorder::tc) );

	ogg_stream_init( &(OggRecorder::to), std::rand() );
	//--------------------------------------------
	// Init Vorbis
	//
/*
	vorbis_info_init      ( &(OggRecorder::vi) );
	if ( vorbis_encode_init_vbr( &(OggRecorder::vi), 2, 44100, OggRecorder::audio_q) )
		return;
	vorbis_comment_init   ( &(OggRecorder::vc) );
	vorbis_analysis_init  ( &(OggRecorder::vd), &(OggRecorder::vi) );
	vorbis_block_init     ( &(OggRecorder::vd), &(OggRecorder::vb) );

	ogg_stream_init( &(OggRecorder::vo), std::rand() );
*/

	//--------------------------------------------
	// Writing first frame or file header
	//

	// first packet will get its own page automatically
	while ( th_encode_flushheader( OggRecorder::td, &(OggRecorder::tc), &(OggRecorder::op)) != 0 )
		ogg_stream_packetin( &(OggRecorder::to), &(OggRecorder::op) );

	if ( ogg_stream_pageout( &(OggRecorder::to), &(OggRecorder::og)) != 1 )
		return;
	Recorder::binary_write( OggRecorder::og.header, OggRecorder::og.header_len );
	Recorder::binary_write( OggRecorder::og.body,   OggRecorder::og.body_len );

	// create the remaining theora headers
	while ( th_encode_flushheader( OggRecorder::td, &(OggRecorder::tc), &(OggRecorder::op)) != 0 )
		ogg_stream_packetin( &(OggRecorder::to), &(OggRecorder::op));

/*
	// ---audio---
		ogg_packet header;
		ogg_packet header_comm;
		ogg_packet header_code;
		vorbis_analysis_headerout( &(OggRecorder::vd), &(OggRecorder::vc), &header, &header_comm, &header_code);
		ogg_stream_packetin( &(OggRecorder::vo), &header);

		if ( ogg_stream_pageout( &(OggRecorder::vo), &(OggRecorder::og) ) != 1 )
			return;
		Recorder::mov_fs.write( OggRecorder::og.header, OggRecorder::og.header_len );
		Recorder::mov_fs.write( OggRecorder::og.body,   OggRecorder::og.body_len );

		// remaining vorbis header packets
		ogg_stream_packetin( &(OggRecorder::vo), &header_comm);
		ogg_stream_packetin( &(OggRecorder::vo), &header_code);
	// ---audio---
*/

	// Flush the rest of our headers.
	while ( ogg_stream_flush( &(OggRecorder::to), &(OggRecorder::og) ) != 0 )
	{
		Recorder::binary_write( OggRecorder::og.header, OggRecorder::og.header_len );
		Recorder::binary_write( OggRecorder::og.body,   OggRecorder::og.body_len );
	}
/*
		// ---audio---
		while ( ogg_stream_flush( &(OggRecorder::vo), &(OggRecorder::og) ) != 0 )
		{
			Recorder::binary_write( OggRecorder::og.header, OggRecorder::og.header_len );
			Recorder::binary_write( OggRecorder::og.body,   OggRecorder::og.body_len );
		}
		// ---audio---
*/
	Recorder::mov_fs.flush(); // write data from fstream to the actual file!
}

void OggRecorder::unset()
{
	printf(">> OggRecorder::unset()\n");
/*
	// clean up vorbis
	vorbis_block_clear  ( &(OggRecorder::vb) );
	vorbis_dsp_clear    ( &(OggRecorder::vd) );
	vorbis_comment_clear( &(OggRecorder::vc) );
	vorbis_info_clear   ( &(OggRecorder::vi) );
*/

	// clean up theora
	th_encode_free  ( OggRecorder::td );
	th_comment_clear(&(OggRecorder::tc) );

	// cleanup ogg
	//ogg_stream_clear( &(OggRecorder::vo) );
	ogg_stream_clear( &(OggRecorder::to) );
}

void OggRecorder::record_video( sf::Image& img, int last )
{
	Recorder::rgba2yuv420( img );

	th_ycbcr_buffer ycbcr;

	//444 = w; 422 = w/2; 420 = w/2
	//444 = h; 422 = h;   420 = h/2
	ycbcr[0].width  = Recorder::width;
	ycbcr[0].height = Recorder::height;
	ycbcr[0].stride = Recorder::width;

	ycbcr[1].width  = (Recorder::width >> 1);
	ycbcr[1].height = Recorder::height;
	ycbcr[1].stride = ycbcr[1].width;
		if ( OggRecorder::chroma_format == TH_PF_444 )
			ycbcr[1].width = Recorder::width;
		if ( OggRecorder::chroma_format == TH_PF_420 )
			ycbcr[1].height = (Recorder::height >> 1);

	ycbcr[2].width  = ycbcr[1].width;
	ycbcr[2].height = ycbcr[1].height;
	ycbcr[2].stride = ycbcr[1].stride;

	ycbcr[0].data = &Recorder::yuv_y[0];
	ycbcr[1].data = &Recorder::yuv_u[0];
	ycbcr[2].data = &Recorder::yuv_v[0];

	// Submit a frame for compression and pull out the packet
	if( th_encode_ycbcr_in( OggRecorder::td, ycbcr ) )
		return;

	while ( th_encode_packetout( OggRecorder::td, last, &(OggRecorder::op)) != 0 )
		ogg_stream_packetin( &(OggRecorder::to), &(OggRecorder::op));

	while( ogg_stream_pageout( &(OggRecorder::to), &(OggRecorder::og)) )
	{
		Recorder::binary_write( OggRecorder::og.header, OggRecorder::og.header_len );
		Recorder::binary_write( OggRecorder::og.body,   OggRecorder::og.body_len );
	}

	Recorder::mov_fs.flush(); // write data from fstream to the actual file!
}

void OggRecorder::record_audio()
{
}
