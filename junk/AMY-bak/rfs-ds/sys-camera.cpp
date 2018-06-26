#include "header.hpp"
using namespace amy;
extern PlayData DATA;

Camera::Camera()
{
	Camera::velocity_x = 20;
	Camera::velocity_y = 20;
}
Camera::~Camera() {}

void Camera::set_pos(int x, int y)
{
	Camera::pos_x = x;
	Camera::pos_y = y;
	Camera::set_rect();
}

void Camera::set_rect()
{
	Camera::rect.tl_setsize(Camera::pos_x, Camera::pos_y, WIDTH, HEIGHT);
}

void Camera::reset()
{
	Camera::pos_x = 0;
	Camera::pos_y = 0;
	Camera::set_rect();
}

// from (x,y) to (x,y)
// point-to-point, from (0,0) to (2,2)
void Camera::set_range(uint left, uint top, uint right, uint bottom)
{
	Camera::min_x = left;
	Camera::min_y = top;
	Camera::max_x = right;
	Camera::max_y = bottom;
}

// from top-left of (x,y) to bottom-right of (x,y)
// from top-left of (10,10) => (160,160)
// to bottom-right of (20,20) => (335,335)
void Camera::set_rangetile(uint left, uint top, uint right, uint bottom)
{
	Camera::min_x = left   * TILE_W;
	Camera::min_y = top    * TILE_H;
	Camera::max_x = ( ( right  + 1 ) * TILE_W ) - 1;
	Camera::max_y = ( ( bottom + 1 ) * TILE_H ) - 1;
}

void Camera::mv_up()
{
	uint limit = Camera::rect.top - Camera::min_y;
	uint valid = ( limit > Camera::velocity_y ) ? Camera::velocity_y : limit;
	Camera::pos_y -= valid;
	//if ( (Camera::rect.top - 1) >= Camera::min_y )
	//	Camera::pos_y--;
	Camera::set_rect();
}
void Camera::mv_down()
{
	uint limit = Camera::max_y - Camera::rect.bottom;
	uint valid = ( limit > Camera::velocity_y ) ? Camera::velocity_y : limit;
	Camera::pos_y += valid;
	//if ( (Camera::rect.bottom + 1) <= Camera::max_y )
	//	Camera::pos_y++;
	Camera::set_rect();
}
void Camera::mv_left()
{
	uint limit = Camera::rect.left - Camera::min_x;
	uint valid = ( limit > Camera::velocity_x ) ? Camera::velocity_x : limit;
	Camera::pos_x -= valid;
	//if ( (Camera::rect.left - 1) >= Camera::min_x )
	//	Camera::pos_x--;
	Camera::set_rect();
}
void Camera::mv_right()
{
	uint limit = Camera::max_x - Camera::rect.right;
	uint valid = ( limit > Camera::velocity_x ) ? Camera::velocity_x : limit;
	Camera::pos_x += valid;
	//if ( (Camera::rect.right + 1) <= Camera::max_x )
	//	Camera::pos_x++;
	Camera::set_rect();
}
