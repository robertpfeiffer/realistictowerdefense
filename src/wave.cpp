// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <wave.h>

/**
 * @class Wave
 * A Wave of Creeps
 */

#include <creep.h>
#include <creepattributes.h>
#include <gametimer.h>
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

void Wave::startSpawning()
{
	_doSpawn = true;
	prepareNextCreep();
}

void Wave::prepareNextCreep()
{
	if(_attributes.size() == 0)
	{
		_doSpawn = false;
		World::instance()->onWaveDone();
		return;
	}

	_currentOffset = _attributes.front()->spawnOffset;
}

void Wave::spawnNextCreep()
{
	OpenSteer::Vec3 steerSpawn = World::instance()->getPath()->point(0);
	osg::Vec3 osgSpawn = osg::Vec3(steerSpawn.x, steerSpawn.y, steerSpawn.z);

	Creep* myCreep = new Creep(*World::instance()->getProximities(), osgSpawn, World::instance()->getPath());
	myCreep->setCreepStats(_attributes.front());
	myCreep->setModel(_attributes.front()->model);
	_attributes.pop();

	World::instance()->spawnCreep(myCreep);

	prepareNextCreep();
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
				spawnNextCreep();
			}
		}
		else
		{
			_waveOffset -= GameTimer::instance()->elapsedTime();
		}
	}
	traverse(node, nv);
}
