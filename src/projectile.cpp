// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include<projectile.h>

/*
  Projectile game object
  Projectiles are shot by towers.
  Each has a projectileattributes that represents the kind of projectile.
 */

#include <creep.h>
#include <algorithm>
#include <gametimer.h>
#include <graveyard.h>
#include <projectileattributes.h>
#include <transformhelper.h>

Projectile::Projectile(osg::Vec3 origin, Creep* target, ProjectileAttributes* attributes)
{
	_attributes = attributes;
	_target = target;

	this->setPosition(origin);
	this->addChild(_attributes->model);
}

void Projectile::onUpdate()
{
	if(!_target->isAlive() || _target->isLeaked())
	{
		Graveyard::instance()->killChild(this);
		return;
	}

	approachToTarget();

    //FIXME is this a good condition?
	// maybe use bounding boxes for collision detection?
	if((this->getPosition()-_target->getHitPosition()).length2() < 0.1) 
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
	osg::Vec3 targetVector = _target->getHitPosition() - this->getPosition();
	float maxDistance = targetVector.normalize();
	float travelDistance = _attributes->travelSpeed * GameTimer::instance()->elapsedTime();
	travelDistance = std::min(maxDistance, travelDistance);

	this->setPosition(this->getPosition()+(targetVector*travelDistance));
	turnToTarget();
}

void Projectile::turnToTarget()
{
	this->setAttitude(TransformHelper::lookAt(this->getPosition(), _target->getHitPosition()));
}
