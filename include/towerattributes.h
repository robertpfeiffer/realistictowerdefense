#pragma once

#include <osg/PositionAttitudeTransform>

#include <projectileattributes.h>

class TowerAttributes : public osg::Referenced
{
public:
	float cooldown;
	float range;
	long cost;
	float height;
	std::string name;

	ProjectileAttributes projectile;

	osg::ref_ptr<osg::PositionAttitudeTransform> model;
	std::vector<osg::ref_ptr<TowerAttributes>> upgrades;
};