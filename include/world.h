#pragma once

#include <map.h>
#include <osg/Group>
#include <OpenSteer\Pathway.h>

class World : public osg::Group
{
	public:
		World(const std::string mapFilename);
		~World();
	private:
		osg::Drawable* createPin(const float & scale, osg::StateSet* bbState);
		osg::Billboard* addBillBoards();
		osg::Node* createTerrainBlock(int x, int y);
		osg::Node* createStrata();
		void createPath();

		osg::ref_ptr<Map> _map;
		OpenSteer::PolylinePathway* _path;
};