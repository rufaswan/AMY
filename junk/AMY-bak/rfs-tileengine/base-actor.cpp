#include "define.h"
using namespace amy;
extern State STATE;

Actor::Actor()
{
	Actor::is_player	= false;
	Actor::is_boss		= false;
	Actor::is_control	= false;
	Actor::is_left		= false;
	Actor::is_action	= false;
	Actor::is_action_done = false;
	Actor::is_invicible	= false;
	Actor::is_dead		= false;
	Actor::is_dash		= false;
	Actor::is_jump		= false;
	Actor::is_wallkick	= false;
	Actor::is_leftwall	= false;
	Actor::is_rightwall	= false;
	Actor::mv_speed_x = 0;
	Actor::mv_speed_y = 0;
	Actor::countdown_jump	= -10;
	Actor::prev_state   = 0;
	Actor::curr_state   = 0;
	Actor::action_frame = 0;
	Actor::draw_frame   = 0;
	Actor::countdown_invicible = 0;
	Actor::countdown_shoot     = 0;
	Actor::countdown_action    = 0;
	Actor::action_queue.clear();
}
Actor::~Actor() {}
void Actor::update_player() {}
void Actor::update_npc() {}

void Actor::set_tileset(const std::string& file, int col, int row, int w, int h)
{
	if ( DEBUG ) printf(">> setting actor file : %s...\n", file.c_str() );
	int x, y;
	int c = 0;
	STATE.add_tileimg(file);
	Object::tile_width = w;
	Object::tile_height = h;

	Actor::tileset.clear();
	for ( y=0; y < row; y++ )
	{
		for ( x=0; x < col; x++ )
		{
			//printf("actor tileset c %i r %i w %i h %i\n", x*w, y*h, w, h);
			sf::Texture tmptex;
			tmptex.setSmooth(false);
			tmptex.loadFromImage( STATE.TILEIMG[file], sf::IntRect(x*w, y*h, w, h) );
			Actor::tileset.push_back(tmptex);
			c++;
		}
	}
	if ( DEBUG ) printf(">> found %d tile for actor\n", c );
}

// common update function for both player and npc
void Actor::update()
{
	Actor::is_walk		= false;
	Actor::is_shot		= false;

	Object::update_rect();
	Actor::contact_topwall		= ( Actor::collusion_wall_y(1,-1) > 0 ) ? false : true;
	Actor::contact_bottomwall	= ( Actor::collusion_wall_y(1, 1) > 0 ) ? false : true;
	Actor::contact_leftwall		= ( Actor::collusion_wall_x(1,-1) > 0 ) ? false : true;
	Actor::contact_rightwall	= ( Actor::collusion_wall_x(1, 1) > 0 ) ? false : true;

	( Actor::countdown_invicible > 0 ) ? Actor::countdown_invicible-- : Actor::countdown_invicible = 0;
	( Actor::countdown_shoot     > 0 ) ? Actor::countdown_shoot--     : Actor::countdown_shoot     = 0;
	( Actor::countdown_action    > 0 ) ? Actor::countdown_action--    : Actor::countdown_action    = 0;

	( Actor::countdown_jump > 0 ) ? Actor::countdown_jump -=  Actor::mv_speed_y      : Actor::countdown_jump = 0;
	( Actor::countdown_dash > 0 ) ? Actor::countdown_dash -= (Actor::mv_speed_x * 3) : Actor::countdown_dash = 0;

	Actor::is_invicible = ( Actor::countdown_invicible > 0 ) ? true : false;

	// CANNOT AUTOSET, not while still holding the button
	//Actor::is_action    = ( Actor::countdown_action    > 0 ) ? true : false;
	//Actor::is_dash      = ( Actor::countdown_dash      > 0 ) ? true : false;
	//Actor::is_jump      = ( Actor::countdown_jump      > 0 ) ? true : false;
}

void Actor::update_state(int st, int max, int loop_end, int loop_begin)
{
	//printf("st %i prev %i curr %i\n", st, Actor::prev_state, Actor::curr_state);
	// st is to compare with prev_state to calculate action_frame
	// curr_state is for drawing
	// max = 10, array = 0-9

	// stuck is for action like dashing
	// first frame startup, stuck at second frame, third frame when change to another state

	// same state, add action frame
	// if stuck, stop at that frame until change state
	// if not, loop the action frame
	if ( Actor::prev_state == st )
	{
		if ( loop_end > 0 )
		{
			if ( Actor::action_frame >= loop_end )
				Actor::action_frame = loop_end;
			else
				Actor::action_frame++;
		}
		else
		{
			if ( Actor::action_frame < max )
				Actor::action_frame++;
			else
				Actor::action_frame = loop_begin;
		}
	}

	// different state, reset action frame
	// if stuck, finish the action frame first before changing
	// if not, reset and update
	else
	{
		if ( loop_end > 0 )
		{
			if ( Actor::action_frame < max )
				Actor::action_frame++;
			else
			{
				Actor::prev_state = Actor::curr_state;
				Actor::curr_state = st;
				Actor::action_frame = 0;
			}
		}
		else
		{
			Actor::prev_state = Actor::curr_state;
			Actor::curr_state = st;
			Actor::action_frame = 0;
		}
	}

}

