#include "header.hpp"
using namespace amy;

FileData::FileData()
{
	FileData::ready = false;
}
FileData::~FileData() {}

bool FileData::loadFromFile( const std::string &file )
{
	printf(">> FileData::loadFromFile( %s )\n", file.c_str() );
	std::fstream fs;
	uint i;

	fs.open(file.c_str(), fs.in | fs.binary);
	if ( !fs.good() )
		return false;
	printf("fopen OK... ");

	fs.seekg (0, fs.end);
	FileData::size = fs.tellg();
	fs.seekg (0, fs.beg);

	// copy the file into the buffer:
	FileData::buffer.clear();
	FileData::buffer.resize( FileData::size );
	for ( i=0; i < FileData::size; i++ )
		FileData::buffer[i] = fs.get();
	printf("read %i of %i bytes OK... ", i, FileData::size);

	// All Done!
	printf("Done!\n");
	fs.close();

	FileData::ready = true;
	return true;
}

void* FileData::get_data() { return ( FileData::ready ) ? (void*)&FileData::buffer[0] : NULL; }

sf::Image FileData::get_sfimage()
{
	sf::Image data;
	if ( FileData::ready )
		data.loadFromMemory( FileData::get_data(), FileData::size );
	return data;
}
sf::Font FileData::get_sffont()
{
	sf::Font data;
	if ( FileData::ready )
		data.loadFromMemory( FileData::get_data(), FileData::size );
	return data;
}
sf::SoundBuffer FileData::get_sfsndbuf()
{
	sf::SoundBuffer data;
	if ( FileData::ready )
		data.loadFromMemory( FileData::get_data(), FileData::size );
	return data;
}
