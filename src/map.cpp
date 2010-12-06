#include "map.h"
#include "SimpleIni.h"
#include <string>

Map::Map(const std::string& filename)
{
	Width = 5;
}

unsigned int Map::GetWidth()
{
	return Width;
}

unsigned int Map::GetHeight()
{
	return Height;
}