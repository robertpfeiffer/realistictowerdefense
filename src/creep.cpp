// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <creep.h>
#include <creepattributes.h>
#include <creepinfobox.h>
#include <gametimer.h>
#include <hud.h>
#include <projectileattributes.h>
#include <transformhelper.h>
#include <world.h>

#include <osg/PositionAttitudeTransform>
#include <osg/ComputeBoundsVisitor>

Creep::Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylineSegmentedPathwaySingleRadius* path)
{
	OpenSteer::Vec3 steer_position = OpenSteer::Vec3(position.x(), position.y(), position.z());
	_steering = new CreepSteering(pd, steer_position, path, this);

	_gameTimer = GameTimer::instance();
	_model = NULL;

	this->_healthBar = new HealthBar();

	_healthBarTransform = new PositionAttitudeTransform();
	_healthBarTransform->addChild(this->_healthBar);
	_healthBarTransform->setPosition(osg::Vec3d(0.0, 0.0, 1.0));
	this->addChild(_healthBarTransform);

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
	if(isLeaked())
	{
		//corner-case: leaked, but killing projectile is on its way
		return;
	}

	_steering->setSpeed(_steering->speed() * hitter->slow);
	_health -= computeDamageReceived(hitter);
	_healthBar->setHealth(_health);

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
	this->_healthBar->setMaxHealth(_health, _attributes->maxHealth);
	_steering->setMaxSpeed((float)_attributes->speed/100.0f);
}

void Creep::setModel(osg::Node* model)
{
	//replace model
	this->removeChild(_model);
	_model = model;
	this->addChild(model);

	//calculate new zPosition of healtbar
	osg::PositionAttitudeTransform* pat = dynamic_cast<osg::PositionAttitudeTransform*>(model);

	osg::ComputeBoundsVisitor boundsVisitor;
	boundsVisitor.apply(*model);
	osg::BoundingBox boundingBox = boundsVisitor.getBoundingBox();
	if (pat)
	{
		osg::Vec3d scaleVec = pat->getScale();
		_zMax = boundingBox.zMax()*scaleVec.z();
	}
	else
	{
		_zMax = boundingBox.zMax();
	}

	//applay calculated position
	_healthBarTransform->setPosition(osg::Vec3d(0.0, 0.0, _zMax + 0.1));
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

osg::Vec3 Creep::getHitPosition()
{
	if (_attributes->height < 0)
		return this->getPosition() + osg::Vec3(0, 0, _zMax + _attributes->height);
	
	return this->getPosition() + osg::Vec3(0, 0, _attributes->height);
}

void Creep::updateRealHeading()
{
	osg::Vec3 directionVector = osg::Vec3(_steering->forward().x, _steering->forward().z, 0);
	directionVector.normalize();
	this->setAttitude(TransformHelper::lookAt(directionVector));
}

void Creep::onFocus(osgGA::GUIActionAdapter& aa)
{
	Hud::instance()->setInfoBox(new CreepInfoBox(_attributes));
}

void Creep::onBlur()
{
	Hud::instance()->setInfoBox(NULL);
}
