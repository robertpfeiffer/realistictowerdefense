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
#include <map.h>
#include <terrain.h>
#include <wave.h>
#include <world.h>

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
	this->addChild(creep);
	creep->setUpdateCallback(_creepCallback.get());
	_creepCount++;
}

ProximityDatabase* World::getProximities()
{
	return _proximities;
}

void World::startNextWave()
{
	if(_map->getWaves()->size() > 0)
	{
		_currentWave = _map->getWaves()->front();
		_map->getWaves()->pop();

		_currentWave->startSpawning(this);
		this->setUpdateCallback(_currentWave);
		_waveDone = false;
	}
}

void World::dropCreep()
{
	_creepCount--;

	if(_creepCount == 0 && _waveDone)
	{
		this->removeUpdateCallback(_currentWave);
		startNextWave();
	}
}

void World::OnWaveDone()
{
	_waveDone = true;
}

World::World(const std::string mapFilename) : osg::Group()
{
	_map = new Map(mapFilename);
	this->addChild(new Terrain(_map));
	
	_waveDone = true;
	_creepCount = 0;	
	_proximities = new ProximityDatabase();
	_creepCallback = new CreepCallback();

	createPath();

	startNextWave();
}

World::~World()
{
	delete _path;
	//delete _proximities; //uncomment for crash -> crash because memory is still in use by creeps
}