#pragma once

#define HAVE_NO_GLUT

#include <map.h>
#include <osg/Group>
#include <OpenSteer/PolylineSegmentedPathwaySingleRadius.h>
#include <creep.h>
#include <creepcallback.h>
#include <creep_event_handler.h>

class World : public osg::Group
{
	public:
		World(const std::string mapFilename);
		~World();

		void dropCreep();
		void OnWaveDone();

		void spawnCreep(Creep* creep);
		OpenSteer::PolylineSegmentedPathwaySingleRadius* getPath();
		ProximityDatabase* getProximities();

	private:
		void createPath();
		void startNextWave();

		Wave* _currentWave;
		bool _waveDone;
		int _creepCount;

		osg::ref_ptr<Map> _map;
		OpenSteer::PolylineSegmentedPathwaySingleRadius* _path;

		osg::ref_ptr<CreepCallback> _creepCallback;

		ProximityDatabase* _proximities;
};
