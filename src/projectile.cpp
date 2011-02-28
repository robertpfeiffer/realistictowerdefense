#include<projectile.h>

Projectile::Projectile(osg::Vec3 origin, Creep* target, ProjectileAttributes* attributes)
{
	this->setPosition(origin);
	_attributes = attributes;
	_target = target;
}

void Projectile::onUpdate()
{
	approachToTarget();

	if(/*close enough*/ false)
	{
		hitTarget();
	}
}

void Projectile::hitTarget()
{

}

void Projectile::approachToTarget()
{

}