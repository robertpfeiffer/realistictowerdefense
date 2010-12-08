#include <string>
#include <SimpleIni.h>
#include "map.h"

Map::Map(const std::string& filename)
{
	CSimpleIni Ini(false, false, false);
	Ini.LoadFile(filename.c_str());

	Width  = Ini.GetLongValue("Size", "Width", 10);
	Height = Ini.GetLongValue("Size", "Height", 10);
}

unsigned int Map::GetWidth()
{
	return Width;
}

unsigned int Map::GetHeight()
{
	return Height;
}