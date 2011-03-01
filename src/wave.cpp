// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <game_timer.h>
#include <creepattributes.h>
#include <wave.h>
#include <world.h>

Wave::Wave(double waveOffset)
{
	_waveOffset = waveOffset;
	_doSpawn = false;
}

void Wave::addCreeps(int count, CreepAttributes* attributes)
{
	for(int i=0;i<count;i++)
	{
		_attributes.push(attributes);
	}
}

void Wave::startSpawning(World* world)
{
	_doSpawn = true;
	prepareNextCreep(world);
}

void Wave::prepareNextCreep(World* world)
{
	if(_attributes.size() == 0)
	{
		_doSpawn = false;
		world->OnWaveDone();
		return;
	}

	_currentOffset = _attributes.front()->spawnOffset;
}

void Wave::spawnNextCreep(World* world)
{
	OpenSteer::Vec3 steerSpawn = world->getPath()->point(0);
	osg::Vec3 osgSpawn = osg::Vec3(steerSpawn.x, steerSpawn.y, steerSpawn.z);

	Creep* myCreep = new Creep(*world->getProximities(), osgSpawn, world->getPath(), world);
	myCreep->setCreepStats(_attributes.front());
	myCreep->addChild(_attributes.front()->model);
	_attributes.pop();

	world->spawnCreep(myCreep);

	prepareNextCreep(world);
}

void Wave::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	World* myWorld = dynamic_cast<World*>(node);
	if (myWorld != NULL && _doSpawn)
	{
		if(_waveOffset <= 0) {
			_currentOffset -= GameTimer::instance()->elapsedTime();
			if(_currentOffset <= 0)
			{
				spawnNextCreep(myWorld);
			}
		}
		else
		{
			_waveOffset -= GameTimer::instance()->elapsedTime();
		}
	}
	traverse(node, nv);
}
