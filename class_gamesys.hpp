#ifndef GAMESYS_HPP
#define GAMESYS_HPP

#include "rfstk.hpp"
#include <SFML/Graphics.hpp>

class gamesys {
	private:
		bool   m_turbo;
		bool   m_sshot;    uint   m_sshot_no;
		std::string   m_appname;

		void event_sshot()
		{
			sf::Texture   texture;
			sf::Vector2u  win_sz;

			std::string  sshot_fn;
			char  sshot_no[10];
			FILE* fp;
			while(1)
			{
				rfs::memset( sshot_no, 10 );
				sprintf(sshot_no, "%06d", m_sshot_no);
				sshot_fn = m_appname + "_" + sshot_no + ".png";

				// DO NOT OVERWRITE
				fp = fopen( sshot_fn.c_str(), "r" );
				if ( fp )
				{
					fclose(fp);
					m_sshot_no++;
				}
				else
				{
					//m_win.capture().saveToFile( sshot_fn );
					win_sz = m_win.getSize();
					texture.create( win_sz.x, win_sz.y );
					texture.update( m_win );
					texture.copyToImage().saveToFile( sshot_fn );

					printf("SSHOT %4dx%4d [ %s ]\n", win_sz.x, win_sz.y, sshot_fn.c_str() );
					m_sshot_no++;
					return;
				}
			} // while 1
		}

		void reset_opt()
		{
			m_closed   = false;
			m_paused   = false;
			m_ctrl     = m_shift = m_alt  = m_sys = false;
			m_turbo    = false;
			m_sshot    = false;
			m_sshot_no = 0;
			m_zoom     = 1;
			m_fps      = 24;
			m_fps_no   = 24;
			m_appname  = "";

			m_Dup    = m_Ddown   = m_Dleft = m_Dright = false;
			m_Dbut1  = m_Dbut2   = m_Dbut3 = m_Dbut4  = false;
			m_DL1    = m_DR1     = false;
			m_Dstart = m_Dselect = false;

			m_Kbut1  = m_Kbut2   = m_Kbut3 = m_Kbut4 = sf::Keyboard::Unknown;
			m_KL1    = m_KR1     = sf::Keyboard::Unknown;
			m_Kstart = m_Kselect = sf::Keyboard::Unknown;

			m_Kup    = sf::Keyboard::Up;
			m_Kdown  = sf::Keyboard::Down;
			m_Kleft  = sf::Keyboard::Left;
			m_Kright = sf::Keyboard::Right;
		}

	protected:
	public:
		bool   m_closed;
		bool   m_paused;
		sf::RenderWindow    m_win;
		int    m_zoom;
		int    m_ctrl;     int    m_shift;   int    m_alt;     int    m_sys;

		uint   m_fps;      uint   m_fps_no;
		uint   m_w;        uint   m_h;
		uint   m_half_w;   uint   m_half_h;

		bool   m_Dup;      bool   m_Ddown;   bool   m_Dleft;   bool   m_Dright;
		bool   m_Dbut1;    bool   m_Dbut2;   bool   m_Dbut3;   bool   m_Dbut4;
		bool   m_DL1;      bool   m_DR1;
		bool   m_Dstart;   bool   m_Dselect;

		int    m_Kup;      int    m_Kdown;   int    m_Kleft;   int    m_Kright;
		int    m_Kbut1;    int    m_Kbut2;   int    m_Kbut3;   int    m_Kbut4;
		int    m_KL1;      int    m_KR1;
		int    m_Kstart;   int    m_Kselect;

		void event_handler()
		{
			if ( m_closed )   m_win.close();
			if ( m_sshot  )   event_sshot();

			if ( m_turbo )
				m_win.setFramerateLimit(48);
			else
				m_win.setFramerateLimit(24);
		}

