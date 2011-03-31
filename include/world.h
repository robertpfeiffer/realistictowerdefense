#pragma once

#define HAVE_NO_GLUT

#include <osg/Group>
#include <OpenSteer/PolylineSegmentedPathwaySingleRadius.h>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleEffect>
#include <osgAnimation/Animation>
#include <osgAnimation/BasicAnimationManager>

#include <creepsteering.h>

class Map;
class UpdatableNode;
class UpdateCallback;
class Wave;

/**
 * \class	World
 *
 * \brief	The World of the game. 
 */
class World : public osg::Group
{
public:
	static World* instance();
	~World();

	void loadMap(const std::string mapFilename);

	void onDeath(Creep* creep);
	void onLeak(Creep* creep);

	inline std::set< osg::ref_ptr<Creep> >::iterator getCreepsIterator() { return _creeps.begin(); };
	inline std::set< osg::ref_ptr<Creep> >::iterator getCreepsIteratorEnd() { return _creeps.end(); };

	void spawnCreep(Creep* creep);
	void dropCreep(Creep* creep);
	void onWaveDone();

	void addUpdatableNode(osg::Node* node);
	/*inline*/ void registerForUpdates(osg::Node* node);

	inline Map* getMap() { return _map.get(); };

	inline OpenSteer::PolylineSegmentedPathwaySingleRadius* getPath(){ return _path; };
	inline ProximityDatabase* getProximities() {return _proximities; };
	void addParticleEffect(osgParticle::ParticleSystem* ps);
	void addAnimation(osgAnimation::Animation* anim);
private:
	World();
	void createPath();
	void startNextWave();

	osg::ref_ptr<Wave> _currentWave;
	bool _waveDone;
	std::set<osg::ref_ptr<Creep> > _creeps;

	osg::ref_ptr<Map> _map;
	osg::ref_ptr<osgParticle::ParticleSystemUpdater> _particleUpdater;
	osg::ref_ptr<osgAnimation::BasicAnimationManager> _animationManager;
	OpenSteer::PolylineSegmentedPathwaySingleRadius* _path;

	osg::ref_ptr<UpdateCallback> _updateCallback;

	ProximityDatabase* _proximities;
};
