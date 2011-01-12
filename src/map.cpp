#include "map.h"
#include "constants.h"
#include <string>
#include <SimpleIni.h>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#ifdef _MSC_VER
	#define snprintf sprintf_s
#endif


Map::Map(const std::string& filename) : osg::Referenced()
{
	_ini.LoadFile(filename.c_str());

	loadTextures();
	loadModels();
	loadMap();
	loadWorld();
	loadCheckPoints();
}

long Map::getWidth()
{
	return _width;
}

long Map::getHeight()
{
	return _height;
}

Field* Map::getField(unsigned int x, unsigned int y)
{
	return _fields[y][x];
}

osg::Texture2D* Map::getStrataTexture()
{
	return _textures[_strataTextureId];
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
		sprintf_s(texIdx, 10, MAP_KEY_TEXTURE_MASK, i);
		std::string textureFilename = MAP_DIRECTORY_TEXTURES;
		textureFilename.append(_ini.GetValue(MAP_SECTION_TEXTURES, texIdx, ""));

		osg::Image* image = osgDB::readImageFile(textureFilename.c_str());
		if (image != NULL)
		{
			osg::Texture2D* texture = new osg::Texture2D(image);
			texture->setMaxAnisotropy(AF_LEVEL);
			texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
			texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
			texture->setDataVariance(osg::Object::STATIC);

			_textures[i] = texture;
		}
	}
}

void Map::loadModels()
{
	int modelCount = _ini.GetSectionSize(MAP_SECTION_MODELS);
	if (modelCount == -1)
	{
		modelCount = 0;
	}

	_models.resize(modelCount);
	for (int i = 0; i < modelCount; i++)
	{
		char modelIdx[10];
		sprintf_s(modelIdx, 10, MAP_KEY_MODEL_MASK, i);
		std::string modelFilename = MAP_DIRECTORY_MODELS;
		modelFilename.append(_ini.GetValue(MAP_SECTION_MODELS, modelIdx, ""));
		_models[i] =  osgDB::readNodeFile(modelFilename);
	}
}

void Map::loadFields()
{
	for (int i = 0; i <= MAXBYTE; i++)
	{
		_fieldTypes[i] = 0;
	}

	const CSimpleIni::TKeyVal * pSectionData = _ini.GetSection(MAP_SECTION_FIELDS);
	if (pSectionData) {
		for (CSimpleIni::TKeyVal::const_iterator iKeyVal = pSectionData->begin(); iKeyVal != pSectionData->end(); iKeyVal++) {
			std::string sectionKey = MAP_KEY_FIELDS_PREFIX;
			sectionKey.append(iKeyVal->first.pItem);
			loadFieldTypes(sectionKey.c_str(), *(iKeyVal->second));
		}
	}
}

void Map::loadFieldTypes(const char* sectionKey, unsigned char fieldBlockIndex)
{
	bool isBuildable  = _ini.GetLongValue(sectionKey, MAP_KEY_FIELD_BUILDABLE ,  0) != 0;
	bool isAccessible = _ini.GetLongValue(sectionKey, MAP_KEY_FIELD_ACCESSIBLE,  0) != 0;
	long texureId	  = _ini.GetLongValue(sectionKey, MAP_KEY_FIELD_TEXTUREID ,  0);
	long modelId	  = _ini.GetLongValue(sectionKey, MAP_KEY_FIELD_MODELID   , -1);	

	FieldType* fieldType;
	if (modelId > -1)
	{
		fieldType =  new FieldType(isBuildable, isAccessible, _textures[texureId].get(), _models[modelId].get());
	}else{
		fieldType =  new FieldType(isBuildable, isAccessible, _textures[texureId].get());
	}

	_fieldTypes[fieldBlockIndex] = fieldType;
}

void Map::loadMap()
{
	loadFields();

	//Load dimension of map
	_width  = _ini.GetLongValue(MAP_SECTION_SIZE, MAP_KEY_SIZE_WIDTH, 0);
	_height = _ini.GetLongValue(MAP_SECTION_SIZE, MAP_KEY_SIZE_HEIGHT, 0);

	//prepare field 2d vector
	_fields.resize(_height);
	for (long y = 0; y < _height; y++)
	{
		_fields[y].resize(_width);

		//allow up to 999.999 rows
		char rowIdx[10];
		snprintf(rowIdx, 10, MAP_KEY_MAP_ROW_MASK, y);
		std::string rowString = _ini.GetValue(MAP_SECTION_MAP, rowIdx, "");
		rowString.resize(_width, 0);

		for (long x = 0; x < _width; x++)
		{
			_fields[y][x] = new Field(_fieldTypes[rowString[x]].get());
		}
	}
}

void Map::loadWorld()
{
	_strataTextureId = _ini.GetLongValue(MAP_SECTION_WORLD, MAP_KEY_WORLD_STRATA, 0);
}

void Map::loadCheckPoints()
{
	_checkpoints.clear();

	int i = -1;
	do{
		i++;

		_checkpoints.resize(i+1);

		char pointName[10];

		snprintf(pointName, 10, MAP_KEY_CHECKPOINTS_POINT_X_MASK, i);
		_checkpoints[i].X = _ini.GetLongValue(MAP_SECTION_CHECKPOINTS, pointName, -1);

		snprintf(pointName, 10, MAP_KEY_CHECKPOINTS_POINT_Y_MASK, i);
		_checkpoints[i].Y = _ini.GetLongValue(MAP_SECTION_CHECKPOINTS, pointName, -1);
		
	}while ((_checkpoints[i].X > -1) && (_checkpoints[i].Y > -1));
	_checkpoints.pop_back();
}

std::vector<MapPoint>* Map::getCheckpoints()
{
	return &_checkpoints;
}

void Map::createFields()
{

}