		void input_handler()
		{
			sf::Event event;
			while ( m_win.pollEvent(event) )
			{
				if ( event.type == sf::Event::Closed )
					m_closed = true;
				//if ( event.type == sf::Event::Resized )
					//m_win.setSize( sf::Vector2u(event.size.width, event.size.height) );
				if ( event.type == sf::Event::LostFocus )
					m_paused = true;
				if ( event.type == sf::Event::GainedFocus )
					m_paused = false;
				if ( event.type == sf::Event::KeyPressed )
				{
					if ( event.key.code == sf::Keyboard::Escape  )   m_closed = true;
					if ( event.key.code == sf::Keyboard::Pause   )   m_paused = !(m_paused);
					if ( event.key.code == sf::Keyboard::Tab     )   m_turbo  = !(m_turbo);

					if ( event.key.code == sf::Keyboard::F1      )   resize_win(1);
					if ( event.key.code == sf::Keyboard::F2      )   resize_win(2);
					if ( event.key.code == sf::Keyboard::F3      )   resize_win(3);
					if ( event.key.code == sf::Keyboard::F4      )   resize_win(4);
					if ( event.key.code == sf::Keyboard::F12     )   m_sshot = true;

					if ( event.key.code == sf::Keyboard::LControl )   m_ctrl  = true;
					if ( event.key.code == sf::Keyboard::RControl )   m_ctrl  = true;
					if ( event.key.code == sf::Keyboard::LShift   )   m_shift = true;
					if ( event.key.code == sf::Keyboard::RShift   )   m_shift = true;
					if ( event.key.code == sf::Keyboard::LAlt     )   m_alt = true;
					if ( event.key.code == sf::Keyboard::RAlt     )   m_alt = true;
					if ( event.key.code == sf::Keyboard::LSystem  )   m_sys = true;
					if ( event.key.code == sf::Keyboard::RSystem  )   m_sys = true;

					if ( event.key.code == m_Kup    )   m_Dup    = true;
					if ( event.key.code == m_Kdown  )   m_Ddown  = true;
					if ( event.key.code == m_Kleft  )   m_Dleft  = true;
					if ( event.key.code == m_Kright )   m_Dright = true;

					if ( event.key.code == m_Kbut1 )   m_Dbut1 = true;
					if ( event.key.code == m_Kbut2 )   m_Dbut2 = true;
					if ( event.key.code == m_Kbut3 )   m_Dbut3 = true;
					if ( event.key.code == m_Kbut4 )   m_Dbut4 = true;

					if ( event.key.code == m_KL1 )   m_DL1 = true;
					if ( event.key.code == m_KR1 )   m_DR1 = true;

					if ( event.key.code == m_Kstart  )   m_Dstart  = true;
					if ( event.key.code == m_Kselect )   m_Dselect = true;
				}
				if ( event.type == sf::Event::KeyReleased )
				{
					if ( event.key.code == sf::Keyboard::F12     )   m_sshot = false;

					if ( event.key.code == sf::Keyboard::LControl )   m_ctrl  = false;
					if ( event.key.code == sf::Keyboard::RControl )   m_ctrl  = false;
					if ( event.key.code == sf::Keyboard::LShift   )   m_shift = false;
					if ( event.key.code == sf::Keyboard::RShift   )   m_shift = false;
					if ( event.key.code == sf::Keyboard::LAlt     )   m_alt = false;
					if ( event.key.code == sf::Keyboard::RAlt     )   m_alt = false;
					if ( event.key.code == sf::Keyboard::LSystem  )   m_sys = false;
					if ( event.key.code == sf::Keyboard::RSystem  )   m_sys = false;

					if ( event.key.code == m_Kup    )   m_Dup    = false;
					if ( event.key.code == m_Kdown  )   m_Ddown  = false;
					if ( event.key.code == m_Kleft  )   m_Dleft  = false;
					if ( event.key.code == m_Kright )   m_Dright = false;

					if ( event.key.code == m_Kbut1 )   m_Dbut1 = false;
					if ( event.key.code == m_Kbut2 )   m_Dbut2 = false;
					if ( event.key.code == m_Kbut3 )   m_Dbut3 = false;
					if ( event.key.code == m_Kbut4 )   m_Dbut4 = false;

					if ( event.key.code == m_KL1 )   m_DL1 = false;
					if ( event.key.code == m_KR1 )   m_DR1 = false;

					if ( event.key.code == m_Kstart  )   m_Dstart  = false;
					if ( event.key.code == m_Kselect )   m_Dselect = false;
				}
			} // while ( m_win.pollEvent(event) )
		}

		void resize_win( int zoom )
		{
			m_zoom = zoom;
			m_win.setSize( sf::Vector2u(m_w * zoom, m_h * zoom) );
		}

		void create_win( uint w, uint h, const char title[] )
		{
			// 0 = sf::Style::None
			// 1 = sf::Style::Titlebar [Default]
			// 2 = sf::Style::Resize   [Default]
			// 4 = sf::Style::Close    [Default]
			// 8 = sf::Style::Fullscreen
			m_win.create( sf::VideoMode(w, h), title, sf::Style::Titlebar | sf::Style::Close );
				m_win.setPosition(sf::Vector2i(0, 0));
				m_win.setFramerateLimit(24);
				m_win.setMouseCursorVisible(true);
				//win.setVerticalSyncEnabled(true);
			m_w = w;
			m_h = h;
			m_half_w = w / 2;
			m_half_h = h / 2;
			m_appname = title;

			printf("INFO : Max Texture Size : %d\n", sf::Texture::getMaximumSize() );
		}

		gamesys()
		{
			reset_opt();
		}
}; // class gamesys

#endif // GAMESYS_HPP
