#pragma once

#include <osg/Texture2D>

struct SkyBoxAttributes
{
	osg::ref_ptr<osg::Texture2D> texturePosX;
	osg::ref_ptr<osg::Texture2D> textureNegX;
	osg::ref_ptr<osg::Texture2D> texturePosY;
	osg::ref_ptr<osg::Texture2D> textureNegY;
	osg::ref_ptr<osg::Texture2D> texturePosZ;
	osg::ref_ptr<osg::Texture2D> textureNegZ;
};