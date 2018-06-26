#include "define.h"
using namespace amy;
extern State STATE;

Object::Object()
{
	Object::set_pos(0, 0);
	Object::set_basepos(0, 0);
	Object::set_defaults();
	Object::update_rect();
}
Object::~Object() {}

void Object::set_defaults()
{
	Object::is_incamera	= false;
	Object::is_onhold	= false;
	Object::char_width  = TILE_W;
	Object::char_height = TILE_H;
	Object::rect_adj_x = 0;
	Object::rect_adj_y = 0;
	Object::mv_speed_x = 1;
	Object::mv_speed_y = 1;
}

// checkpoint or respawn point
void Object::set_basepos(int x, int y)
{
	Object::base_pos_x = x;
	Object::base_pos_y = y;
	Object::set_pos(x, y);
}

// teleport (8 boss rematch)
void Object::set_pos(int x, int y)
{
	Object::pos_x = x;
	Object::pos_y = y;
	Object::update_rect();
}

// given center pos, reverse calculate the pos and rect
void Object::center2pos(int cen_x, int cen_y)
{
	Object::pos_x = cen_x - ( Object::char_width  / 2 ) - Object::rect_adj_x;
	Object::pos_y = cen_y - ( Object::char_height / 2 ) - Object::rect_adj_y;
	Object::update_rect();
}

void Object::update_rect()
{
	Object::rect_top			= Object::pos_y + Object::rect_adj_y;
	Object::rect_bottom			= Object::pos_y + Object::rect_adj_y + Object::char_height - 1;
	Object::rect_left			= Object::pos_x + Object::rect_adj_x;
	Object::rect_right			= Object::pos_x + Object::rect_adj_x + Object::char_width - 1;

	Object::char_rect.set_rect(Object::rect_left, Object::rect_top, Object::rect_right, Object::rect_bottom);

	Object::center_x = Object::pos_x + Object::rect_adj_x + ( Object::char_width  / 2 );
	Object::center_y = Object::pos_y + Object::rect_adj_y + ( Object::char_height / 2 );
}

void Object::draw()
{
	sf::Sprite spr;
	int x = Object::pos_x - STATE.CAMERA.cam_x;
	int y = Object::pos_y - STATE.CAMERA.cam_y;

	spr.setTexture( Object::char_texture );
	spr.setPosition( x, y );
	STATE.SCREEN.draw( spr );
}

void Object::force_mv_up   (int mv)
{
	int move = ( mv > 0 ) ? mv : Object::mv_speed_y;
	Object::pos_y -= move;
}
void Object::force_mv_down (int mv)
{
	int move = ( mv > 0 ) ? mv : Object::mv_speed_y;
	Object::pos_y += move;
}
void Object::force_mv_left (int mv)
{
	int move = ( mv > 0 ) ? mv : Object::mv_speed_x;
	Object::pos_x -= move;
}
void Object::force_mv_right(int mv)
{
	int move = ( mv > 0 ) ? mv : Object::mv_speed_x;
	Object::pos_x += move;
}
