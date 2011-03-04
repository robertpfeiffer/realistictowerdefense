#pragma once

#include <osg/PositionAttitudeTransform>

class ProjectileAttributes
{
public:
	long physicalDamage;
	long magicalDamage;
	double travelSpeed;

	osg::ref_ptr<osg::PositionAttitudeTransform> model;
};