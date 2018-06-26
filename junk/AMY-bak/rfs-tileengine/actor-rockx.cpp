#include "define.h"
using namespace amy;
extern State STATE;

RockX::RockX()
{
	Actor::char_width  = 16;
	Actor::char_height = 32;
	Actor::rect_adj_x = 24;
	Actor::rect_adj_y = 16;
	Actor::mv_speed_x = 3;
	Actor::mv_speed_y = 9;
	Actor::set_tileset("actor/rockx.png", 16, 7, 64, 64);

	// animation frame data
	int anm_stand[14]  = { 7, 7, 7, 7, 8, 9, 7, 7, 8, 9, 7, 7, 8, 9 };
	int anm_stand_s[2] = { 14, 15 };
	int anm_walk[11]   = { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };
	int anm_walk_s[11] = { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42 };
	int anm_dash[4]    = { 30, 31, 31, 30 };
	int anm_dash_s[4]  = { 46, 47, 47, 46 };
	int anm_jump[4]    = { 48, 49, 50, 50 };
	int anm_jump_s[4]  = { 64, 65, 66, 66 };
	int anm_fall[4]    = { 51, 52, 53, 54 };
	int anm_fall_s[4]  = { 67, 68, 69, 70 };
	int anm_wallslide[4]   = { 80, 81, 81, 81 };
	int anm_wallslide_s[4] = { 96, 97, 97, 97 };
	int anm_wallkick[4]    = { 82, 83, 84, 84 };
	int anm_wallkick_s[4]  = { 98, 99, 100, 100 };

	RockX::anm_stand.assign(   anm_stand,  14, true);
	RockX::anm_stand_s.assign( anm_stand_s, 2, true);
	RockX::anm_walk.assign(    anm_walk,   11, true);
	RockX::anm_walk_s.assign(  anm_walk_s, 11, true);
	RockX::anm_dash.assign(    anm_dash,    4, true);
	RockX::anm_dash_s.assign(  anm_dash_s,  4, true);
	RockX::anm_jump.assign(    anm_jump,    4, true);
	RockX::anm_jump_s.assign(  anm_jump_s,  4, true);
	RockX::anm_fall.assign(    anm_fall,    4, true);
	RockX::anm_fall_s.assign(  anm_fall_s,  4, true);
	RockX::anm_wallslide.assign(   anm_wallslide,    4, true);
	RockX::anm_wallslide_s.assign( anm_wallslide_s,  4, true);
	RockX::anm_wallkick.assign(    anm_wallkick,     4, true);
	RockX::anm_wallkick_s.assign(  anm_wallkick_s,   4, true);
}
RockX::~RockX() {}

