#pragma once

#include <map.h>
#include <osg/Node>
#include <osgGA/TerrainManipulator>

class World
{
	public:
		void run();
	private:
		osg::Drawable* createPin(const float & scale, osg::StateSet* bbState);
		void addBillBoards(osg::Group* world, osg::Group* terrain);
		void LimitCamera(osgGA::TerrainManipulator* manipulator);
		osg::Node* createTerrainBlockFromMap(Map* map, int x, int y);
};