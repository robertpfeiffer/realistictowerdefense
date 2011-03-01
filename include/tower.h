#pragma once

#include <osg/Group>
#include <updatablenode.h>

class Creep;
class TowerAttributes;

class Tower : public osg::Group, public UpdatableNode
{
public:
	Tower(osg::Vec3 position, TowerAttributes* attributes);
	void onUpdate();
private:
	bool findNewTarget();
	void shootAtTarget();

	float _currentCooldown;
	osg::ref_ptr<Creep> _target;

	osg::Vec3 _position;

	osg::ref_ptr<TowerAttributes> _attributes;
};