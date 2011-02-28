#include <osg/Geometry>

class ProjectileAttributes;
class Creep;

class Projectile
{
public:
	Projectile(osg::Vec3 origin, Creep* target, ProjectileAttributes* attributes);

private:
	ProjectileAttributes* _attributes;
	Creep* _target;
};