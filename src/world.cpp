// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <world.h>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osg/Group>

#include <inscenetext.h>
#include <terrain.h>
#include <constants.h>
#include <convert.h>
#include <creep.h>
#include <gametimer.h>
#include <graveyard.h>
#include <hatchery.h>
#include <hud.h>
#include <inscenetext.h>
#include <map.h>
#include <skybox.h>
#include <terrain.h>
#include <updatecallback.h>
#include <wave.h>

/**
 * \fn	World* World::instance()
 *
 * \brief	Gets the instance of the world (singleton).
 *
 * \return	The world.
 */
World* World::instance()
{
	static osg::ref_ptr<World> world_ptr;
	if(world_ptr.get() == NULL)
	{
		world_ptr = new World();
	}
	return world_ptr.get();
}

/**
 * \fn	void World::createPath()
 *
 * \brief	Creates the path for creeps from checkpoint list.
 */
void World::createPath()
{
	std::vector<OpenSteer::Vec3> pathPoints = std::vector<OpenSteer::Vec3>();
	std::vector<MapPoint>* checkpoints = _map->getCheckpoints();
	pathPoints.resize(_map->getCheckpoints()->size());
	for(unsigned int i=0;i < _map->getCheckpoints()->size(); i++)
	{
		pathPoints[i] = OpenSteer::Vec3((float) ((*checkpoints)[i].X), 0.0, (float) -((*checkpoints)[i].Y));
	}
	
	//FIXME: made path extra-narrow to work around creeps leaving path
	_path = new OpenSteer::PolylineSegmentedPathwaySingleRadius(pathPoints.size(), pathPoints.data(), 0.35, false);
}

/**
 * \fn	void World::spawnCreep(Creep* creep)
 *
 * \brief	Spawns a creep.
 *
 * \param	creep	The new creep.
 */
void World::spawnCreep(Creep* creep)
{
	this->addUpdatableNode(creep);
	_creeps.insert(creep);
}

/**
 * \fn	void World::startNextWave()
 *
 * \brief	Start next wave.
 */
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
	else
	{
		if(_map->getPlayer()->getLives() > 0)
		{
			GameTimer::instance()->pause();
			Hud::instance()->onGameEnd(true);
		}
	}
}

/**
 * \fn	void World::onDeath(Creep* creep)
 *
 * \brief	Executed if the player had killed a creep.
 * 			Show an InSceneText, that the player got a bounty for this kill.
 *
 * \param	creep	The killed creep.
 */
void World::onDeath(Creep* creep)
{
	int bounty = creep->bounty();
	_map->getPlayer()->addMoney(bounty);

	InSceneText* bountyText = new InSceneText(Convert::toString(bounty), creep->getPosition());
	bountyText->setColor(osg::Vec3(1.0, 1.0, 0.0));
	Hatchery::instance()->enqueueChild(bountyText);

	dropCreep(creep);
}

/**
 * \fn	void World::onLeak(Creep* creep)
 *
 * \brief	Executed if a creep reaches his target field.
 * 			Reduce live of player by once.
 * 			Show an InSceneText, that the player lost a live.
 *
 * \param	creep	The creep.
 */
void World::onLeak(Creep* creep)
{
	_map->getPlayer()->takeLife();
	if(_map->getPlayer()->getLives() == 0)
	{
		GameTimer::instance()->pause();
		Hud::instance()->onGameEnd(false);
	}

	InSceneText* lifeLostText = new InSceneText("-1", creep->getPosition());
	lifeLostText->setColor(osg::Vec3(1.0, 0.0, 0.0));
	Hatchery::instance()->enqueueChild(lifeLostText);

	dropCreep(creep);
}

/**
 * \fn	void World::dropCreep(Creep* creep)
 *
 * \brief	Drop a creep from world.
 * 			Start a new wave if all creeps are removed from world.
 *
 * \param	creep	The creep.
 */
void World::dropCreep(Creep* creep)
{
	Graveyard::instance()->killChild(creep);
	_creeps.erase(creep);

	if(_creeps.size() == 0 && _waveDone)
	{
		startNextWave();
	}
}

/**
 * \fn	void World::onWaveDone()
 *
 * \brief	Executed, if the wave spawned completly.
 * 			Remove callback on wave spawner.
 */
void World::onWaveDone()
{
	this->removeUpdateCallback(_currentWave);
	_waveDone = true;
}

/**
 * \fn	void World::addUpdatableNode(osg::Node* node)
 *
 * \brief	Adds a node to scenegraph and register it for the update callback. 
 *
 * \param	node	The node.
 */
void World::addUpdatableNode(osg::Node* node)
{
	this->addChild(node);
	registerForUpdates(node);
}

/**
 * \fn	void World::registerForUpdates(osg::Node* node)
 *
 * \brief	Registers this object for updates.
 *
 * \param	node	The node, which want to recieve the update callback.
 */
void World::registerForUpdates(osg::Node* node)
{
	node->addUpdateCallback(_updateCallback.get());
}

/**
 * \fn	World::World()
 *
 * \brief	Add a ParticleSystemUpdater and a BasicAnimationManager to scenegraph
 */
World::World()
{
	_path = NULL;

	osg::PositionAttitudeTransform* grp = new osg::PositionAttitudeTransform;
	grp->setScale(osg::Vec3(0.00001,0.00001,0.00001));
	this->addChild(grp);
	
	_particleUpdater = new osgParticle::ParticleSystemUpdater;
	this->addChild(_particleUpdater);

	_animationManager = new osgAnimation::BasicAnimationManager();
	this->setUpdateCallback(_animationManager);
}

/**
 * \fn	void World::loadMap(const std::string mapFilename)
 *
 * \brief	Loads a map and add them to scenegraph.
 *
 * \param	mapFilename	Filename of the map file.
 */
void World::loadMap(const std::string mapFilename)
{
	_map = new Map();
	if (!_map->loadMap(mapFilename)) exit(1);

	this->addChild(new Terrain(_map));
	this->addChild(new SkyBox(_map->getSkyBoxAttributes()));
	Hud::instance()->setPlayer(_map->getPlayer());

	_waveDone = true;
	_proximities = new ProximityDatabase();
	_updateCallback = new UpdateCallback();

	createPath();

	startNextWave();
}

/**
 * \fn	void World::addParticleEffect(osgParticle::ParticleSystem* ps)
 *
 * \brief	Adds a particle effect. 
 *
 * \param	ps	The particle, which should be registered.
 */
void World::addParticleEffect(osgParticle::ParticleSystem* ps)
{
	_particleUpdater->addParticleSystem(ps);
}

/**
 * \fn	void World::addAnimation(osgAnimation::Animation* anim)
 *
 * \brief	Adds an animation. 
 *
 * \param	anim	The animation, which should be registered.
 */
void World::addAnimation(osgAnimation::Animation* anim)
{
	_animationManager->registerAnimation(anim);
	_animationManager->playAnimation(anim);
}

/**
 * \fn	World::~World()
 *
 * \brief	Destructor.
 */
World::~World()
{
	if (_path != NULL)	delete _path;
	//delete _proximities; //uncomment for crash -> crash because memory is still in use by creeps
}
