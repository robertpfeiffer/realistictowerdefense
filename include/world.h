#pragma once

#include <map.h>
#include <osg/Group>

class World : public osg::Group
{
	public:
		World();
	private:
		osg::Drawable* createPin(const float & scale, osg::StateSet* bbState);
		osg::Billboard* addBillBoards();
		osg::Node* createTerrainBlockFromMap(Map* map, int x, int y);
};