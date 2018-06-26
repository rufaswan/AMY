#include "theora/theoraenc.h"
#include "vorbis/vorbisenc.h"

namespace amy
{
	class OggRecorder : public Recorder
	{
		public:
			OggRecorder();
			~OggRecorder();

			void init();
			void unset();
			void record_video( sf::Image&, int last=0 );
			void record_audio();

			// Theora related
			th_enc_ctx*      td;
			th_info          ti;
			th_comment       tc;
			th_colorspace    colorspace;
			th_pixel_fmt     chroma_format;
			int video_q; // q= 0 <-> 100

			// Vorbis related
			vorbis_block     vb;
			vorbis_info      vi;
			vorbis_comment   vc;
			vorbis_dsp_state vd;
			int audio_q;  // q= -1 <-> 10

			// Ogg related
			ogg_page         og;
			ogg_packet       op;
			ogg_stream_state to; // Theora data
			ogg_stream_state vo; // Vorbis data
			ogg_stream_state ogg_os;
	};
}
