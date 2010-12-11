#include <string>
#include <SimpleIni.h>
#include "map.h"

Map::Map(const std::string& filename)
{
	Ini.LoadFile(filename.c_str());

	loadMap();
}

unsigned int Map::getWidth()
{
	return Width;
}

unsigned int Map::getHeight()
{
	return Height;
}

void Map::loadMap()
{
	Width  = Ini.GetLongValue("Size", "Width", 10);
	Height = Ini.GetLongValue("Size", "Height", 10);
}