#pragma once

#include <osg/ClearNode>

struct SkyBoxAttributes;

/**
 * \class	SkyBox
 *
 * \brief	The skybox. 
 */
class SkyBox : public osg::ClearNode
{
public:
	SkyBox(SkyBoxAttributes*);
private:
	osg::TextureCubeMap* createTextureCubeMap(SkyBoxAttributes*);
	osg::Image* getImageFromTexture(osg::Texture2D* texture);
};