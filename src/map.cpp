// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <assetlibrary.h>
#include <map.h>
#include <constants.h>
#include <modeldata.h>
#include <limits>
#include <string.h>
#include <osg/PositionAttitudeTransform>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <stdlib.h>

#include <creepattributes.h>
#include <convert.h>
#include <wave.h>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;

Map::Map()
{
	_reset();
}

bool Map::loadMap(const std::string& filename)
{
	rapidxml::file<>* xmlFile;

	try
	{
		xmlFile = new rapidxml::file<>(filename.c_str());
	}
	catch(std::runtime_error rex)
	{
		return false;
	};

	_xml.parse<0>(xmlFile->data());

	_loadLevel();

	delete xmlFile;

	return true;
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

osg::Texture2D* Map::_getTexture(xml_attribute<> *attr)
{
	if (attr == NULL) return NULL;

	return AssetLibrary::instance()->getTexture(attr->value());
}

void Map::_reset()
{
	_checkpoints.clear();

	for (int i = 0; i <= 255; i++)
	{
		_fieldTypes[i] = NULL;
	}

	_width = 0;
	_height = 0;
}

void Map::_cleanup()
{
	for (int i = 0; i <= 255; i++)
	{
		_fieldTypes[i] = NULL;
	}
}

void Map::_loadLevel()
{
	_reset();

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

	child = root->first_node("Skybox", 0, false);
	if (child != NULL)
	{
		_loadSkyBox(child);
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
	_player.setLives(Convert::attrToLong(node->first_attribute("lives", 0, false), 1));
	_player.setMoney(Convert::attrToLong(node->first_attribute("money", 0, false), 50));
}

void Map::_loadWaves(xml_node<> *node)
{
	for(xml_node<> *creepWave = node->first_node("Wave", 0, false); creepWave; creepWave = creepWave->next_sibling("Wave", 0, false))
	{
		Wave* wave = new Wave(Convert::attrToLong(creepWave->first_attribute("spawnoffset", 0, false), 0));
		for(xml_node<> *creep = creepWave->first_node("Creep", 0, false); creep; creep = creep->next_sibling("Creep", 0, false))
		{
			CreepAttributes* attributes = new CreepAttributes();
			xml_attribute<>* nameAttr = creep->first_attribute("name", 0, false);
			attributes->name = "";
			if (nameAttr != NULL)
			{
				attributes->name = nameAttr->value();
			}

			attributes->height = Convert::attrToDouble(creep->first_attribute("height", 0, false), -0.5);
			attributes->maxHealth = Convert::attrToLong(creep->first_attribute("health", 0, false), 1);
			attributes->armor = Convert::attrToLong(creep->first_attribute("armor", 0, false), 0);
			attributes->magicResistance = Convert::attrToLong(creep->first_attribute("magicresist", 0, false), 0);
			attributes->speed = Convert::attrToDouble(creep->first_attribute("speed", 0, false), 1);
			attributes->bounty = Convert::attrToDouble(creep->first_attribute("bounty", 0, false), 0);
			attributes->spawnOffset = Convert::attrToDouble(creep->first_attribute("spawnOffset", 0, false), 0);

			xml_attribute<>* modelAttr = creep->first_attribute("model", 0, false);
			attributes->model = NULL;
			if (modelAttr != NULL)
			{
				attributes->model = new osg::PositionAttitudeTransform();
				attributes->model->addChild(AssetLibrary::instance()->getModel(modelAttr->value()));

				float scale = Convert::attrToDouble(creep->first_attribute("scale", 0, false), 1);
				attributes->model->setScale(osg::Vec3d(scale, scale, scale));
			}

			wave->addCreeps(Convert::attrToLong(creep->first_attribute("count", 0, false), 0), attributes);
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
	tower->cooldown = Convert::attrToDouble(node->first_attribute("cooldown", 0, false), 1);
	tower->range = Convert::attrToDouble(node->first_attribute("range", 0, false), 0);
	tower->cost = Convert::attrToLong(node->first_attribute("cost", 0, false), 0);
	tower->height = Convert::attrToDouble(node->first_attribute("shotheight", 0, false), 0);

	tower->stock = Convert::attrToLong(node->first_attribute("stock", 0, false), LONG_MAX);

	xml_attribute<>* nameAttr = node->first_attribute("name", 0, false);
	tower->name = "";
	if (nameAttr != NULL)
	{
		tower->name = nameAttr->value();
	}

	tower->icon = _getTexture(node->first_attribute("icon", 0, false));

	xml_attribute<>* modelAttr = node->first_attribute("model", 0, false);
	tower->model = NULL;
	if (modelAttr != NULL)
	{
		tower->model = new osg::PositionAttitudeTransform();
		tower->model->addChild(AssetLibrary::instance()->getModel(modelAttr->value()));

		float scale = Convert::attrToDouble(node->first_attribute("scale", 0, false), 1);
		tower->model->setScale(osg::Vec3d(scale, scale, scale));
	}

	//load projectile
	modelAttr = node->first_attribute("projectile", 0, false);
	tower->projectile.model = NULL;
	if (modelAttr != NULL)
	{
		tower->projectile.model = new osg::PositionAttitudeTransform();
		tower->projectile.model->addChild(AssetLibrary::instance()->getModel(modelAttr->value()));

		float scale = Convert::attrToDouble(node->first_attribute("projectilescale", 0, false), 1);
		tower->projectile.model->setScale(osg::Vec3d(scale, scale, scale));
	}
	tower->projectile.slow = Convert::attrToDouble(node->first_attribute("slow", 0, false), 1.0);
	tower->projectile.physicalDamage = Convert::attrToLong(node->first_attribute("physicalDamage", 0, false), 0);
	tower->projectile.magicalDamage = Convert::attrToLong(node->first_attribute("magicalDamage", 0, false), 0);
	tower->projectile.travelSpeed = Convert::attrToLong(node->first_attribute("projectilespeed", 0, false), 1);

	for(xml_node<> *child = node->first_node("Tower", 0, false); child; child = child->next_sibling("Tower", 0, false))
	{
		tower->upgrades.push_back(_getTowerAttributes(child));
	}

	return tower;
}

void Map::_loadSkyBox(xml_node<> *node)
{
	_skyBoxAttributes.texturePosX = _getTexture(node->first_attribute("posX", 0, false));
	_skyBoxAttributes.textureNegX = _getTexture(node->first_attribute("negX", 0, false));
	_skyBoxAttributes.texturePosY = _getTexture(node->first_attribute("posY", 0, false));
	_skyBoxAttributes.textureNegY = _getTexture(node->first_attribute("negY", 0, false));
	_skyBoxAttributes.texturePosZ = _getTexture(node->first_attribute("posZ", 0, false));
	_skyBoxAttributes.textureNegZ = _getTexture(node->first_attribute("negZ", 0, false));
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
	_strata = _getTexture(node->first_attribute("texture", 0, false));
}

void Map::_loadFieldTypes(xml_node<> *node)
{
	for(xml_node<> *child = node->first_node("Field", 0, false); child; child = child->next_sibling("Field", 0, false))
	{
		osg::Texture2D* texture = NULL;
		texture = _getTexture(child->first_attribute("texture", 0, false));

		char symbol = 0;
		xml_attribute<>* attr = child->first_attribute("symbol", 0, false);
		if (attr != NULL)
		{
			if (attr->value_size() > 0)
			{
				symbol = attr->value()[0];
			}
		}
		
		bool buildable = true;
		attr = child->first_attribute("buildable", 0, false);		
		buildable = Convert::attrToBool(attr, true);
		
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
		modelData->model = AssetLibrary::instance()->getModel(attr->value());
	}

	//scale model
	xml_node<> *child = node->first_node("Scale", 0, false);
	attr = child->first_attribute("min", 0, false);
	modelData->minScale = Convert::attrToDouble(attr, 1.0);
	attr = child->first_attribute("max", 0, false);
	modelData->maxScale = Convert::attrToDouble(attr, 1.0);

	//rotate model
	child = node->first_node("Rotation", 0, false);
	attr = child->first_attribute("min", 0, false);
	modelData->minRotation = Convert::attrToDouble(attr, 1.0);
	attr = child->first_attribute("max", 0, false);
	modelData->maxRotation = Convert::attrToDouble(attr, 1.0);

	//probability of displaying model
	attr = node->first_attribute("probability", 0, false);
	modelData->probability = Convert::attrToDouble(attr, 1.0);

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
		p.X = Convert::attrToLong(x, 0);
		p.Y = Convert::attrToLong(y, 0);

		_checkpoints.push_back(p);
	}

	std::vector<MapPoint>(_checkpoints).swap(_checkpoints);
}
