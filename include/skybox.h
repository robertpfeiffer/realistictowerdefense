#pragma once

#include <osg/ClearNode>

class SkyBox : public osg::ClearNode
{
public:
	SkyBox();
	osg::TextureCubeMap* readCubeMap();
};