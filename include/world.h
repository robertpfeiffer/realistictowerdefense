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
	private:
		osg::Drawable* createTestPin(const float & scale, osg::StateSet* bbState);
		osg::Node* createTestBillboard();

		void createPath();
		void spawnCreep(osg::Node* style);

		osg::ref_ptr<Map> _map;
		OpenSteer::PolylineSegmentedPathwaySingleRadius* _path;
		osg::Vec3 _spawnPosition;

		ProximityDatabase* _proximities;
};
