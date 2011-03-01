#pragma once

#include <osg/PositionAttitudeTransform>

class ProjectileAttributes
{
public:
	long physicalDamage;
	long magicalDamage;
	float travelSpeed;

	osg::ref_ptr<osg::PositionAttitudeTransform> model;
};