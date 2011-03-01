#include <osg/PositionAttitudeTransform>

class ProjectileAttributes;

class TowerAttributes : public osg::Referenced
{
public:
	float cooldown;
	int cost;
	float height;
	std::string name;

	osg::ref_ptr<ProjectileAttributes> projectile;

	osg::ref_ptr<osg::PositionAttitudeTransform> model;
	std::vector<osg::ref_ptr<TowerAttributes>> upgrades;
};