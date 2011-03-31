#pragma once

#include <osg/PositionAttitudeTransform>

/**
 * \class	ProjectileAttributes
 *
 * \brief	Projectile attributes. 
 */
class ProjectileAttributes
{
public:
	long physicalDamage;
	long magicalDamage;
	double travelSpeed;
	double slow;

	osg::ref_ptr<osg::PositionAttitudeTransform> model;
};
