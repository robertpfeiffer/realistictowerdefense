// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <tower.h>

#include <creep.h>
#include <gametimer.h>
#include <projectile.h>
#include <towerattributes.h>
#include <world.h>

Tower::Tower(osg::Vec3 position, TowerAttributes* attributes)
{
	_position = position;
	_attributes = attributes;
	_target = NULL;

	this->addChild(_attributes->model);
}

void Tower::onUpdate()
{
	_currentCooldown -= GameTimer::instance()->elapsedTime();
	if(_currentCooldown > 0)
	{
		return;
	}

	_currentCooldown = _attributes->cooldown;

	if(_target.get() == NULL || _target->health() <= 0)
	{
		findNewTarget();
	}

	shootAtTarget();
}

void Tower::findNewTarget()
{
	float range = _attributes->range * _attributes->range; //we will do comparison on range²
	std::set<osg::ref_ptr<Creep>>::iterator it;
	for(it = World::instance()->getCreepsIterator(); it != World::instance()->getCreepsIterator(); it++)
	{
		if((_position - (*it)->getPosition()).length2() <= range)
		{
			_target = it->get();
			return;
		}
	}
}

void Tower::shootAtTarget()
{
	osg::Vec3 origin = osg::Vec3(_position.x(), _attributes->height, _position.z());
	Projectile* p = new Projectile(origin, _target.get(), _attributes->projectile.get());
	World::instance()->addUpdatableNode(p);
}