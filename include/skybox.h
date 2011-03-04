#pragma once

#include <osg/ClearNode>

struct SkyBoxAttributes;

class SkyBox : public osg::ClearNode
{
public:
	SkyBox(SkyBoxAttributes*);
	osg::TextureCubeMap* readCubeMap(SkyBoxAttributes*);
};