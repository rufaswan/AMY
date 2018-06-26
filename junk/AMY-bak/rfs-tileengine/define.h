#ifndef DEFINE_H
#define DEFINE_H

#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <typeinfo>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
//----------------------------------------------------------------
#define DEBUG		true

#define TILE_W		16
#define TILE_H		16
#define WIDTH		(16 * TILE_W)
#define HEIGHT		(14 * TILE_H)
#define HALF_WIDTH	(WIDTH  / 2)
#define HALF_HEIGHT	(HEIGHT / 2)

#define DEPTH		32
#define FPS			60

enum Buttons
{
	KEY_INV		= 0,
	KEY_UP		= 1,	KEY_DN		= 2,
	KEY_LF		= 4,	KEY_RT		= 8,
	KEY_STR		= 16,	KEY_SEL		= 32,
	KEY_L_TR	= 64,	KEY_R_TR	= 128,
	KEY_DSH		= 256,
	KEY_JMP		= 512,
	KEY_SHT		= 1024,
	KEY_RPD		= 2048
};

enum Stages
{
	XXXXX,  STG_TEST, STG_INTRO, STG_SECRET,
	STG_ME, STG_VE,   STG_EA, STG_MA, STG_JU, STG_SA, STG_NE, STG_UR, STG_PL,
	STG_Z1, STG_Z2,   STG_Z3, STG_Z4, STG_W1, STG_W2, STG_W3, STG_W4,
	STG_DEMO, STG_ENDING,
	SCR_TITLE, SCR_PASSWD, SCR_CREDIT
};
//----------------------------------------------------------------
#include "hpp/amy-ivect.hpp"
#include "hpp/amy-rect.hpp"
#include "hpp/amy-tile.hpp"
#include "hpp/amy-base-screen.hpp"
#include "hpp/cus-stages.hpp"
#include "hpp/amy-base-obj.hpp"
#include "hpp/cus-actors.hpp"
#include "hpp/amy-camera.hpp"
#include "hpp/amy-state.hpp"
//----------------------------------------------------------------
#endif // DEFINE_H