void RockX::update_player()
{
	Actor::update();

	// !!!BIG CONTROL COMMENT!!!
	// main move buttons = left/right
	// main action button = shot, jump, dash
	// order : dash -> jump -> left/right -> shot
	// dash -> jump for dashjump (jump -> dash for airdash)
	// dash -> left/right for disable mid-dash change direction
	// jump -> left/right for pixel-perfect jump (jump first then move)
	// shot is available on every state
	// shot+hold doesn't affect others

	// default state is FALL
	// gravity check on each update
	// do not check for gravity when on jump (JUMP)
	// do not check for gravity when on wallkick (WALLKICK)
	// do not check for gravity when on damaged (DAMAGED)
	// do not check for gravity when on action (BREAKDASH)
	// do not check for gravity when on airdash (jumping + DASH)
	// on contact bottom wall (->STAND)
	// press left + contact left wall (WALLSLIDE)
	// WALLSLIDE + press left + press jump (WALLKICK)
	// WALKSLIDE release left = FALL (no jump again!)
	// until STAND or WALLSLIDE, do not allow JUMP again
	// until STAND or WALLSLIDE, do not allow DASH again
	// on DASH, press left/right has no effect
	// on WALLSLIDE, press dash has no effect (except trigger the flag for dashjump)
	// on action (BREAKDASH), press any button has no effect
	// on JUMP, limit jump to max height, then change to FALL
	// on SHOT, check for recoil time before allow SHOT again
	// on SHOT, when hold, allow every action with charging state
	// ACTION includes BREAK-DASH, NOVA-STRIKE, ASTRO-CRUSH, FIREFLY-LASER, etc...

	// FALL reset jumpspeed, so jump reached its peak
	// FALL set jumping to true, so jump cannot jump again

	// FALL stimulation
	// 1.  idle								(FALL)			(y)
	// 2.  idle, contact floor				(STAND)			(y)
	// 2a. press left, walk left			(WALK)			(y)
	// 2b. hold left, walk down a slope		(WALK)			(n1)
	// 2c. hold left, walk off cliff		(FALL)			(y)
	// 3.  idle, contact floor				(STAND)			(y)
	// 3a. press dash, dash					(DASH)			(y)
	// 3b. hold dash, dash down a slope		(DASH)			(n1)
	// 3c. hold dash, dash off cliff		(FALL)			(y)
	// 4.  idle, contact floor				(STAND)			(y)
	// 4a. press jump, jump					(JUMP)			(-)
	// 4b, hold jump, reached max height	(FALL)			(y)
	// 5.  idle, contact floor				(STAND)			(y)
	// 5a. press jump, jump					(JUMP)			(-)
	// 5b. press dash, dash					(jump + DASH)	(-)
	// 5c. hold dash, reach max range		(FALL)			(y)
	// 6.  idle, contact floor				(STAND)			(y)
	// 6b. idle, floor breaks				(FALL)			(y)
	// 7.  idle, contact floor				(STAND)			(y)
	// 7a. press shot, perform UPPERCUT		(ACTION)		(-)
	// 7b. idle, action finished			(FALL)			(y)
	// 8.  idle, contact floor				(STAND)			(y)

	// NOTES FROM ABOVE
	// from 1 = default FALL, contact floor STAND
	// from 2&3&6 = check for FALL on every update, interrupt if true
	// from 4 = FALL has jumping set to true
	//          jumpspeed > 0 = JUMP, jumpspeed < 0 = FALL
	//          jumpspeed is reduced by each update, so gravity is set to off (to avoid double reduce)
	// from 5 = if dashing when jumping = true, set gravity to off
	// from 7 = ACTION also set gravity to off
	//          ACTION also set jumping to true (no jump after ACTION)

	// n1 - a slope will auto-adjust the POS,
	//      so WALK / DASH / ACTION will react like plain floor

	// seperated controls
	// mid-air / floor = true
	// idle = countdown > 0 = JUMP, else FALL
	// left/right = move but no change
	// jump = disabled (or hold to reduce countdown normally to keep JUMP, release to set as 0 to cause FALL)
	// dash = air DASH (no gravity check) or disabled

	// ground / floor = true
	// idle = STAND
	// left/right = WALK
	// jump = JUMP, cause floor = false, change control to mid-air
	// dash = DASH
	// dashjump = on DASH then JUMP, after DASH holding dash won't dashjump

	// wall / press left while contact left wall
	// idle = FALL
	// left/right = WALLSLIDE, opposite wall = FALL
	// jump = WALLKICK
	// dash = do nothing, hold will toggle state for DASHJUMP + DWALLKICK

	// check if mid-air or ground control
	if ( Actor::contact_bottomwall )
	{
		Actor::is_jump      = false;
		Actor::is_leftwall  = false;
		Actor::is_rightwall = false;
	}



	if ( STATE.KEYS[ KEY_RT ] && Actor::is_rightwall )
		RockX::ctrl_wall(false);
	else if ( STATE.KEYS[ KEY_LF ] && Actor::is_leftwall )
		RockX::ctrl_wall(true);
	else
	{
		// mid-air control
		if ( Actor::is_jump )
			RockX::ctrl_air();
		// ground control
		// check for floor on every update
		else
		{
			if ( Actor::contact_bottomwall )
				RockX::ctrl_floor();
			else
				RockX::cmd_fall();
		}
	}




	// update frame index to draw
	Actor::draw_frame = RockX::update_frame();

	//if ( DEBUG ) printf(">> RockX anm %d draw %d\n", Actor::action_frame, Actor::draw_frame);
}

void RockX::update_npc()
{
	Actor::update();
}

