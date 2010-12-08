#include "map.h"
#include <SimpleIni.h>
#include <string>

Map::Map(const std::string& filename)
{
	CSimpleIni Ini(false, false, false);
	Ini.Load(filename);
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