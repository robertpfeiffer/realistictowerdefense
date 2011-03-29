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

/**
 * \fn	Tower::Tower(osg::Vec3 position, TowerAttributes* attributes)
 *
 * \brief	Creates a new tower-instance
 *
 * \param	position	The position of the tower, this is important for range-calculations
 *
 * \param	attributes	The attributes of the tower, they will determine its range and other game-values
 */
Tower::Tower(osg::Vec3 position, TowerAttributes* attributes)
{
	_position = position;
	_attributes = attributes;
	_target = NULL;

	this->addChild(_attributes->model);
}

/**
 * \fn	Tower::onUpdate()
 *
 * \brief	Updates the tower, it might aim for creeps and spawn projectiles to kill them.
 */
void Tower::onUpdate()
{
	_currentCooldown -= GameTimer::instance()->elapsedTime();
	if(_currentCooldown > 0)
	{
		return;
	}

	_currentCooldown = _attributes->cooldown;

	if(_target.get() == NULL || !_target->isAlive() || _target->isLeaked() || !isInRange(_target))
	{
		if(!findNewTarget())
		{
			return;
		}
	}

	shootAtTarget();
}

/**
 * \fn	Tower::getAttributes()
 *
 * \brief	tells which attributes belong to the tower (attributes contain game-relevant values)
 *
 * \return	the attributes of this tower
 */
TowerAttributes* Tower::getAttributes()
{
	return _attributes;
}

/**
 * \fn	Tower::getPosition()
 *
 * \brief	returns the position of the tower
 *
 * \return	the towers position
 */
osg::Vec3 Tower::getPosition()
{
	return _position;
}

/**
 * \fn	Tower::upgradeTo(TowerAttributes* attributes)
 *
 * \brief	Tells the tower to replace its own attributes by the ones specified.
 *
 * \param	attributes	the new attributes
 */
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
	for(it = World::instance()->getCreepsIterator(); it != World::instance()->getCreepsIteratorEnd(); it++)
	{
		if(isInRange(*it))
		{
			_target = it->get();
			return true;
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

	Hud::instance()->pushInfoBox(new TowerInfoBox(_attributes));
}

void Tower::onBlur()
{
	if(this->_menu != NULL)
		this->removeChild(this->_menu);
	this->_menu = NULL;

	Hud::instance()->popInfoBox();
}
