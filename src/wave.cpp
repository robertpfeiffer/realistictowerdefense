// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <wave.h>
#include <creep.h>
#include <creepattributes.h>
#include <gametimer.h>
#include <world.h>

/**
 * \fn	Wave::Wave(double waveOffset)
 *
 * \brief	Create a new wave spawner.
 *
 * \param	waveOffset	The wave offset.
 */
Wave::Wave(double waveOffset)
{
	_waveOffset = waveOffset;
	_doSpawn = false;
}

/**
 * \fn	void Wave::addCreeps(int count, CreepAttributes* attributes)
 *
 * \brief	Adds the creeps to spawn list.
 *
 * \param	count		Number of creeps to spawn of this type.
 * \param	attributes	The attributes.
 */
void Wave::addCreeps(int count, CreepAttributes* attributes)
{
	for(int i=0;i<count;i++)
	{
		_attributes.push(attributes);
	}
}

/**
 * \fn	void Wave::startSpawning()
 *
 * \brief	Starts spawning of creeps.
 */
void Wave::startSpawning()
{
	_doSpawn = true;
	prepareNextCreep();
}

/**
 * \fn	void Wave::prepareNextCreep()
 *
 * \brief	Prepare next creep.
 */
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

/**
 * \fn	void Wave::spawnNextCreep()
 *
 * \brief	Spawn next creep.
 */
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

/**
 * \fn	void Wave::operator()(osg::Node* node, osg::NodeVisitor* nv)
 *
 * \brief	Spawn next creep, if enough time is elapsed to previous wave/creep.
 */
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
