// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <creep.h>
#include <creepattributes.h>
#include <game_timer.h>
#include <inscenetext.h>
#include <projectileattributes.h>
#include <world.h>

Creep::Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylineSegmentedPathwaySingleRadius* path, World* eventHandler)
{
	OpenSteer::Vec3 steer_position = OpenSteer::Vec3(position.x(), position.y(), position.z());
	_steering = new CreepSteering(pd, steer_position, path, this, eventHandler);
	_world = eventHandler;

	_gameTimer = GameTimer::instance();

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
	int damage = computeDamageReceived(hitter);
	_health -= damage;

	//TODO: insert damage-value
	InSceneText* damageText = new InSceneText(osgText::String("ouch!"), this->getPosition());
	_world->addChild(damageText);
	damageText->addUpdateCallback(_world->getUpdateCallback());

	if(_health <= 0)
	{
		_health = 0;
		_world->onDeath(this);
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
	osg::Quat quad; 
    osg::Matrix matrix; 
	matrix.makeLookAt(osg::Vec3d(0.0,0.0,0.0), directionVector, osg::Z_AXIS); 
    matrix.get(quad);
	this->setAttitude(quad.inverse());
}
