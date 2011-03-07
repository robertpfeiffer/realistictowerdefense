// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-
#include <tower.h>

#include <creep.h>
#include <gametimer.h>
#include <hatchery.h>
#include <hud.h>
#include <field.h>
#include <projectile.h>
#include <towerattributes.h>
#include <towercontextmenu.h>
#include <towerinfobox.h>
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

	if(_target.get() == NULL || !_target->isAlive() || _target->isLeaked() || !isInRange(_target) || _attributes->strategy != 1)
	{
		if(!findNewTarget())
		{
			return;
		}
	}

	shootAtTarget();
}

TowerAttributes* Tower::getAttributes()
{
	return _attributes;
}


osg::Vec3 Tower::getPosition()
{
	return _position;
}

void Tower::upgradeTo(TowerAttributes* attributes)
{
	this->removeChild(_attributes->model);
	this->addChild(attributes->model);
	_attributes = attributes;
	dynamic_cast<Field*>(this->getParent(0))->reset();
}

bool Tower::findNewTarget()
{
	float range = _attributes->range * _attributes->range; //we will do comparison on range 
	std::set< osg::ref_ptr<Creep> >::iterator it;
	_target = NULL;
	switch(_attributes->strategy)
	{
	case 2:
	{
		//hit weakest target
		int health = 1000000;
		for(it = World::instance()->getCreepsIterator(); it != World::instance()->getCreepsIteratorEnd(); it++)
		{
			if(isInRange(*it) && health >_target->health() )
			{
				_target = it->get();
				health = _target->health();
			}
		}
		if ( _target != NULL )
			return true;
	}
	case 3:
	{
		//hit youngest target
		for(it = World::instance()->getCreepsIterator(); it != World::instance()->getCreepsIteratorEnd(); it++)
		{
			if(isInRange(*it))
			{
				_target = it->get();
			}
		}
		if ( _target != NULL )
			return true;
	}
	default:
	{
		for(it = World::instance()->getCreepsIterator(); it != World::instance()->getCreepsIteratorEnd(); it++)
		{
			if(isInRange(*it))
			{
				_target = it->get();
				return true;
			}
		}
	}
	}
	_target = NULL;
	return false;
}

bool Tower::isInRange(Creep* creep)
{
	return (_position - creep->getPosition()).length() <= _attributes->range;
}

void Tower::shootAtTarget()
{
	osg::Vec3 origin = osg::Vec3(_position.x(), _position.y(), _attributes->height);
	Projectile* p = new Projectile(origin, _target.get(), &(_attributes->projectile));
	Hatchery::instance()->enqueueChild(p);
}

void Tower::onFocus(osgGA::GUIActionAdapter& aa)
{
	this->_menu = new TowerContextMenu(this);
	this->addChild(_menu);

	Hud::instance()->setInfoBox(new TowerInfoBox(_attributes));
}

void Tower::onBlur()
{
	if(this->_menu != NULL)
		this->removeChild(this->_menu);
	this->_menu = NULL;

	Hud::instance()->setInfoBox(NULL);
}
