namespace amy
{
	class Object
	{
		public:
			Object();
			~Object();
			bool is_incamera;
			bool is_onhold;
			int base_pos_x; // for respawn (npc) and checkpoint (player)
			int base_pos_y;
			int pos_x; // MUST BE int ! float causes weird map collusion
			int pos_y;
			int center_x; // for homing missiles or attacks, for calculate distance
			int center_y;
			int tile_width; // specify the tile image size
			int tile_height;
			int char_width; // specify the rect size
			int char_height;
			int rect_adj_x; // adjust the start pos for rect, normally is 0,0
			int rect_adj_y;
			int rect_top;  // char rect
			int rect_bottom;
			int rect_left;
			int rect_right;
			int mv_speed_x; // current moving speed, use int because pos is int
			int mv_speed_y;
			sf::Texture char_texture; // current tile to show
			amy::Rect char_rect;

			virtual void draw();
			void set_defaults();
			void set_pos(int, int);
			void set_basepos(int, int);
			void center2pos(int, int);
			void update_rect();
			void force_mv_up   (int = 1); // move char, no colission, no checking
			void force_mv_down (int = 1);
			void force_mv_left (int = 1);
			void force_mv_right(int = 1);
	};
}

namespace amy
{
	class Actor : public amy::Object
	{
		public:
			Actor();
			virtual ~Actor();
			bool is_player;
			bool is_boss;
			bool is_control;
			bool is_left;
			bool is_dead;
			bool is_invicible;
			bool is_walk;
			bool is_shot;
			bool is_dash;
			bool is_action;
			bool is_action_done;
			bool is_jump;
			bool is_wallkick;
			bool is_leftwall;
			bool is_rightwall;
			int prev_state;
			int curr_state;
			int action_frame;
			int action_type;
			int draw_frame;
			int countdown_invicible; // count down for various action
			int countdown_shoot;
			int countdown_dash;
			int countdown_jump;
			int countdown_action;
			bool contact_topwall;
			bool contact_bottomwall;
			bool contact_leftwall;
			bool contact_rightwall;
			std::vector	< sf::Texture > tileset;
			std::list < int > action_queue;

			void update();
			void update_state(int st, int max = 1, int loop_end = 0, int loop_begin = 0);
			virtual void update_player();
			virtual void update_npc();
			void draw();
			void set_tileset(const std::string&, int col, int row, int w, int h);
			void jump(); // general jump calculation, it has nothing to do with animation
			void leftjump();
			void rightjump();
			int collusion_wall_x(int, int);
			int collusion_wall_y(int, int);
			void mv_up   (int = 0); // move char, it has nothing to do with animations
			void mv_down (int = 0);
			void mv_left (int = 0);
			void mv_right(int = 0);
		protected:
		private:
	};
}
