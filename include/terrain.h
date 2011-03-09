#pragma once

#include <osg/Geode>
#include <osg/Texture2D>

class Map;

class Terrain : public osg::Group
{
public:
	Terrain(Map* map);
private:
	osg::Node* createTerrainBlock(int x, int y);
	osg::Node* createStrata();

	osg::ref_ptr<Map> _map;
};