#pragma once

#include <osg/Texture2D>
#include <osg/PositionAttitudeTransform>

#include <projectileattributes.h>

class TowerAttributes : public osg::Referenced
{
public:
	double cooldown;
	double range;
	long cost;
	double height;
	std::string name;

	ProjectileAttributes projectile;

	osg::ref_ptr<osg::PositionAttitudeTransform> model;
	std::vector<osg::ref_ptr<TowerAttributes> > upgrades;
	osg::ref_ptr<osg::Texture2D> icon;
};
