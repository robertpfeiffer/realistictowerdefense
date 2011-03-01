// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include<projectile.h>

#include <creep.h>
#include <algorithm>
#include <game_timer.h>
#include <graveyard.h>
#include <projectileattributes.h>

Projectile::Projectile(osg::Vec3 origin, Creep* target, ProjectileAttributes* attributes)
{
	_attributes = attributes;
	_target = target;

	this->setPosition(origin);
	this->addChild(_attributes->model);
}

void Projectile::onUpdate()
{
	approachToTarget();

	if((this->getPosition()-_target->getPosition()).length2() < 0.1) //FIXME is this a good condition?
	{
		hitTarget();
	}
}

void Projectile::hitTarget()
{
	_target->OnHit(_attributes);
	Graveyard::instance()->killChild(this);
}

void Projectile::approachToTarget()
{
	osg::Vec3 targetVector = _target->getPosition() - this->getPosition();
	float maxDistance = targetVector.normalize();
	float travelDistance = _attributes->travelSpeed * GameTimer::instance()->elapsedTime();
	travelDistance = std::min(maxDistance, travelDistance);

	this->setPosition(this->getPosition()+(targetVector*travelDistance));
}
