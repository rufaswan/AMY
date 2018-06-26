#ifndef ENTITYTOOLS_HPP
#define ENTITYTOOLS_HPP

#include "class_entitydata.hpp"

typedef  std::vector < EntityData* >   EntityList;

void loop_entity( EntityData* host, EntityData* enem )
{
	if ( host->m_is_hitted > 0 )   return;
	if ( enem->m_is_hitted > 0 )   return;

	host->collide( enem );
}

void loop_entity( EntityList &lst, EntityData* en )
{
	int i = lst.size();
	int dist;
	while ( i > 0 )
	{
		i--;
		loop_entity( lst[i], en );
		if ( lst[i]->m_is_dead && ! lst[i]->is_onscreen() )
		{
			delete lst[i];
			lst.erase( lst.begin() + i );
		}
	}
}

void loop_entity( EntityList &lst1, EntityList &lst2 )
{
	int i = lst1.size();
	int j = lst2.size();
	while( i > 0 )
	{
		i--;
		if ( lst1[i]->m_is_dead )
		{
			if ( ! lst1[i]->is_onscreen() )
			{
				delete lst1[i];
				lst1.erase( lst1.begin() + i );
				continue;
			}
		}
		else
		{
			while( j > 0 )
			{
				j--;
				loop_entity( lst1[i], lst2[j] );
			} // while j > 0
		}
	} // while i > 0
}

void move_entity( EntityList &from, int from_no, EntityList &to )
{
	if ( from_no >= from.size() )  return;

	to.push_back( from[ from_no ] );
	from.erase( from.begin() + from_no );
}

void clear_entities( EntityList &enlist, bool clear_all = false )
{
	int n = enlist.size();
	while ( n > 0 )
	{
		n--;
		if ( clear_all || enlist[n]->m_is_dead )
		{
			delete enlist[n];
			enlist.erase( enlist.begin() + n );
		}
	}
}

void draw_player_pos( gamesys* sys, sf::Font &font, EntityData* entity )
{
	char fps[20];
		rfs::memset( fps, 20 );
		sprintf(fps, "POS %d,%d", entity->m_pos_x, entity->m_pos_y);

	sf::Text   text(fps, font);
		text.setCharacterSize( sys->m_h / 16 );
	sys->m_win.draw(text);
}

bool has_enlist_id( int id, EntityList &EnList )
{
	int l;
	for ( l=0; l < EnList.size(); l++ )
	{
		if ( id == EnList[l]->m_id )
			return true;
	}
	return false;
}

#endif //  ENTITYTOOLS_HPP
