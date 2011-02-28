#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>

class ProjectileAttributes;
class Creep;

class Projectile : public osg::PositionAttitudeTransform
{
public:
	Projectile(osg::Vec3 origin, Creep* target, ProjectileAttributes* attributes);

	void onUpdate();

private:
	void hitTarget();
	void approachToTarget();

	ProjectileAttributes* _attributes;
	Creep* _target;
};