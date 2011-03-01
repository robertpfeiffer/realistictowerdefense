// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include "map.h"
#include "constants.h"
#include "modeldata.h"
#include <string.h>
#include <osg/PositionAttitudeTransform>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <stdlib.h>

#include <creepattributes.h>
#include <wave.h>

#ifndef WIN32
	#include <strings.h>
	#define strcmpi strcasecmp
#endif

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;

Map::Map()
{
	_reset();
}

Map::Map(const std::string& filename)
{
	_load(filename);
}

std::vector<MapPoint>* Map::getCheckpoints()
{
	return &_checkpoints;
}

osg::Texture2D* Map::getStrataTexture()
{
	return _strata.get();
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

bool Map::_attrToBool(xml_attribute<>* attr, bool defaultValue) const
{
	if (attr == NULL) return defaultValue;
	char* str = attr->value();

	if (strcmp(str, "0") == 0) return false;
	if (strcmp(str, "1") == 0) return true;

	if (strcmpi(str, "false") == 0) return false;
	if (strcmpi(str, "true") == 0) return true;

	return defaultValue;
}

long Map::_attrToLong(xml_attribute<>* attr, long defaultValue) const
{
	if (attr == NULL) return defaultValue;
	char* str = attr->value();

	long val = atol(str);
	if (val == 0)
	{
		if (strcmp(str, "0") != 0) return defaultValue;
	}
	return val;	
}

float Map::_attrToFloat(xml_attribute<>* attr, float defaultValue) const
{
	if (attr == NULL) return defaultValue;
	char* str = attr->value();

	float val = atof(str);
	if (val == 0.0)
	{
		if (strcmp(str, "0.0") != 0) return defaultValue;
	}
	return val;	
}

void Map::_reset()
{
	//mark all textures as not used on current map
	{
		std::list< _cache< osg::ref_ptr<osg::Texture2D> > >::iterator it;
		for(it = _textureCache.begin(); it != _textureCache.end(); it++)
		{
			it->used = false;
		}
	}

	//mark all models as not used on current map
	{
		std::list< _cache< osg::ref_ptr<osg::Node> > >::iterator it;
		for(it = _modelCache.begin(); it != _modelCache.end(); it++)
		{
			it->used = false;
		}
	}

	_checkpoints.clear();

	for (int i = 0; i <= 255; i++)
	{
		_fieldTypes[i] = NULL;
	}

	_width = 0;
	_height = 0;
}

bool isTextureUnused(Map::_cache<osg::ref_ptr<osg::Texture2D> > item) {
    return !item.used;
}

bool isModelUnused(Map::_cache<osg::ref_ptr<osg::Node> > item) {
    return !item.used;
}

void Map::_cleanup()
{
      _textureCache.remove_if(isTextureUnused);
      _modelCache.remove_if(isModelUnused);

	for (int i = 0; i <= 255; i++)
	{
		_fieldTypes[i] = NULL;
	}
}

void Map::_load(const std::string& filename)
{
	_reset();

	file<> file(filename.c_str());
	_xml.parse<0>(file.data());

	xml_node<> *root = _xml.first_node("Map", 0, false);

	xml_node<> *child = root->first_node("Player", 0, false);
	if (child != NULL)
	{
		_loadPlayer(child);
	}

	child = root->first_node("Waves", 0, false);
	if (child != NULL)
	{
		_loadWaves(child);
	}

	child = root->first_node("Towers", 0, false);
	if (child != NULL)
	{
		_loadTowers(child);
	}

	child = root->first_node("Terrain", 0, false);
	if (child != NULL)
	{
		_loadTerrain(child);
	}

	child = root->first_node("Checkpoints", 0, false);
	if (child != NULL)
	{
		_loadCheckPoints(child);
	}

	_cleanup();
}

void Map::_loadPlayer(xml_node<> *node)
{
	_player.setLifes(_attrToLong(node->first_attribute("life", 0, false), 1));
	_player.setMoney(_attrToLong(node->first_attribute("money", 0, false), 0));
}

void Map::_loadWaves(xml_node<> *node)
{
	for(xml_node<> *creepWave = node->first_node("Wave", 0, false); creepWave; creepWave = creepWave->next_sibling("Wave", 0, false))
	{
		Wave* wave = new Wave(_attrToLong(creepWave->first_attribute("spawnoffset", 0, false), 0));
		for(xml_node<> *creep = creepWave->first_node("Creep", 0, false); creep; creep = creep->next_sibling("Creep", 0, false))
		{
			CreepAttributes* attributes = new CreepAttributes();
			xml_attribute<>* nameAttr = creep->first_attribute("name", 0, false);
			attributes->name = "";
			if (nameAttr != NULL)
			{
				attributes->name = nameAttr->value();
			}
			
			attributes->maxHealth = _attrToLong(creep->first_attribute("health", 0, false), 1);
			attributes->armor = _attrToLong(creep->first_attribute("armor", 0, false), 0);
			attributes->magicResistance = _attrToLong(creep->first_attribute("magicresist", 0, false), 0);
			attributes->speed = _attrToFloat(creep->first_attribute("speed", 0, false), 1);
			attributes->bounty = _attrToFloat(creep->first_attribute("bounty", 0, false), 0);
			attributes->spawnOffset = _attrToFloat(creep->first_attribute("spawnOffset", 0, false), 0);

			xml_attribute<>* modelAttr = creep->first_attribute("model", 0, false);
			attributes->model = NULL;
			if (modelAttr != NULL)
			{
				attributes->model = new osg::PositionAttitudeTransform();
				attributes->model->addChild(_getModel(modelAttr->value()));

				float scale = _attrToFloat(creep->first_attribute("scale", 0, false), 1);
				attributes->model->setScale(osg::Vec3d(scale, scale, scale));
			}

			wave->addCreeps(_attrToLong(creep->first_attribute("count", 0, false), 0), attributes);
		}
		_waves.push(wave);
	}
}

void Map::_loadTowers(xml_node<> *node)
{
	for(xml_node<> *child = node->first_node("Tower", 0, false); child; child = child->next_sibling("Tower", 0, false))
	{
		_towers.push_back(_getTowerAttributes(child));
	}
}

TowerAttributes* Map::_getTowerAttributes(xml_node<> *node)
{
	TowerAttributes* tower = new TowerAttributes();
	tower->cooldown = _attrToFloat(node->first_attribute("cooldown", 0, false), 1);
	tower->range = _attrToFloat(node->first_attribute("range", 0, false), 0);
	tower->cost = _attrToLong(node->first_attribute("cost", 0, false), 0);
	tower->height = _attrToFloat(node->first_attribute("shotheight", 0, false), 0);

	xml_attribute<>* nameAttr = node->first_attribute("name", 0, false);
	tower->name = "";
	if (nameAttr != NULL)
	{
		tower->name = nameAttr->value();
	}

	xml_attribute<>* modelAttr = node->first_attribute("model", 0, false);
	tower->model = NULL;
	if (modelAttr != NULL)
	{
		tower->model = new osg::PositionAttitudeTransform();
		tower->model->addChild(_getModel(modelAttr->value()));

		float scale = _attrToFloat(node->first_attribute("scale", 0, false), 1);
		tower->model->setScale(osg::Vec3d(scale, scale, scale));
	}

	//load projectile
	modelAttr = node->first_attribute("projectile", 0, false);
	tower->projectile.model = NULL;
	if (modelAttr != NULL)
	{
		tower->projectile.model = new osg::PositionAttitudeTransform();
		tower->projectile.model->addChild(_getModel(modelAttr->value()));

		float scale = _attrToFloat(node->first_attribute("projectilescale", 0, false), 1);
		tower->projectile.model->setScale(osg::Vec3d(scale, scale, scale));
	}

	tower->projectile.physicalDamage = _attrToLong(node->first_attribute("physicalDamage", 0, false), 0);
	tower->projectile.magicalDamage = _attrToLong(node->first_attribute("magicalDamage", 0, false), 0);
	tower->projectile.travelSpeed = _attrToLong(node->first_attribute("projectilespeed", 0, false), 1);

	for(xml_node<> *child = node->first_node("Tower", 0, false); child; child = child->next_sibling("Tower", 0, false))
	{
		tower->upgrades.push_back(_getTowerAttributes(child));
	}

	return tower;
}

void Map::_loadTerrain(xml_node<> *node)
{
	xml_node<> *child = node->first_node("Strata", 0, false);
	if (child != NULL)
	{
		_loadStrata(child);
	}

	child = node->first_node("Fields", 0, false);
	if (child != NULL)
	{
		_loadFieldTypes(child);
	}

	child = node->first_node("Grid", 0, false);
	if (child != NULL)
	{
		_loadGrid(child);
	}
}

void Map::_loadStrata(xml_node<> *node)
{
	xml_attribute<> *attr = node->first_attribute("texture", 0, false);
	if (attr != NULL)
	{
		_strata = _getTexture(attr->value());
	}
}

void Map::_loadFieldTypes(xml_node<> *node)
{
	for(xml_node<> *child = node->first_node("Field", 0, false); child; child = child->next_sibling("Field", 0, false))
	{
		osg::Texture2D* texture = NULL;
		xml_attribute<> *attr = child->first_attribute("texture", 0, false);
		if (attr != NULL)
		{
			texture = _getTexture(attr->value());
		}

		char symbol = 0;
		attr = child->first_attribute("symbol", 0, false);
		if (attr != NULL)
		{
			if (attr->value_size() > 0)
			{
				symbol = attr->value()[0];
			}
		}
		
		bool buildable = true;
		attr = child->first_attribute("buildable", 0, false);		
		buildable = _attrToBool(attr, true);
		
		ModelData* modelData = NULL;
		xml_node<> *modelNode= child->first_node("Model", 0, false);
		if (modelNode != NULL)
		{
			modelData = _readModel(modelNode);
		}

		FieldType* fieldType =  new FieldType(texture, modelData, buildable);

		_fieldTypes[symbol] = fieldType;
	}
}

ModelData* Map::_readModel(xml_node<> *node)
{
	ModelData* modelData = new ModelData();
	xml_attribute<> *attr = node->first_attribute("path", 0, false);
	if (attr != NULL)
	{
		modelData->model = _getModel(attr->value());
	}

	//scale model
	xml_node<> *child = node->first_node("Scale", 0, false);
	attr = child->first_attribute("min", 0, false);
	modelData->minScale = _attrToFloat(attr, 1.0);
	attr = child->first_attribute("max", 0, false);
	modelData->maxScale = _attrToFloat(attr, 1.0);

	//rotate model
	child = node->first_node("Rotation", 0, false);
	attr = child->first_attribute("min", 0, false);
	modelData->minRotation = _attrToFloat(attr, 1.0);
	attr = child->first_attribute("max", 0, false);
	modelData->maxRotation = _attrToFloat(attr, 1.0);

	//probability of displaying model
	attr = node->first_attribute("probability", 0, false);
	modelData->probability = _attrToFloat(attr, 1.0);

	return modelData;
}

void Map::_loadGrid(xml_node<> *node)
{
	_height = 0;
	for(xml_node<> *child = node->first_node("Row", 0, false); child; child = child->next_sibling("Row", 0, false))
	{
		std::string rowString = child->value();
		_width = child->value_size();

		_fields.resize(_fields.size() + 1);
		_fields[_height].resize(_width);

		for (long x = _width - 1; x >= 0; x--)
		{
			_fields[_height][x] = new Field(_fieldTypes[rowString[x]].get());
		}

		_height++;
	}
}

void Map::_loadCheckPoints(xml_node<> *node)
{
	for(xml_node<> *child = node->first_node("Checkpoint", 0, false); child; child = child->next_sibling("Checkpoint", 0, false))
	{
		xml_attribute<> *x = child->first_attribute("x", 0, false);
		xml_attribute<> *y = child->first_attribute("y", 0, false);

		MapPoint p;
		p.X = _attrToLong(x, 0);
		p.Y = _attrToLong(y, 0);

		_checkpoints.push_back(p);
	}

	std::vector<MapPoint>(_checkpoints).swap(_checkpoints);
}

osg::Texture2D* Map::_getTexture(const char* filename)
{
	std::list< _cache< osg::ref_ptr<osg::Texture2D> > >::iterator it;
	for(it = _textureCache.begin(); it != _textureCache.end(); it++)
	{
		if (it->filename.compare(filename) == 0)
		{
			it->used = true;
			return it->item;
		}
	}

	std::string textureFilename = MAP_DIRECTORY_TEXTURES;
	textureFilename.append(filename);

	osg::Image* image = osgDB::readImageFile(textureFilename.c_str());
	if (image != NULL)
	{
		osg::Texture2D* texture = new osg::Texture2D(image);
		texture->setMaxAnisotropy(AF_LEVEL);
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
		texture->setDataVariance(osg::Object::STATIC);

		_cache< osg::ref_ptr<osg::Texture2D> > newElement;

		newElement.filename = filename;
		newElement.item = texture;
		newElement.used = true;
		_textureCache.push_back(newElement);
		return texture;
	}
	return NULL;
}

osg::Node* Map::_getModel(const char* filename)
{
	std::list< _cache< osg::ref_ptr<osg::Node> > >::iterator it;
	for(it = _modelCache.begin(); it != _modelCache.end(); it++)
	{
		if (it->filename.compare(filename) == 0)
		{
			it->used = true;
			return it->item;
		}
	}
	std::string modelFilename = MAP_DIRECTORY_MODELS;
	modelFilename.append(filename);

	_cache< osg::ref_ptr<osg::Node> > newElement;

	newElement.filename = filename;
	newElement.item = osgDB::readNodeFile(modelFilename);
	newElement.used = true;
	_modelCache.push_back(newElement);
	return newElement.item;
}
