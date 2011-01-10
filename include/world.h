#pragma once

#include <map.h>
#include <osg/Group>

class World : public osg::Group
{
	public:
		World(const std::string mapFilename);
	private:
		osg::Drawable* createPin(const float & scale, osg::StateSet* bbState);
		osg::Billboard* addBillBoards();
		osg::Node* createTerrainBlock(int x, int y);
		osg::Node* createStrata();

		osg::ref_ptr<Map> _map;
};