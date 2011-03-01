// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <tower.h>

#include <creep.h>
#include <gametimer.h>
#include <projectile.h>
#include <towerattributes.h>

void Tower::onUpdate()
{
	_currentCooldown -= GameTimer::instance()->elapsedTime();
	if(_currentCooldown > 0)
	{
		return;
	}

	_currentCooldown = _attributes->cooldown;
	//TODO: shootAt(someCreep);
}

void Tower::shootAt(Creep* creep)
{
	osg::Vec3 origin = osg::Vec3(_position.x(), _attributes->height, _position.z());
	Projectile* p = new Projectile(origin, creep, _attributes->projectile.get());
	//TODO: add projectile to world
}