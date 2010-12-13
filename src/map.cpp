#include "map.h"
#include <string>
#include <sstream>
#include <SimpleIni.h>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#ifdef _MSC_VER
#define snprintf sprintf_s
#endif


Map::Map(const std::string& filename)
{
	Ini.LoadFile(filename.c_str());

	loadMap();
	loadTextures();
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
		snprintf(rowIdx, 10, "Row%d", y+1);
		Fields[y].assign(Ini.GetValue("Map", rowIdx, ""));
		Fields[y].resize(Width, INI_FIELD_GRAS);
	}
}

void Map::loadTextures()
{
	std::string sad = Ini.GetValue("Textures", "Tex0", "sd");
	int textureCount = Ini.GetSectionSize("Textures");
	if (textureCount == -1)
	{
		textureCount = 0;
	}

	Textures.resize(textureCount);
	for (int i = 0; i < textureCount; i++)
	{
		char texIdx[10];
		sprintf_s(texIdx, 10, "Tex%d", i+1);
		std::string tex = "textures/";
		tex.append(Ini.GetValue("Textures", texIdx, ""));

		osg::Image* image = osgDB::readImageFile(tex.c_str());
		if (image != NULL)
		{
			Textures[i] = new osg::Texture2D(image);
			Textures[i]->setDataVariance(osg::Object::STATIC);		
		}
		else
		{
			printf("Could not load texture \"%s\"", tex.c_str());
		}
	}
}
