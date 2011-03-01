#pragma once

#include <osg/PositionAttitudeTransform>

class ProjectileAttributes : public osg::Referenced
{
public:
	int physicalDamage;
	int magicalDamage;
	float travelSpeed;

	osg::ref_ptr<osg::PositionAttitudeTransform> model;
};