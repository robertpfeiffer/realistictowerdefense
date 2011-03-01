class ProjectileAttributes
{
public:
	int physicalDamage;
	int magicalDamage;
	float travelSpeed;

	osg::ref_ptr<osg::PositionAttitudeTransform> model;
};