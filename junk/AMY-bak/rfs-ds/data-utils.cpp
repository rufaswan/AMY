#include "header.hpp"
using namespace amy;

Utils::Utils() {}
Utils::~Utils() {}

void Utils::fopen( std::fstream &fs, const std::string &file, const std::string &mode )
{
	std::fstream fp;

	if ( mode == "r" )
		fs.open(file.c_str(), fp.in);
	else if ( mode == "rb" )
		fs.open(file.c_str(), fp.in | fp.binary);
	else if ( mode == "r+" )
		fs.open(file.c_str(), fp.in | fp.out);
	else if ( mode == "rb+" )
		fs.open(file.c_str(), fp.in | fp.out | fp.binary);

	else if ( mode == "w" )
		fs.open(file.c_str(), fp.out | fp.trunc);
	else if ( mode == "wb" )
		fs.open(file.c_str(), fp.out | fp.binary | fp.trunc);
	else if ( mode == "w+" )
		fs.open(file.c_str(), fp.in | fp.out | fp.trunc);
	else if ( mode == "wb+" )
		fs.open(file.c_str(), fp.in | fp.out | fp.binary | fp.trunc);

	else if ( mode == "a" )
		fs.open(file.c_str(), fp.out | fp.ate | fp.app);
	else if ( mode == "ab" )
		fs.open(file.c_str(), fp.out | fp.binary | fp.ate | fp.app);
	else if ( mode == "a+" )
		fs.open(file.c_str(), fp.in | fp.out | fp.app | fp.ate);
	else if ( mode == "ab+" )
		fs.open(file.c_str(), fp.in | fp.out | fp.app | fp.ate | fp.binary);

}

std::string Utils::int2str( int i, uint zeros )
{
	std::stringstream ss;
	ss << std::setw( zeros ) << std::setfill('0') << i;
	return ss.str();
}

void Utils::set_tileset( sf::Image &sfimg, int tw, int th, vect_tex &vect )
{
	sf::Vector2u imgsize( sfimage.getSize() );
	uint col = imgsize.x / tw;
	uint row = imgsize.y / th;
	uint i, j, k;

	vect.clear();
	vect.resize( col * row );
	k = 0;
	for ( i=0; i < row; i++ )
	{
		for ( j=0; j < col; j++ )
		{
			sf::Texture tmptex;
			tmptex.setSmooth(false);
			tmptex.loadFromImage( sfimage, sf::IntRect( j*tw, i*th, tw, th) );

			vect[k] = tmptex;
			k++;
		}
	}
}
