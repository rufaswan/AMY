#ifndef DEFINE_H
#define DEFINE_H

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>
#include <map>
#include <queue>
#include <deque>
#include <sstream>
#include <fstream>
#include <iomanip>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#define WIDTH  (256)
#define HEIGHT (224)
#define FPS    (24)
#define TILE_W (16)
#define TILE_H (16)

// rainbow colors
// from http://simple.wikipedia.org/wiki/Rainbow
#define BLACK    sf::Color(  0,  0,  0)
#define WHITE    sf::Color(255,255,255)
#define RED      sf::Color(255,  0,  0)
#define ORANGE   sf::Color(255,127,  0)
#define YELLOW   sf::Color(255,255,  0)
#define GREEN    sf::Color(  0,255,  0)
#define BLUE     sf::Color(  0,  0,255)
#define INDIGO   sf::Color( 75,  0,130)
#define VIOLENT  sf::Color(143,  0,255)

typedef unsigned int   uint;
typedef unsigned char  uchar;
typedef std::vector < int  >  vect_i;
typedef std::vector < char >  vect_c;
typedef std::vector < uint  > vect_ui;
typedef std::vector < uchar > vect_uc;
typedef std::vector < sf::Texture > vect_tex;

#include "data-utils.hpp"
#include "data-timer.hpp"
#include "data-rect.hpp"

#include "base-bg.hpp"
#include "bg-static.hpp"      // *Background
#include "bg-follow.hpp"      // *Background
#include "bg-forced.hpp"      // *Background

#include "base-recorder.hpp"
#include "rec-raw.hpp"        // *Recorder
#include "rec-ogg.hpp"        // *Recorder

#include "base-display.hpp"   // Timer
#include "sys-gmaster.hpp"    // Display

#include "sys-tiles.hpp"
#include "sys-world.hpp"
#include "data-file.hpp"
#include "sys-fileload.hpp"   // FileData
#include "data-key.hpp"
#include "sys-input.hpp"      // KeyData
#include "base-entity.hpp"    // RectData, Timer
#include "sys-camera.hpp"     // RectData

#include "sys-objects.hpp"
#include "sys-playlist.hpp"
#include "base-events.hpp"
#include "sys-playdata.hpp"   // Input, Camera, Events*, FileLoad, World, Tiles, Playlist, Recorder*

#include "dis-x1intro.hpp"
#include "dis-x3seahorse.hpp"
#include "dis-x1stgslt.hpp"

#endif // DEFINE_H