void RockX::ctrl_air()
{
	printf("RockX mid-air control...\n");
	bool arrowbtn = true;
	bool gravity = true;

	// wallslide & wallkick happens on mid-air only
	Actor::is_rightwall = ( STATE.KEYS[ KEY_RT ] && Actor::contact_rightwall ) ? true : false;
	Actor::is_leftwall  = ( STATE.KEYS[ KEY_LF ] && Actor::contact_leftwall )  ? true : false;

	if ( STATE.KEYS[ KEY_DSH ] )
	{
		RockX::cmd_dash();
		arrowbtn = false;
	}
	else
		Actor::is_dash = false;

	// jump button
	if ( STATE.KEYS[ KEY_JMP ] )
	{
		// check if jump up or jump down
		if ( Actor::countdown_jump > 0 )
		{
			if ( Actor::is_dash )
				RockX::cmd_jump(false);
			else
				RockX::cmd_jump();
			gravity = false;
		}
	}

	// direction buttons
	if ( arrowbtn )
	{
		if ( STATE.KEYS[ KEY_RT ] )
			RockX::cmd_move_air( 1);
		else if ( STATE.KEYS[ KEY_LF ] )
			RockX::cmd_move_air(-1);
	}

	if ( gravity )
		RockX::cmd_fall();
}

void RockX::ctrl_floor()
{
	printf("RockX floor control...\n");
	bool arrowbtn = true;
	//bool gravity = true;

	if  ( STATE.KEYS[ KEY_DSH ] )
	{
		// not working when dash (for speedup / slowdown bug)
		arrowbtn = false;
		RockX::cmd_dash();
	}
	else
		Actor::is_dash = false;

	// jump button
	if ( STATE.KEYS[ KEY_JMP ] )
	{
		Actor::is_jump = true;
		Actor::countdown_jump = 50;
		if ( Actor::is_dash )
		{
			RockX::cmd_move_air(0);
			RockX::cmd_jump(false);
		}
		else
			RockX::cmd_jump();
	}

	if ( arrowbtn )
	{
		// direction buttons
		if ( STATE.KEYS[ KEY_RT ] )
			RockX::cmd_move( 1);
		else if ( STATE.KEYS[ KEY_LF ] )
			RockX::cmd_move(-1);
		else
			RockX::cmd_stand();
	}
}

void RockX::ctrl_wall(bool left)
{
	printf("RockX wall control...\n");
	bool wkick = false;
	Actor::is_jump = true;
	Actor::is_dash = ( STATE.KEYS[ KEY_DSH ] ) ? true : false;

	if ( Actor::is_wallkick )
	{
		if ( STATE.KEYS[ KEY_JMP ] )
		{
			if ( Actor::countdown_jump > 0 )
				wkick = true;
		}
	}
	else
	{
		if ( STATE.KEYS[ KEY_JMP ] )
		{
			Actor::is_wallkick = true;
			Actor::countdown_jump = 50;
			wkick = true;
		}
	}

	if ( wkick )
	{
		RockX::cmd_wallkick(left);
		if ( STATE.KEYS[ KEY_RT ] )
			Actor::is_left = false;
		else if ( STATE.KEYS[ KEY_LF ] )
			Actor::is_left = true;
	}
	else
	{
		if ( left )
			Actor::mv_left( Actor::mv_speed_x * 2 );
		else
			Actor::mv_right( Actor::mv_speed_x * 2 );
	}

	// standard check
	if ( left && Actor::contact_leftwall )
	{
		RockX::cmd_wallslide(true);
		Actor::is_wallkick = false;
	}

	if ( !left && Actor::contact_rightwall )
	{
		RockX::cmd_wallslide(false);
		Actor::is_wallkick = false;
	}

}

void RockX::cmd_move(int dir)
{
	int mv;

	// 0 = neutral
	if ( dir == 0 )
		mv = ( Actor::is_left ) ? -1 : 1;
	else
		mv = dir;

	// positive = right
	if ( mv > 0 )
	{
		Actor::mv_right();
		Actor::is_left = false;
		Actor::is_walk = ( Actor::contact_rightwall ) ? false : true;
	}
	// negative = left
	else if ( mv < 0 )
	{
		Actor::mv_left();
		Actor::is_left = true;
		Actor::is_walk = ( Actor::contact_leftwall ) ? false : true;
	}

	// set frame to animate
	if ( Actor::is_walk )
		Actor::update_state( WALK, 10 );
	else
		Actor::update_state( STAND, 13 );

}

void RockX::cmd_move_air(int dir)
{
	int mv;

	// 0 = neutral
	if ( dir == 0 )
		mv = ( Actor::is_left ) ? -1 : 1;
	else
		mv = dir;

	// positive = right
	if ( mv > 0 )
	{
		Actor::mv_right();
		Actor::is_left = false;
	}
	// negative = left
	else if ( mv < 0 )
	{
		Actor::mv_left();
		Actor::is_left = true;
	}
}

