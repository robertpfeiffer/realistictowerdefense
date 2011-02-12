#pragma once

#include <osg/Geode>
#include <osg/Texture2D>
#include <map.h>
#include <keyboard_event_handler.h>

class Terrain : public osg::Group, public KeyboardEventHandler
{
	public:
		Terrain(Map* map);
	private:
		osg::Node* createTerrainBlock(int x, int y);
		osg::Node* createStrata();

		osg::ref_ptr<Map> _map;
};