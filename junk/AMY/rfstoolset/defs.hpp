#ifndef RFS_DEFS
#define RFS_DEFS

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <cmath>
#include <cctype>
#include <cerrno>
#include <vector>
#include <map>
#include <queue>
#include <deque>
#include <sstream>
#include <fstream>
#include <iomanip>

typedef unsigned int   uint;
typedef unsigned char  uchar;
typedef std::string    ascii;
typedef std::vector <  int  >   intvect;
typedef std::vector < uint  >  uintvect;
typedef std::vector < uchar >  charvect;
typedef std::vector < ascii >   strvect;
typedef std::deque  <  int  >   intdeq;
typedef std::deque  < uint  >  uintdeq;
typedef std::deque  < uchar >  chardeq;
typedef std::deque  < ascii >   strdeq;
typedef std::map    < int,  int  >  intintmap;
typedef std::map    < int, ascii >  intstrmap;

// rainbow colors
// from http://simple.wikipedia.org/wiki/Rainbow
#define C_NOIR     0x000000ff
#define C_WEISS    0xffffffff
#define C_ROSSO    0xff0000ff
#define C_ORANGE   0xff7f00ff
#define C_GIALLO   0xffff00ff
#define C_VERDE    0x00ff00ff
#define C_AZUL     0x0000ffff
#define C_INDIGO   0x4b0082ff
#define C_VIOLA    0x8f00ffff
#define C_CLEAR    0x00000000

// en = black,   white,  red,      orange,    yellow,   green, blue, indigo, violent
// de = schwarz, weiss,  rot,      orange,    gelb,     gruen, blau, indigo, violett
// fr = noir,    blanc,  rouge,    orange,    jaune,    vert,  bleu, indigo, violet
// it = nero,    bianco, rosso,    arancione, giallo,   verde, blu,  indaco, viola
// es = negro,   blanco, rojo,     naranja,   amarillo, verde, azul, anil,   violeta
// pt = preto,   branco, vermelho, laranja,   amarelo,  verde, azul, anil,   violeta

#endif
