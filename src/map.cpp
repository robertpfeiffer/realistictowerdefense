#include "map.h"
#include "constants.h"
#include <string>
#include <SimpleIni.h>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#ifdef _MSC_VER
	#define snprintf sprintf_s
#endif


Map::Map(const std::string& filename)
{
	_ini.LoadFile(filename.c_str());

	loadTextures();
	loadFields();
	loadMap();
}

long Map::getWidth()
{
	return _width;
}

long Map::getHeight()
{
	return _height;
}

FieldBlock* Map::getFieldBlock(unsigned int x, unsigned int y)
{
	return _fieldBlocks[_fields[y][x]];
}

void Map::loadTextures()
{
	int textureCount = _ini.GetSectionSize(MAP_SECTION_TEXTURES);
	if (textureCount == -1)
	{
		textureCount = 0;
	}

	_textures.resize(textureCount);
	for (int i = 0; i < textureCount; i++)
	{
		char texIdx[10];
		sprintf_s(texIdx, 10, MAP_KEY_TEXTURES_MASK, i+1);
		std::string tex = MAP_DIRECTORY_TEXTURES;
		tex.append(_ini.GetValue(MAP_SECTION_TEXTURES, texIdx, ""));

		osg::Image* image = osgDB::readImageFile(tex.c_str());
		if (image != NULL)
		{
			_textures[i] = new osg::Texture2D(image);
			_textures[i]->setMaxAnisotropy(AF_LEVEL);
			_textures[i]->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
			_textures[i]->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
			_textures[i]->setDataVariance(osg::Object::STATIC);
		}
	}
}

void Map::loadFields()
{
	for (int i = 0; i < sizeof(char); i++)
	{
		_fieldBlocks[i] = 0;
	}

	const CSimpleIni::TKeyVal * pSectionData = _ini.GetSection(MAP_SECTION_FIELDS);
	if (pSectionData) {
		for (CSimpleIni::TKeyVal::const_iterator iKeyVal = pSectionData->begin(); iKeyVal != pSectionData->end(); iKeyVal++) {
			std::string sectionKey = MAP_KEY_FIELDS_PREFIX;
			sectionKey.append(iKeyVal->first.pItem);
			loadFieldBlock(sectionKey.c_str(), *(iKeyVal->second));
		}
	}
}

void Map::loadFieldBlock(const char* sectionKey, unsigned char fieldBlockIndex)
{
	bool isBuildable  = _ini.GetLongValue(sectionKey, MAP_KEY_FIELD_BUILDABLE , 0) != 0;
	bool isAccessible = _ini.GetLongValue(sectionKey, MAP_KEY_FIELD_ACCESSIBLE, 0) != 0;
	long texureId	  = _ini.GetLongValue(sectionKey, MAP_KEY_FIELD_TEXTUREID, 1) - 1;

	FieldBlock* fieldBlock =  new FieldBlock(isBuildable, isAccessible, _textures[texureId]);

	_fieldBlocks[fieldBlockIndex] = fieldBlock;
}

void Map::loadMap()
{
	//Load dimension of map
	_width  = _ini.GetLongValue(MAP_SECTION_SIZE, MAP_KEY_SIZE_WIDTH, 0);
	_height = _ini.GetLongValue(MAP_SECTION_SIZE, MAP_KEY_SIZE_HEIGHT, 0);

	//prepare field 2d vector
	_fields.resize(_height);
	for (long y = 0; y < _height; y++)
	{
		//allow up to 999.999 rows
		char rowIdx[10];
		snprintf(rowIdx, 10, MAP_KEY_MAP_ROW_MASK, y+1);
		_fields[y].assign(_ini.GetValue(MAP_SECTION_MAP, rowIdx, ""));
		_fields[y].resize(_width, 0);
	}
}
