// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <creep.h>
#include <creepattributes.h>
#include <gametimer.h>
#include <projectileattributes.h>
#include <transformhelper.h>
#include <world.h>

Creep::Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylineSegmentedPathwaySingleRadius* path)
{
	OpenSteer::Vec3 steer_position = OpenSteer::Vec3(position.x(), position.y(), position.z());
	_steering = new CreepSteering(pd, steer_position, path, this);

	_gameTimer = GameTimer::instance();

	this->_healthBar = new HealthBar();
	this->addChild(this->_healthBar);

	updateRealPosition();
	updateRealHeading();
}

Creep::~Creep()
{
	delete _steering;
}

void Creep::onUpdate()
{
	_steering->update(_gameTimer->elapsedTime());
	updateRealPosition();
	updateRealHeading();
}

void Creep::OnHit(ProjectileAttributes* hitter)
{
	_health -= computeDamageReceived(hitter);
    _healthBar->setHealth((float)_health/(float)maxHealth());

	if(_health <= 0)
	{
		_health = 0;
		World::instance()->onDeath(this);
	}
}

int Creep::computeDamageReceived(ProjectileAttributes* source)
{
	double physDmgFactor = 1.0 - (double)_attributes->armor/(100.0+(double)_attributes->armor);
	double magicDmgFactor = 1.0 - (double)_attributes->magicResistance/(100.0+(double)_attributes->magicResistance);

	int totalDamage = 0;
	totalDamage += source->physicalDamage * physDmgFactor;
	totalDamage += source->magicalDamage * magicDmgFactor;

	return totalDamage;
}

void Creep::setCreepStats(CreepAttributes* attributes)
{
	_attributes = attributes;
	_health = _attributes->maxHealth;
	_steering->setMaxSpeed((float)_attributes->speed/100.0f);
}

bool Creep::isAlive()
{
	return _health > 0;
}

bool Creep::isLeaked()
{
	return _steering->isLeaked();
}

int Creep::health()
{
	return _health;
}

int Creep::maxHealth()
{
	return _attributes->maxHealth;
}

int Creep::armor()
{
	return _attributes->armor;
}

int Creep::magicResistance()
{
	return _attributes->magicResistance;
}

int Creep::speed()
{
	return _steering->maxSpeed()*100;
}

int Creep::bounty()
{
	return _attributes->bounty;
}

void Creep::updateRealPosition()
{
	this->setPosition(osg::Vec3(_steering->position().x, _steering->position().z, 0));
	osg::Vec3 pos = this->getPosition();
}

void Creep::updateRealHeading()
{
	osg::Vec3 directionVector = osg::Vec3(_steering->forward().x, _steering->forward().z, 0);
	directionVector.normalize();
	this->setAttitude(TransformHelper::lookAt(directionVector));
}
