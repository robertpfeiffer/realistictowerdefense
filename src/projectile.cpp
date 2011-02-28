#include <projectile.h>

#include <creep.h>
#include <algorithm>
#include <projectileattributes.h>

Projectile::Projectile(osg::Vec3 origin, Creep* target, ProjectileAttributes* attributes)
{
	this->setPosition(origin);
	_attributes = attributes;
	_target = target;
}

void Projectile::onUpdate()
{
	//TODO upodatecallback to call this... callback in projectile?

	approachToTarget();

	if(this->getPosition() == _target->getPosition()) //is this a good condition?
	{
		hitTarget();
	}
}

void Projectile::hitTarget()
{
	_target->OnHit(_attributes);
	//TODO clean up (remove from everywhere)
}

void Projectile::approachToTarget()
{
	osg::Vec3 targetVector = _target->getPosition() - this->getPosition();
	float maxDistance = targetVector.normalize();
	float travelDistance = std::min(maxDistance, _attributes->travelSpeed);

	this->setPosition(this->getPosition()+(targetVector*travelDistance));
}