void Actor::draw()
{
	int x = Object::pos_x - STATE.CAMERA.cam_x;
	int y = Object::pos_y - STATE.CAMERA.cam_y;

	sf::Sprite spr;
	spr.setTexture( Actor::tileset[ Actor::draw_frame ] );

	// ALL SPRITE FACING RIGHT!!!
	// flip the image to face left
	if ( Actor::is_left )
		spr.setTextureRect( sf::IntRect(Object::tile_width, 0, Object::tile_width * -1, Object::tile_height) );

	spr.setPosition( x, y );
	STATE.SCREEN.draw( spr );
}
//----------------------------------------------------------------
// standard actions
//----------------------------------------------------------------
void Actor::jump()
{
	// lots of jump check
	//
	// situation - jumping, but before fall down (or in middle of), the char can jump again...
	// reason - countdown_jump only track how high the char jump before falling
	// counter - use bool jumping to make sure the char completely landed first
	//
	// situation - jumping, but before fall down (or in middle of), try jump again causes the char to hover
	// reason - no default action when countdown_jump less than 0
	// counter - force the char to either mv_up or mv_down, depending on countdown_jump
	//
	// situation - standing, walk off a cliff, the char can jump again
	// reason - no checking of bottomwall to disable jump (or set jumping to true)
	// counter - made function collusion_wall_y to check, standardized mv_up and mv_down
	//
	// situation - jumping, when the ceiling is low, jump again causes the char to hover until timeout
	// reason - no checking of topwall to set countdown_jump to > 0
	// counter - made function collusion_wall_y to check, standardized mv_up and mv_down
	//

	if ( Actor::collusion_wall_y(1,-1) == 0 )
		Actor::countdown_jump = -10;
	if ( Actor::collusion_wall_y(1, 1) > 0 )
		Actor::is_jump = true;

	// jumping, don't allow jump again
	if ( Actor::is_jump )
	{
		Actor::countdown_jump = Actor::countdown_jump - Actor::mv_speed_y;

		// check if jump up or jump down
		if ( Actor::countdown_jump > 0 )
			Actor::mv_up();
		else
			Actor::mv_down();
	}
	// on floor, allow jump action
	else
	{
		Actor::countdown_jump = 80;
		Actor::is_jump = true;
	}
}

void Actor::leftjump()	{ Actor::jump(); Actor::mv_left();  }
void Actor::rightjump()	{ Actor::jump(); Actor::mv_right(); }
//----------------------------------------------------------------
// object collusion
//----------------------------------------------------------------
int Actor::collusion_wall_x(int speed, int adj)
{
	int i, t, u, b;
	int mv = 0;
	int hor = 0;
	bool is_wall = false;
	for ( t=1; t <= speed; t++)
	{
		if ( adj == 0 )		return adj;
		if ( adj > 0 )
		{
			if ( ( Actor::rect_right + t - WIDTH ) > STATE.CAMERA.cam_max_x )
				return t-1;
			hor = ( Actor::rect_right + t ) / TILE_W;
		}
		if ( adj < 0 )
		{
			if ( ( Actor::rect_left - t ) < STATE.CAMERA.cam_min_x )
				return t-1;
			hor = ( Actor::rect_left - t ) / TILE_W;
		}

		u = ( Actor::rect_top )  / TILE_H;
		b = ( Actor::rect_bottom ) / TILE_H;
		for ( i=u; i<=b; i++ )
		{
			if ( STATE.MAP_FG_OBJECT[i][hor]->is_wall )
				is_wall = true;
		}
		if ( !is_wall ) mv++;
	}
	return mv;
}

int Actor::collusion_wall_y(int speed, int adj)
{
	int i, t, l, r;
	int mv = 0;
	int ver = 0;
	bool is_wall = false;
	for ( t=1; t <= speed; t++)
	{
		if ( adj == 0 )		return adj;
		if ( adj > 0 )		ver = ( Actor::rect_bottom + t ) / TILE_H;
		if ( adj < 0 )
		{
			if ( ( Actor::rect_top - t ) < STATE.CAMERA.cam_min_y )
				return t-1;
			ver = ( Actor::rect_top - t ) / TILE_H;
		}

		l = ( Actor::rect_left )  / TILE_W;
		r = ( Actor::rect_right ) / TILE_W;
		for ( i=l; i<=r; i++ )
		{
			if ( STATE.MAP_FG_OBJECT[ver][i]->is_wall )
				is_wall = true;
		}
		if ( !is_wall ) mv++;
	}
	return mv;
}

void Actor::mv_up(int mv)
{
	int move = ( mv > 0 ) ? mv : Actor::mv_speed_y;
	int dist = Actor::collusion_wall_y( move, -1 );
	Actor::pos_y -= dist;
	Actor::rect_top -= dist;
	if ( dist != move )
		Actor::contact_topwall = true;
}
void Actor::mv_down(int mv)
{
	int move = ( mv > 0 ) ? mv : Actor::mv_speed_y;
	int dist = Actor::collusion_wall_y( move,  1 );
	Actor::pos_y += dist;
	Actor::rect_bottom += dist;
	if ( dist != move )
		Actor::contact_bottomwall = true;
}
void Actor::mv_left(int mv)
{
	int move = ( mv > 0 ) ? mv : Actor::mv_speed_x;
	int dist = Actor::collusion_wall_x( move, -1 );
	Actor::pos_x -= dist;
	Actor::rect_left -= dist;
	if ( dist != move )
		Actor::contact_leftwall = true;
}
void Actor::mv_right(int mv)
{
	int move = ( mv > 0 ) ? mv : Actor::mv_speed_x;
	int dist = Actor::collusion_wall_x( move,  1 );
	Actor::pos_x += dist;
	Actor::rect_right += dist;
	if ( dist != move )
		Actor::contact_rightwall = true;
}
//----------------------------------------------------------------
