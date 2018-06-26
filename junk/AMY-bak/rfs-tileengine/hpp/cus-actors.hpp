class RockX : public amy::Actor
{
	public:
		RockX();
		~RockX();
		enum statefnt { IDLE, STAND, WEAK, SHOT, WALK, JUMP, FALL, DASH, WALLSLIDE, WALLKICK, DAMAGED, TEL_IN, TEL_OUT, VICTORY };
		enum weaponfnt { XB, ME, VE, EA, MA, JU, SA, NE, UR, PL, ON };
		int weapons;
		int charge;
		amy::iVect anm_stand;
		amy::iVect anm_stand_s;
		amy::iVect anm_walk;
		amy::iVect anm_walk_s;
		amy::iVect anm_dash;
		amy::iVect anm_dash_s;
		amy::iVect anm_jump;
		amy::iVect anm_jump_s;
		amy::iVect anm_fall;
		amy::iVect anm_fall_s;
		amy::iVect anm_wallslide;
		amy::iVect anm_wallslide_s;
		amy::iVect anm_wallkick;
		amy::iVect anm_wallkick_s;

		void update_player();
		void update_npc();
		int update_frame();
		void ctrl_action();
		void ctrl_air();
		void ctrl_floor();
		void ctrl_wall(bool = false);
		void cmd_action();
		void cmd_move(int dir = 0);
		void cmd_move_air(int dir = 0);
		void cmd_stand();
		void cmd_fall();
		void cmd_jump(bool = true);
		void cmd_dash(bool = true);
		void cmd_wallslide(bool = false);
		void cmd_wallkick(bool = false);
		void cmd_hitd();
		void cmd_shoot(bool = false);
		void bul_breakdash();
};
