#include "define.h"
using namespace amy;
extern State STATE;

Camera::Camera() {}
Camera::~Camera() {}

void Camera::update()
{
	Camera::draw_adjust_x = ( Camera::cam_x % TILE_W ) * -1;
	Camera::draw_adjust_y = ( Camera::cam_y % TILE_H ) * -1;

	// adjust the camera to display area first
	if ( Camera::cam_x < Camera::cam_min_x
		|| Camera::cam_x > Camera::cam_max_x )
		Camera::mv_pos_x( Camera::cam_min_x - Camera::cam_x );

	if ( Camera::cam_y < Camera::cam_min_y
		|| Camera::cam_y > Camera::cam_max_y )
		Camera::mv_pos_y( Camera::cam_min_y - Camera::cam_y );

}

void Camera::update_player()
{
	// adjust the camera to player pos
	Camera::mv_pos_x( STATE.PLAYER->center_x - Camera::cam_x - HALF_WIDTH  );
	Camera::mv_pos_y( STATE.PLAYER->center_y - Camera::cam_y - HALF_HEIGHT );
}

void Camera::set_range(int left, int top, int right, int bottom)
{
	Camera::cam_min_x = left * TILE_W;
	Camera::cam_max_x = (right+1) * TILE_W - WIDTH;
	Camera::cam_min_y = top * TILE_H;
	Camera::cam_max_y = (bottom+1) * TILE_H - HEIGHT;

	Camera::tile_min_x = left;
	Camera::tile_max_x = right;
	Camera::tile_min_y = top;
	Camera::tile_max_y = bottom;
}

void Camera::set_pos(int x, int y)
{
	Camera::cam_x = x;
	Camera::cam_y = y;
}

void Camera::set_speed(int spd_x, int spd_y)
{
	Camera::cam_spd_x = spd_x;
	Camera::cam_spd_y = spd_y;
}

void Camera::mv_pos_x(int dist)
{
	if ( dist > 0 )			Camera::mv_right(dist);
	else if ( dist < 0 )	Camera::mv_left (dist * -1);
}
void Camera::mv_pos_y(int dist)
{
	if ( dist > 0 )			Camera::mv_down(dist);
	else if ( dist < 0 )	Camera::mv_up  (dist * -1);
}

void Camera::mv_up(int dist, int spd)
{
	int i, test;
	int speed = ( spd > 1 ) ? spd : Camera::cam_spd_y;
	int max = ( dist > speed ) ? speed : dist;
	for ( i=1; i <= max; i++ )
	{
		test = Camera::cam_y - 1;
		if ( test >= Camera::cam_min_y )
			Camera::cam_y--;
	}
}
void Camera::mv_down(int dist, int spd)
{
	int i, test;
	int speed = ( spd > 1 ) ? spd : Camera::cam_spd_y;
	int max = ( dist > speed ) ? speed : dist;
	for ( i=1; i <= max; i++ )
	{
		test = Camera::cam_y + 1;
		if ( test <= Camera::cam_max_y )
			Camera::cam_y++;
	}
}
void Camera::mv_left(int dist, int spd)
{
	int i, test;
	int speed = ( spd > 1 ) ? spd : Camera::cam_spd_x;
	int max = ( dist > speed ) ? speed : dist;
	for ( i=1; i <= max; i++ )
	{
		test = Camera::cam_x - 1;
		if ( test >= Camera::cam_min_x )
			Camera::cam_x--;
	}
}
void Camera::mv_right(int dist, int spd)
{
	int i, test;
	int speed = ( spd > 1 ) ? spd : Camera::cam_spd_x;
	int max = ( dist > speed ) ? speed : dist;
	for ( i=1; i <= max; i++ )
	{
		test = Camera::cam_x + 1;
		if ( test <= Camera::cam_max_x )
			Camera::cam_x++;
	}
}
