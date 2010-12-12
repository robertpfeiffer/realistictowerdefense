#include <string>
#include <sstream>
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

char Map::getField(unsigned int x, unsigned int y)
{
	return Fields[y][x];
}

void Map::loadMap()
{
	//Load dimension of map
	Width  = Ini.GetLongValue("Size", "Width", 10);
	Height = Ini.GetLongValue("Size", "Height", 10);

	//prepare field 2d vector
	Fields.resize(Height);
	for (unsigned int y = 0; y < Height; y++)
	{
		//allow up to 999.999 rows
		char rowIdx[10];
		sprintf_s(rowIdx, 10, "Row%d", y+1);
		Fields[y].assign(Ini.GetValue("Fields", rowIdx, ""));
		Fields[y].resize(Width, INI_FIELD_GRAS);
	}
}