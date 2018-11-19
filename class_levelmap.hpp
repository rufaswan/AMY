#ifndef LEVELMAP_HPP
#define LEVELMAP_HPP

#include "rfstk.hpp"
#include "rfstk_txt.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

struct mapobj {
	int ob;
	int id;
	int x;
	int y;
}; // struct mapobj
struct vertex {
	int id;
	int x;
	int y;
	int w;
	int h;
}; // struct vertex
typedef   std::vector < vertex >   arrver;
typedef   std::vector < uint >     arrmap;
typedef   std::vector < mapobj >   arrobj;

class LevelMap : public sf::Drawable, public sf::Transformable
{
	private:
		std::string  m_class_name;
		std::string  m_rel_path;
		sf::VertexArray   m_vertices;
		arrver  m_ver_col;
		arrver  m_ver_bg;
		arrver  m_ver_obj;
		arrver  m_ver_fg;
		sf::Texture  m_img_col;
		sf::Texture  m_img_bg;
		sf::Texture  m_img_obj;
		sf::Texture  m_img_fg;
		int   m_cur_map;
		std::string  m_fn_def;
		std::string  m_fn_lvl;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			states.transform *= getTransform();
			switch ( m_cur_map )
			{
				case 0:   states.texture = &m_img_col; break;
				case 1:   states.texture = &m_img_bg;  break;
				case 2:   states.texture = &m_img_obj; break;
				case 3:   states.texture = &m_img_fg;  break;
			}
			target.draw(m_vertices, states);
		}

		void put_lvl_sect( FILE* fp, const char title[], arrmap &map )
		{
			printf("%s.put_lvl_sect( %s )\n", m_class_name.c_str(), title);
			rfs::fwrite( fp, title );

			char buf[10];
			uint n = 0;
			while ( n < map.size() )
			{
				rfs::memset( buf, 10 );

				if ( map[n] == 0 )
					sprintf( buf, "%s,", "-" );
				else
					sprintf( buf, "%d,", map[n] );

				rfs::fwrite( fp, buf );

				n++;
				if ( (n % m_map_w) == 0 )
					fputc('\n', fp);
			} // while n
			fputc('\n', fp);
		}

		void set_map( std::string &line, arrmap &map )
		{
			printf("%s.set_map( %d )\n", m_class_name.c_str(), (int)map.size() );
			int c;
			std::string   str = "";

			int n = 0;
			while( n < line.size() )
			{
				switch ( line[n] )
				{
					case '-':
						map.push_back(0);
						break;
					case ',':
						if ( str != "" )
						{
							c = strtol( str.c_str(), NULL, 0 );
							map.push_back(c);
							str.clear();
						}
						break;
					default:
						str += line[n];
						break;
				} // switch line[0]
				n++;
			} // while line[0]

			printf("map.size() = %d\n[ ", (int)map.size() );
			for ( n=0; n < map.size(); n++ )
				printf("%04d ", map[n]);
			printf(" ]\n");
		}

		void parse_lvl( std::string &line, std::string &sect, uint &row )
		{
			printf("%s.parse_lvl( %s, %d )\n", m_class_name.c_str(), sect.c_str(), row);
			if ( line[0] == '[' )
			{
				if ( line == "[col]" )    sect = "col";
				if ( line == "[bg]"  )    sect = "bg";
				if ( line == "[obj]" )    sect = "obj";
				if ( line == "[fg]"  )    sect = "fg";
				row = 0;
				return;
			}
			arrmap  map;
				map.reserve( m_map_w );
				set_map( line, map );

			uint pos = row * m_map_w;
			uint n;
			for ( n=0; n < map.size(); n++ )
			{
				if ( sect == "col" )   m_map_col[ pos + n ] = map[n];
				if ( sect == "bg"  )   m_map_bg[  pos + n ] = map[n];
				if ( sect == "obj" )   m_map_obj[ pos + n ] = map[n];
				if ( sect == "fg"  )   m_map_fg[  pos + n ] = map[n];
			} // for n=0
			row++;
		}

		void set_vertex( vertex &v, std::string &val )
		{
			uint n   = 0;
			uint cnt = 0;
			std::string str = "";
			while(1)
			{
				if ( val[n] == ',' )
				{
					val[n] = 0;

					switch (cnt)
					{
						case 0:  v.x = strtol(str.c_str(), NULL, 0); break;
						case 1:  v.y = strtol(str.c_str(), NULL, 0); break;
						case 2:  v.w = strtol(str.c_str(), NULL, 0); break;
						case 3:  v.h = strtol(str.c_str(), NULL, 0); return;
					} // switch cnt
					str.clear();
					cnt++;
				}
				else
					str += val[n];

				n++;
			} // while 1
		}

		void arrver_id( arrver &arr, int id, vertex &ver )
		{
			vertex v;
				v.id = v.x = v.y = v.w = v.h = 0;
			while ( arr.size() < (id + 1) )
				arr.push_back(v);
			arr[id] = ver;
		}

		void set_texture( sf::Texture &tex, std::string &val )
		{
			std::string  path = "";
			if ( rfs::strpos(val, '/') )
				path = val;
			else
				path = m_rel_path + '/' + val;

			printf("%s.set_texture( %s )\n", m_class_name.c_str(), path.c_str() );
			tex.loadFromFile(path);
		}

		void include_def( std::string &val )
		{
			std::string  path = "";
			if ( rfs::strpos(val, '/') )
				path = val;
			else
				path = m_rel_path + '/' + val;

			printf("%s.include_def( %s )\n", m_class_name.c_str(), path.c_str() );
			load_def(path);
		}

		void set_keyval( std::string &key, std::string &val )
		{
			printf("%s.set_keyval( %s , %s )\n", m_class_name.c_str(), key.c_str(), val.c_str() );
			if ( key == "cam.x" )   { m_cam_x = strtol( val.c_str(), NULL, 0 ); return; }
			if ( key == "cam.y" )   { m_cam_y = strtol( val.c_str(), NULL, 0 ); return; }
			if ( key == "map.w" )   { m_map_w = strtol( val.c_str(), NULL, 0 ); return; }
			if ( key == "map.h" )   { m_map_h = strtol( val.c_str(), NULL, 0 ); return; }

			if ( key == "include" )   { return include_def(val); }
			if ( key == "col.img" )   { return set_texture(m_img_col, val); }
			if ( key == "bg.img"  )   { return set_texture(m_img_bg,  val); }
			if ( key == "obj.img" )   { return set_texture(m_img_obj, val); }
			if ( key == "fg.img"  )   { return set_texture(m_img_fg,  val); }

			vecstr  vs;
				rfs::split( key, '.', vs );

			uint    id;
			vertex  v;
			if ( vs[0] == "bg" )
			{
				id = strtol( vs[1].c_str(), NULL, 0 );
				set_vertex ( v, val );
				arrver_id  ( m_ver_bg, id, v );
			}
			else if ( vs[0] == "fg" )
			{
				id = strtol( vs[1].c_str(), NULL, 0 );
				set_vertex ( v, val );
				arrver_id  ( m_ver_fg, id, v );
			}
			else if ( vs[0] == "col" )
			{
				id = strtol( vs[1].c_str(), NULL, 0 );
				set_vertex ( v, val );
				arrver_id  ( m_ver_col, id, v );
			}
			else if ( vs[0] == "obj" )
			{
				id = strtol( vs[1].c_str(), NULL, 0 );
				set_vertex ( v, val );
				arrver_id  ( m_ver_obj, id, v );
			}
			printf("m_ver_%s[%d] = x %d y %d w %d h %d\n", vs[0].c_str(), id, v.x, v.y, v.w, v.h);
			return;
		}

		void parse_def( std::string &line )
		{
			printf("%s.parse_def( %s )\n", m_class_name.c_str(), line.c_str() );
			std::string  strkey = "";
			std::string  strval = "";

			int  n    = 0;
			bool val  = false;
			bool skip = false;
			while ( n < line.size() )
			{
				if ( skip )  n++;

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
				} // switch line[n]
				n++;
			} // while line[n]

			if ( strkey[0] == 0 || strval[0] == 0 )
				return;
			set_keyval( strkey, strval );
		}
	protected:
	public:
		bool m_has_lvl;
		bool m_has_def;
		int  m_cam_x;
		int  m_cam_y;
		int  m_tile_x;
		int  m_tile_y;
		int  m_map_w;
		int  m_map_h;
		int  m_pad_x;
		int  m_pad_y;
		arrmap  m_map_col;
		arrmap  m_map_bg;
		arrmap  m_map_obj;
		arrmap  m_map_fg;
		arrobj  m_obj_list;

		void save_lvl()
		{
			printf("%s.save_lvl( %s )\n", m_class_name.c_str(), m_fn_lvl.c_str() );

			FILE* fp = fopen( m_fn_lvl.c_str(), "w" );
				if ( ! fp )  return;

			put_lvl_sect( fp, "[col]\n", m_map_col );
			put_lvl_sect( fp, "[bg]\n",  m_map_bg  );
			put_lvl_sect( fp, "[obj]\n", m_map_obj );
			put_lvl_sect( fp, "[fg]\n",  m_map_fg  );

			fclose(fp);
		}

		void set_map_selected( uint id, uint sel )
		{
			vertex v;
			switch ( m_cur_map )
			{
				case 0:
					if ( sel >= m_ver_col.size() )
						return;
					v = m_ver_col[sel];
					printf("COL [%d] = SEL %d x %d y %d w %d h %d\n",
						id, sel, v.x, v.y, v.w, v.h);
					m_map_col[id] = sel;
					break;
				case 1:
					if ( sel >= m_ver_bg.size() )
						return;
					v = m_ver_bg[sel];
					printf("BG [%d] = SEL %d x %d y %d w %d h %d\n",
						id, sel, v.x, v.y, v.w, v.h);
					m_map_bg[id] = sel;
					break;
				case 2:
					if ( sel >= m_ver_obj.size() )
						return;
					v = m_ver_obj[sel];
					printf("OBJ [%d] = SEL %d x %d y %d w %d h %d\n",
						id, sel, v.x, v.y, v.w, v.h);
					m_map_obj[id] = sel;
					break;
				case 3:
					if ( sel >= m_ver_fg.size() )
						return;
					v = m_ver_fg[sel];
					printf("FG [%d] = SEL %d x %d y %d w %d h %d\n",
						id, sel, v.x, v.y, v.w, v.h);
					m_map_fg[id] = sel;
					break;
			} // switch m_cur_map
		}

		void change_map( int id )
		{
			m_cur_map = id % 4;
			//printf("m_cur_map = %d\n", m_cur_map);
		}

		void get_map_obj()
		{
			m_obj_list.clear();

			mapobj  mob;
			int x, y, cx, cy, ob;
			for (y=0; y < 16; y++)
			{
				cy = ( (m_cam_y + y) % m_map_h ) * m_map_w;
				for (x=0; x < 16; x++)
				{
					cx = ( (m_cam_x + x) % m_map_w ) + cy;
					ob = m_map_obj[cx];
					if ( ob > 0 )
					{
						mob.ob = ob;
						mob.id = cx;
						mob.x = (m_cam_x + x) * 16;
						mob.y = (m_cam_y + y) * 16;
						m_obj_list.push_back( mob );
					}
				} // for x=0
			} // for y=0
		}

		int get_cam_id()
		{
			return (m_cam_y * m_map_w) + m_cam_x;
		}

		void cam_move( int mx, int my )
		{
			m_cam_x = rfs::clamp( m_cam_x + mx , 0 , m_map_w - 16 );
			m_cam_y = rfs::clamp( m_cam_y + my , 0 , m_map_h - 16 );
		}

		void cam_lock_x( int pix_x )
		{
			m_cam_x  = (pix_x - (pix_x % 256)) / 16;
			m_tile_x = 0;
		}
		void cam_lock_y( int pix_y )
		{
			m_cam_y  = (pix_y - (pix_y % 256)) / 16;
			m_tile_y = 0;
		}

		void cam_focus_x( int pix_x )
		{
			int cx = (pix_x / 16) - 8;

			if ( cx > (m_map_w - 17) )
			{
				m_cam_x  = m_map_w - 16;
				m_tile_x = 0;
				return;
			}
			if ( cx < 0 )
			{
				m_cam_x  = 0;
				m_tile_x = 0;
				return;
			}

			m_cam_x  = cx;
			m_tile_x = (pix_x % 16) * -1;
			//m_tile_x = 0;
		}
		void cam_focus_y( int pix_y )
		{
			int cy = (pix_y / 16) - 8;

			if ( cy > (m_map_h - 17) )
			{
				m_cam_y  = m_map_h - 16;
				m_tile_y = 0;
				return;
			}
			if ( cy < 0 )
			{
				m_cam_y  = 0;
				m_tile_y = 0;
				return;
			}

			m_cam_y  = cy;
			m_tile_y = (pix_y % 16) * -1;
			//m_tile_y = 0;
		}

		void anim_bg( int dummy, int id )
		{
			m_ver_bg[ dummy ] = m_ver_bg[ id ];
		}
		void anim_fg( int dummy, int id )
		{
			m_ver_fg[ dummy ] = m_ver_fg[ id ];
		}

		void update_tileset(int padx, int pady, int sz_w, int sz_h)
		{
			m_vertices.clear();
			m_vertices.setPrimitiveType(sf::Quads);

			arrver  ver;
			switch ( m_cur_map )
			{
				case 0:
					ver = m_ver_col;
					break;
				case 1:
					ver = m_ver_bg;
					break;
				case 2:
					ver = m_ver_obj;
					break;
				case 3:
					ver = m_ver_fg;
					break;
				break;
			} // switch m_cur_map
			m_vertices.resize( ver.size() * 4 );

			int n, x1, y1;
			int w1 = 16;
			int h1 = 0;
			for ( n=1; n < ver.size(); n++ )
			{
				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[n * 4];

				// define its 4 corners
				x1 = padx + w1;
				y1 = pady + h1;
				quad[0].position = sf::Vector2f( x1      , y1      );
				quad[1].position = sf::Vector2f( x1 + 16 , y1      );
				quad[2].position = sf::Vector2f( x1 + 16 , y1 + 16 );
				quad[3].position = sf::Vector2f( x1      , y1 + 16 );

				// define its 4 texture coordinates
				x1 = ver[n].x;
				y1 = ver[n].y;
				quad[0].texCoords = sf::Vector2f( x1      , y1     );
				quad[1].texCoords = sf::Vector2f( x1 + 16 , y1     );
				quad[2].texCoords = sf::Vector2f( x1 + 16 , y1 + 16);
				quad[3].texCoords = sf::Vector2f( x1      , y1 + 16);

				w1 += 16;
				if ( w1 >= sz_w )
				{
					h1 += 16;
					w1  = 0;
				}
				if ( h1 >= sz_h )
					return;
			} // for n=1

		}

		void update_map( int bright = 255 )
		{
			m_vertices.clear();
			m_vertices.setPrimitiveType(sf::Quads);
			m_vertices.resize          (16 * 16 * 4);

			if ( m_cam_x < 0 )   m_cam_x += m_map_w;
			if ( m_cam_y < 0 )   m_cam_y += m_map_h;

			int  x, y, cx, cy, n;
			int  r_id, x1, y1;
			vertex  v;
			for (y=0; y < 16; y++)
			{
				cy = ( (m_cam_y + y) % m_map_h ) * m_map_w;
				for (x=0; x < 16; x++)
				{
					cx = ( (m_cam_x + x) % m_map_w ) + cy;
					n  = x + (y * 16);

					switch ( m_cur_map )
					{
						case 0:
							r_id = m_map_col[cx];
							v = m_ver_col[ r_id ];
							break;
						case 1:
							r_id = m_map_bg[cx];
							v = m_ver_bg[ r_id ];
							break;
						case 2:
							r_id = m_map_obj[cx];
							v = m_ver_obj[ r_id ];
							break;
						case 3:
							r_id = m_map_fg[cx];
							v = m_ver_fg[ r_id ];
						break;
					}
					if ( r_id == 0 )
						continue;

					// get a pointer to the current tile's quad
					sf::Vertex* quad = &m_vertices[n * 4];

					quad[0].color = sf::Color( bright, bright, bright );
					quad[1].color = sf::Color( bright, bright, bright );
					quad[2].color = sf::Color( bright, bright, bright );
					quad[3].color = sf::Color( bright, bright, bright );

					// define its 4 corners
					x1 = m_pad_x + (x * 16) + m_tile_x;
					y1 = m_pad_y + (y * 16) + m_tile_y;
					quad[0].position = sf::Vector2f( x1       , y1       );
					quad[1].position = sf::Vector2f( x1 + v.w , y1       );
					quad[2].position = sf::Vector2f( x1 + v.w , y1 + v.h );
					quad[3].position = sf::Vector2f( x1       , y1 + v.h );

					// define its 4 texture coordinates
					x1 = v.x + v.w;
					y1 = v.y + v.h;
					quad[0].texCoords = sf::Vector2f( v.x , v.y);
					quad[1].texCoords = sf::Vector2f( x1  , v.y);
					quad[2].texCoords = sf::Vector2f( x1  , y1 );
					quad[3].texCoords = sf::Vector2f( v.x , y1 );

				} // for x=0
			} // for y=0
		}

		void load_lvl( std::string &lvl )
		{
			printf("%s.load_lvl( %s )\n", m_class_name.c_str(), lvl.c_str() );
			m_fn_lvl  = lvl;

			m_has_lvl = false;
			FILE* fp = fopen( lvl.c_str(), "r" );
				if ( ! fp )   return;

			uint fsz = rfs::filesize(fp);
			uint p;
			uint row = 0;
			char c;
			std::string  line = "";
			std::string  sect = "system";
			bool skip = false;

			for ( p=0; p < fsz; p++ )
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
							parse_lvl( line, sect, row );
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
			m_has_lvl = true;
		}

		void load_def( std::string &def )
		{
			printf("%s.load_def( %s )\n", m_class_name.c_str(), def.c_str());
			m_fn_def  = def;

			m_has_def = false;
			FILE* fp = fopen( def.c_str(), "r" );
				if ( ! fp )   return;

			uint fsz = rfs::filesize(fp);

			uint  p;
			char  c;
			std::string   line = "";
			bool skip = false;
			for ( p=0; p < fsz; p++ )
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

			uint msz = m_map_w * m_map_h;
			printf("MAP %d x %d = %d\n", m_map_w, m_map_h, msz);
			if ( msz == 0 )
				return;
			m_map_col.resize( msz );
			m_map_bg.resize( msz );
			m_map_obj.resize( msz );
			m_map_fg.resize( msz );

			m_has_def = true;
		}

		void load_file( std::string &prefix )
		{
			printf("%s.load_file( %s )\n", m_class_name.c_str(), prefix.c_str() );
			rfs::dirname( m_rel_path, prefix );
			std::string  def = prefix + ".def";
				load_def(def);
			std::string  lvl = prefix + ".lvl";
				load_lvl(lvl);
		}
		void load_file( const char prefix[] )
		{
			std::string  p = prefix;
			load_file(p);
		}

		void set_pad( int pad_x, int pad_y )
		{
			m_pad_x = pad_x;
			m_pad_y = pad_y;
		}

		LevelMap()
		{
			m_class_name = "LevelMap";
			m_has_def = false;
			m_has_lvl = false;
			m_cam_x = 0;
			m_cam_y = 0;
			m_tile_x = 0;
			m_tile_y = 0;
			m_map_w = 0;
			m_map_h = 0;
			m_pad_x = 0;
			m_pad_y = 0;
			m_cur_map = 0;
			m_rel_path = "";

			m_ver_col.reserve( 0xff );
			m_ver_bg.reserve( 0xff );
			m_ver_obj.reserve( 0xff );
			m_ver_fg.reserve( 0xff );
			m_obj_list.reserve( 0xff );
		}
}; // class LevelMap

#endif // LEVELMAP_HPP
