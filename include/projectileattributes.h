#include <osg/Referenced>

class ProjectileAttributes : osg::Referenced
{
public:
	int physicalDamage;
	int magicalDamage;
	float travelSpeed;

	osg::ref_ptr<osg::PositionAttitudeTransform> model;
};