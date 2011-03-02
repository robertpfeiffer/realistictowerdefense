// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TerrainManipulator>

#include <osg/AlphaFunc>
#include <osgDB/Registry>

#include <constants.h>
#include <convert.h>
#include <graveyard.h>
#include <hatchery.h>
#include <inscenetext.h>
#include <map.h>
#include <terrain.h>
#include <wave.h>
#include <world.h>
#include <skybox.h>

World* World::instance()
{
	static osg::ref_ptr<World> world_ptr;
	if(world_ptr.get() == NULL)
	{
		world_ptr = new World();
	}
	return world_ptr.get();
}

void World::createPath()
{
	std::vector<OpenSteer::Vec3> pathPoints = std::vector<OpenSteer::Vec3>();
	std::vector<MapPoint>* checkpoints = _map->getCheckpoints();
	pathPoints.resize(_map->getCheckpoints()->size());
	for(unsigned int i=0;i < _map->getCheckpoints()->size(); i++)
	{
		pathPoints[i] = OpenSteer::Vec3((float) ((*checkpoints)[i].X), 0.0, (float) -((*checkpoints)[i].Y));
	}
	
	_path = new OpenSteer::PolylineSegmentedPathwaySingleRadius(pathPoints.size(), pathPoints.data(), 0.5, false);
}

OpenSteer::PolylineSegmentedPathwaySingleRadius* World::getPath()
{
	return _path;
}

void World::spawnCreep(Creep* creep)
{
	this->addUpdatableNode(creep);
	_creeps.insert(creep);
}

std::set<osg::ref_ptr<Creep>>::iterator World::getCreepsIterator()
{
	return _creeps.begin();
}

std::set<osg::ref_ptr<Creep>>::iterator World::getCreepsIteratorEnd()
{
	return _creeps.end();
}

ProximityDatabase* World::getProximities()
{
	return _proximities;
}

Map* World::getMap()
{
	return _map.get();
}

void World::startNextWave()
{
	if(_map->getWaves()->size() > 0)
	{
		_currentWave = _map->getWaves()->front();
		_map->getWaves()->pop();

		_currentWave->startSpawning();
		this->addUpdateCallback(_currentWave);
		_waveDone = false;
	}
}

void World::onDeath(Creep* creep)
{
	int bounty = creep->bounty(); //TODO: reward bounty

	InSceneText* damageText = new InSceneText(osgText::String(Convert::toString(bounty)), creep->getPosition());
	damageText->setColor(osg::Vec3(1.0, 1.0, 0.0));
	Hatchery::instance()->enqueueChild(damageText);

	dropCreep(creep);
}
void World::onLeak(Creep* creep)
{
	dropCreep(creep);
}

void World::dropCreep(Creep* creep)
{
	Graveyard::instance()->killChild(creep);
	_creeps.erase(creep);

	if(_creeps.size() == 0 && _waveDone)
	{
		startNextWave();
	}
}

void World::OnWaveDone()
{
	this->removeUpdateCallback(_currentWave);
	_waveDone = true;
}

void World::addUpdatableNode(osg::Node* node)
{
	this->addChild(node);
	registerForUpdates(node);
}

inline void World::registerForUpdates(osg::Node* node)
{
	node->addUpdateCallback(_updateCallback.get());
}

World::World() : osg::Group()
{
	//hack hack: all construction stuff moved into loadMap
}

//TODO: this is not intended to be called twice
void World::loadMap(const std::string mapFilename)
{
	_map = new Map(mapFilename);
	this->addChild(new Terrain(_map));
	this->addChild(new SkyBox());

	_waveDone = true;
	_proximities = new ProximityDatabase();
	_updateCallback = new UpdateCallback();

	createPath();

	startNextWave();
}

World::~World()
{
	delete _path;
	//delete _proximities; //uncomment for crash -> crash because memory is still in use by creeps
}
