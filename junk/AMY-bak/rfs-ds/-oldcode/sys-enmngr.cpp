#include "header.hpp"
using namespace amy;
extern PlayData DATA;

EnManager::EnManager()
{
	EnManager::objectid   = 0;
	EnManager::has_player = false;
}
EnManager::~EnManager() {}

void EnManager::add( amy::Entity *en, uint type )
{
	if ( type == EnManager::PLAYER )
	{
		if ( EnManager::has_player )
			printf("EnManager::has_player !\n");
		else
		{
			EnManager::player = en;
			EnManager::has_player = true;
		}
	}
	else
	{
		uint id = EnManager::get_objectid();
		EnManager::objects[ id ] = en;
	}
}

void EnManager::update()
{
	if ( EnManager::has_player )
		EnManager::player->update( DATA.world );

	if ( EnManager::objects.size() > 0 )
	{
		for ( EnManager::enit = EnManager::objects.begin();
			EnManager::enit != EnManager::objects.end();
			++EnManager::enit )
				EnManager::objects[ EnManager::enit->first ]->update_npc( DATA.world );
	}
}

void EnManager::draw( sf::RenderTexture &preN, sf::RenderTexture &preD )
{
	if ( EnManager::has_player )
		EnManager::player->draw( preN, preD );

	if ( EnManager::objects.size() > 0 )
	{
		for ( EnManager::enit = EnManager::objects.begin();
			EnManager::enit != EnManager::objects.end();
			++EnManager::enit )
				EnManager::objects[ EnManager::enit->first ]->draw( preN, preD );
	}
}

void EnManager::clear()
{
	if ( EnManager::has_player )
		delete EnManager::player;

	if ( EnManager::objects.size() > 0 )
	{
		for ( EnManager::enit = EnManager::objects.begin();
			EnManager::enit != EnManager::objects.end();
			++EnManager::enit )
				delete EnManager::objects[ EnManager::enit->first ];
	}

	EnManager::player = NULL;
	EnManager::objects.clear();
}

uint EnManager::get_objectid()
{
	bool done = false;
	while ( !done )
	{
		// not found
		if ( EnManager::objects.find( EnManager::objectid ) == EnManager::objects.end() )
			done = true;
		else
			EnManager::objectid++;
	}
	return EnManager::objectid;
}

