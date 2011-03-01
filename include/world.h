#pragma once

#define HAVE_NO_GLUT

#include <map.h>
#include <osg/Group>
#include <OpenSteer/PolylineSegmentedPathwaySingleRadius.h>
#include <creep.h>
#include <updatecallback.h>

class World : public osg::Group
{
public:
	World();
	static World* instance();
	~World();

	void loadMap(const std::string mapFilename);

	void onDeath(Creep* creep);
	void onLeak(Creep* creep);

	void dropCreep();
	void OnWaveDone();

	void spawnCreep(Creep* creep);
	OpenSteer::PolylineSegmentedPathwaySingleRadius* getPath();
	ProximityDatabase* getProximities();

	UpdateCallback* getUpdateCallback();

private:
	void createPath();
	void startNextWave();

	osg::ref_ptr<Wave> _currentWave;
	bool _waveDone;
	int _creepCount;

	osg::ref_ptr<Map> _map;
	OpenSteer::PolylineSegmentedPathwaySingleRadius* _path;

	osg::ref_ptr<UpdateCallback> _updateCallback;

	ProximityDatabase* _proximities;
};
