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

class World : public osg::Group
{
public:
	World();
	static World* instance();
	~World();

	void loadMap(const std::string mapFilename);

	void onDeath(Creep* creep);
	void onLeak(Creep* creep);

	std::set< osg::ref_ptr<Creep> >::iterator getCreepsIterator();
	std::set< osg::ref_ptr<Creep> >::iterator getCreepsIteratorEnd();

	void spawnCreep(Creep* creep);
	void dropCreep(Creep* creep);
	void OnWaveDone();

	void addUpdatableNode(osg::Node* node);
	/*inline*/ void registerForUpdates(osg::Node* node);

	Map* getMap();

	OpenSteer::PolylineSegmentedPathwaySingleRadius* getPath();
	ProximityDatabase* getProximities();
	void addParticleEffect(osgParticle::ParticleSystem* ps);
	void addAnimation(osgAnimation::Animation* anim);

private:
	void createPath();
	void startNextWave();

	osg::ref_ptr<Wave> _currentWave;
	bool _waveDone;
	std::set<osg::ref_ptr<Creep> > _creeps;

	osg::ref_ptr<Map> _map;
	osg::ref_ptr<osgParticle::ParticleSystemUpdater> _psu;
	osg::ref_ptr<osgAnimation::BasicAnimationManager> _mng;
	OpenSteer::PolylineSegmentedPathwaySingleRadius* _path;

	osg::ref_ptr<UpdateCallback> _updateCallback;

	ProximityDatabase* _proximities;
};