void RockX::cmd_stand()
{
	Actor::update_state( STAND, 13 );
}

void RockX::cmd_fall()
{
	//printf("RockX falling...\n");
	Actor::update_state( FALL );
	Actor::is_jump = true;
	Actor::countdown_jump = -10;
	Actor::mv_down();
}

void RockX::cmd_jump(bool jmp)
{
	//printf("RockX jumping...\n");
	if ( jmp )
		Actor::update_state( JUMP );
	Actor::mv_up();
}

void RockX::cmd_dash(bool dsh)
{
	//printf("RockX dashing...\n");
	//Actor::countdown_jump = -10;
	if ( Actor::is_left )
	{
		if ( ! Actor::contact_leftwall )
		{
			Actor::is_dash = true;
			Actor::mv_left( Actor::mv_speed_x * 2 );
		}
	}
	else
	{
		if ( ! Actor::contact_rightwall )
		{
			Actor::is_dash = true;
			Actor::mv_right( Actor::mv_speed_x * 2 );
		}
	}

	if ( Actor::is_dash )
		if ( dsh )
			Actor::update_state( DASH, 3, 2 );
}

void RockX::cmd_wallslide(bool left)
{
	Actor::mv_down( Actor::mv_speed_y / 2 );
	if ( left )
		Actor::is_left = false;
	else
		Actor::is_left = true;

	Actor::update_state( WALLSLIDE );
}

void RockX::cmd_wallkick(bool left)
{
	Actor::mv_up( Actor::mv_speed_y * 2 );
	if ( left )
	{
		if ( Actor::is_dash )
			Actor::mv_right( Actor::mv_speed_x * 2 );
		else
			Actor::mv_right( Actor::mv_speed_x );
	}
	else
	{
		if ( Actor::is_dash )
			Actor::mv_left( Actor::mv_speed_x * 2 );
		else
			Actor::mv_left( Actor::mv_speed_x );
	}
	Actor::update_state( WALLKICK );
}

void RockX::cmd_hitd()
{
}

void RockX::cmd_shoot(bool xb)
{
	// no animation, just set the bool and create the bullets objects
	// check if you are allowed to shoot more bullets (overshot)
	// check if you have any recoil time (no rapid firing)
	if ( Actor::countdown_shoot > 0 )
	{
	}
	else
	{
		Actor::is_shot = true;
		// create the bullet object and set recoil time
	}
}

void RockX::bul_breakdash()
{
	if ( !Actor::is_action )
	{
		Actor::countdown_action = 60;
		Actor::is_action = true;
	}
}

int RockX::update_frame()
{
	switch ( Actor::curr_state )
	{
		case WALK:
		{
			if ( Actor::is_shot )
				return RockX::anm_walk_s.at( Actor::action_frame );
			else
				return RockX::anm_walk.at( Actor::action_frame );
			break;
		}
		case DASH:
		{
			if ( Actor::is_shot )
				return RockX::anm_dash_s.at( Actor::action_frame );
			else
				return RockX::anm_dash.at( Actor::action_frame );
			break;
		}
		case JUMP:
		{
			if ( Actor::is_shot )
				return 66;
				//return RockX::anm_jump_s.at( Actor::action_frame );
			else
				return 50;
				//return RockX::anm_jump.at( Actor::action_frame );
			break;
		}
		case FALL:
		{
			if ( Actor::is_shot )
				return 68;
				//return RockX::anm_fall_s.at( Actor::action_frame );
			else
				return 52;
				//return RockX::anm_fall.at( Actor::action_frame );
			break;
		}
		case WALLSLIDE:
		{
			if ( Actor::is_shot )
				return 97;
				//return RockX::anm_wallslide_s.at( Actor::action_frame );
			else
				return 81;
				//return RockX::anm_wallslide.at( Actor::action_frame );
			break;
		}
		case WALLKICK:
		{
			if ( Actor::is_shot )
				return 100;
				//return RockX::anm_wallkick_s.at( Actor::action_frame );
			else
				return 84;
				//return RockX::anm_wallkick.at( Actor::action_frame );
			break;
		}
		case STAND:
			return RockX::anm_stand.at( Actor::action_frame );
			break;
		case SHOT:
			return 14;
			//return RockX::anm_stand_s.at( Actor::action_frame );
			break;
	}
	return 0;
}
//----------------------------------------------------------------
