#pragma once

#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <updatablenode.h>

class ProjectileAttributes;
class Creep;

/**
 * \class	Projectile
 *
 * \brief	A single projectile. 
 */
class Projectile : public osg::PositionAttitudeTransform, public UpdatableNode
{
public:
	Projectile(osg::Vec3 origin, Creep* target, ProjectileAttributes* attributes);

	void onUpdate();

private:
	void hitTarget();
	void approachToTarget();
	void turnToTarget();

	ProjectileAttributes* _attributes;
	osg::ref_ptr<Creep> _target;
};