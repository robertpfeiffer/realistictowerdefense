#pragma once

#include <osg/Referenced>
#include <osg/Node>

class ModelData : public osg::Referenced
{	
public:
	osg::ref_ptr<osg::Node> model;
	float minScale;
	float maxScale;

	float minRotation;
	float maxRotation;

	float probability;
};