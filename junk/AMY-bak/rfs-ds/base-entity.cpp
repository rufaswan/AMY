#include "header.hpp"
using namespace amy;
extern PlayData DATA;

Entity::Entity( int pos_x, int pos_y, bool is_left )
{
	Entity::base_x  = pos_x;
	Entity::base_y  = pos_y;
	Entity::is_left = is_left;
	Entity::velocity_x = 0;
	Entity::velocity_y = 0;
}
Entity::~Entity() {}
void Entity::update_ctrl() {}
void Entity::update_npc ( bool AI ) {}
void Entity::draw( sf::RenderTexture &preN, sf::RenderTexture &preD ) {}

void Entity::set_tileset( const std::string &file, int tw, int th )
{
	//printf(">> Entity::set_tileset( %s, %i, %i )", file.c_str(), tw, th );
	DATA.sfimage = DATA.file.data[ file ].get_sfimage();
	DATA.utils.set_tileset( DATA.sfimage, TILE_W, TILE_H, Entity::tileset );
}
