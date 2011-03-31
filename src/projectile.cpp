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

/**
 * \fn	Projectile::Projectile(osg::Vec3 origin, Creep* target, ProjectileAttributes* attributes)
 *
 * \brief	Create a new projectile.
 *
 * \param	origin		The origin.
 * \param	target	  	Target creep.
 * \param	attributes	The projectile attributes.
 */
Projectile::Projectile(osg::Vec3 origin, Creep* target, ProjectileAttributes* attributes)
{
	_attributes = attributes;
	_target = target;

	this->setPosition(origin);
	this->addChild(_attributes->model);
}

/**
 * \fn	void Projectile::onUpdate()
 *
 * \brief	Fly in direction of creep if it still exists.
 */
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

/**
 * \fn	void Projectile::hitTarget()
 *
 * \brief	Hit target.
 */
void Projectile::hitTarget()
{
	_target->OnHit(_attributes);
	Graveyard::instance()->killChild(this);
}

/**
 * \fn	void Projectile::approachToTarget()
 *
 * \brief	Approach to target.
 */
void Projectile::approachToTarget()
{
	osg::Vec3 targetVector = _target->getHitPosition() - this->getPosition();
	float maxDistance = targetVector.normalize();
	float travelDistance = _attributes->travelSpeed * GameTimer::instance()->elapsedTime();
	travelDistance = std::min(maxDistance, travelDistance);

	this->setPosition(this->getPosition()+(targetVector*travelDistance));
	turnToTarget();
}

/**
 * \fn	void Projectile::turnToTarget()
 *
 * \brief	Turn to target.
 */
void Projectile::turnToTarget()
{
	this->setAttitude(TransformHelper::lookAt(this->getPosition(), _target->getHitPosition()));
}
