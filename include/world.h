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

		void spawnCreep(Creep* creep);
		OpenSteer::PolylineSegmentedPathwaySingleRadius* getPath();
		ProximityDatabase* getProximities();

	private:
		void createPath();

		osg::ref_ptr<Map> _map;
		OpenSteer::PolylineSegmentedPathwaySingleRadius* _path;

		ProximityDatabase* _proximities;
};
