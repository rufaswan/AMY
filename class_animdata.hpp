#ifndef ANIMDATA_HPP
#define ANIMDATA_HPP

#include "rfstk.hpp"
#include "rfstk_txt.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

struct Frame {
	sf::Texture  tex;
	int x;
	int y;
}; // struct Frame
struct Anim {
	int frame;
	int time;
}; // struct Anim
struct Clsn {
	int x1;
	int y1;
	int x2;
	int y2;
}; // struct Clsn
typedef  std::vector < Frame   >   arrframe;
typedef  std::vector < Anim    >   arramid;
typedef  std::vector < arramid >   arranim;
typedef  std::vector < Clsn    >   arrclsn;

class AnimData {
	private:
		std::string   m_png_prefix;
		std::string   m_class_name;

		void set_clsn( arrclsn &clsn, int id, std::string &val )
		{
			Clsn  clid;
				clid.x1 = clid.y1 = 0;
				clid.x2 = clid.y2 = 1;
			while ( clsn.size() < (id + 1) )
				clsn.push_back(clid);

			std::string  str = "";
			uint n   = 0;
			uint cnt = 0;
			while(1)
			{
				if ( val[n] == ',' )
				{
					switch (cnt)
					{
						case 0:  clid.x1 = strtol(str.c_str(), NULL, 0); break;
						case 1:  clid.y1 = strtol(str.c_str(), NULL, 0); break;
						case 2:  clid.x2 = strtol(str.c_str(), NULL, 0); break;
						case 3:  clid.y2 = strtol(str.c_str(), NULL, 0);
							clsn[id] = clid;
							printf("clsn[%d] = x1 %d y1 %d x2 %d y2 %d\n", id, clid.x1, clid.y1, clid.x2, clid.y2);
							return;
					}
					str.clear();
					cnt++;
				}
				else
					str += val[n];

				n++;
			} // while 1
		}

		void set_amid( int id, std::string &val )
		{
			vecstr  vs;
				rfs::split( val, '-', vs );

			Anim amid;
				amid.frame = strtol( vs[0].c_str(), NULL, 0 );
				amid.time  = strtol( vs[1].c_str(), NULL, 0 );

			m_anim[id].push_back( amid );
		}

		void set_anim( int id, std::string &val )
		{
			arramid  amid;
			while ( m_anim.size() < (id + 1) )
				m_anim.push_back(amid);

			std::string str = "";
			int  n    = 0;
			bool skip = false;
			while ( n < val.size() )
			{
				if ( skip )  n++;

				switch ( val[n] )
				{
					case ';':
					case '#':
						skip = true;
						break;
					case ',':
						set_amid( id, str );
						printf("m_anim[%d][%d] frame %d time %d\n", id, (int)m_anim[id].size() , m_anim[id].back().frame , m_anim[id].back().time );
						str.clear();
						break;
					default:
						str += val[n];
				} // switch val[n]
				n++;
			} // while val[n]
		}

		void set_frame( int id, std::string &val )
		{
			Frame  fr;
				fr.x = fr.y = 0;
			while ( m_frame.size() < (id + 1) )
				m_frame.push_back(fr);

			char fn[64];
				rfs::memset( fn, 64 );
				sprintf( fn, "%s_%04d.png", m_png_prefix.c_str(), id );
				m_frame[id].tex.loadFromFile( fn );

			vecstr  vs;
				rfs::split( val, ',', vs );

			int x = strtol( vs[0].c_str(), NULL, 0 );
			int y = strtol( vs[1].c_str(), NULL, 0 );

			m_frame[id].x = x;
			m_frame[id].y = y;
			printf("m_frame[%d] = fn %s x %d y %d\n", id, fn, x, y );
		}

		void set_keyval( std::string &key, std::string &val )
		{
			printf("%s.set_keyval( %s , %s )\n", m_class_name.c_str(), key.c_str(), val.c_str() );

			int id;
			if ( key == "total" )
			{
				id = strtol( val.c_str(), NULL, 0 );
				m_frame.reserve( id );
				m_anim.reserve( id );
				m_clsn.reserve( id );
				return;
			}

			vecstr vs;
				rfs::split( key, '.', vs );

			id = strtol( vs[1].c_str(), NULL, 0 );

			if ( vs[0] == "frame" )
				set_frame(id, val);
			else if ( vs[0] == "anim" )
				set_anim(id, val);
			else if ( vs[0] == "clsn" )
				set_clsn(m_clsn, id, val);

		}

		void parse_def( std::string &line )
		{
			printf("%s.parse_def( %s )\n", m_class_name.c_str(), line.c_str() );
			std::string  strkey = "";
			std::string  strval = "";

			bool val  = false;
			int  n    = 0;
			bool skip = false;
			while ( n < line.size() )
			{
				if ( skip )   n++;

				switch ( line[n] )
				{
					case ';':
					case '#':
						skip = true;
						break;
					case ' ':
						break;
					case '=':
						val = true;
						break;
					default:
						( val ) ? strval += line[n] : strkey += line[n];
						break;
				}
				n++;
			} // while line[n]

			if ( strkey[0] == 0 || strval[0] == 0 )
				return;
			set_keyval( strkey, strval );
		}

	protected:
	public:
		bool m_has_def;
		bool m_is_done;
		arrframe m_frame;
		arranim  m_anim;
		arrclsn  m_clsn;
		int  m_seqno;
		int  m_time;

		int get_frame( int state )
		{
			// m_anim[ state ][ Anim ].frame
			int time = m_anim[ state ][ m_seqno ].time;

			m_is_done = false;
			if ( time > 99 )
			{
				m_is_done = true;
				return m_anim[ state ][ m_seqno ].frame;
			}

			if ( m_time > time )
			{
				m_seqno = (m_seqno + 1) % m_anim[ state ].size();
				m_time  = 1;
			}
			else
				m_time++;

			return m_anim[ state ][ m_seqno ].frame;
		}

		int change_state( int state_cur, int state_new )
		{
			if ( state_cur == state_new )
				return state_cur % m_anim.size();
			else
			{
				m_seqno = 0;
				m_time  = 0;
				return state_new % m_anim.size();
			}
		}

		int switch_state( int state_new )
		{
			return state_new % m_anim.size();
		}

		void load_def( std::string &fn )
		{
			printf("%s.load_def( %s )\n", m_class_name.c_str(), fn.c_str() );
			m_has_def = false;
			FILE* fp = fopen( fn.c_str(), "r" );
				if ( ! fp )  return;

			vecstr vs;
				rfs::split( fn, '.', vs );
				m_png_prefix = vs[0];

			uint fsz = rfs::filesize( fp );
			uint p;
			char c;
			std::string  line = "";
			bool skip = false;
			for( p=0; p < fsz; p++ )
			{
				c = fgetc(fp);
				switch( c )
				{
					case ';':
					case '#':
						skip = true;
						break;
					case '\n':
					case '\r':
						if ( line != "" )
						{
							parse_def( line );
							line.clear();
						}
						skip = false;
						break;
					case '\t':
					case ' ':
						break;
					default:
						if ( skip )   continue;
						line += c;
						break;
				}
			} // for p=0

			fclose(fp);

			m_has_def = true;
		}
		void load_def( const char def[] )
		{
			std::string  p = def;
			load_def(p);
		}

		AnimData()
		{
			m_class_name = "AnimData";
			m_has_def = false;
			m_is_done = false;
			m_seqno = 0;
			m_time  = 0;
		}
}; // class AnimData

#endif // ANIMDATA_HPP
