#pragma once

#include <osg/ClearNode>

struct SkyBoxAttributes;

class SkyBox : public osg::ClearNode
{
public:
	SkyBox(SkyBoxAttributes*);
private:
	osg::TextureCubeMap* readCubeMap(SkyBoxAttributes*);
	osg::Image* getImageFromTexture(osg::Texture2D* texture);
};