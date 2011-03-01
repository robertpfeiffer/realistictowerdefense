// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include<projectile.h>

#include <creep.h>
#include <algorithm>
#include <game_timer.h>
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
	float travelDistance = _attributes->travelSpeed * GameTimer::instance()->elapsedTime();
	travelDistance = std::min(maxDistance, travelDistance);

	this->setPosition(this->getPosition()+(targetVector*travelDistance));
}
