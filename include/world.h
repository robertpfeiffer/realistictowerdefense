#pragma once

#define HAVE_NO_GLUT

#include <map.h>
#include <osg/Group>
#include <OpenSteer/PolylineSegmentedPathwaySingleRadius.h>
#include <creep.h>
#include <updatecallback.h>

class UpdatableNode;

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

private:
	void createPath();
	void startNextWave();

	osg::ref_ptr<Wave> _currentWave;
	bool _waveDone;
	std::set<osg::ref_ptr<Creep> > _creeps;

	osg::ref_ptr<Map> _map;
	OpenSteer::PolylineSegmentedPathwaySingleRadius* _path;

	osg::ref_ptr<UpdateCallback> _updateCallback;

	ProximityDatabase* _proximities;
};
