#include <creep.h>

Creep::Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylineSegmentedPathwaySingleRadius* path, CreepEventHandler* eventHandler)
{
	OpenSteer::Vec3 steer_position = OpenSteer::Vec3(position.x(), position.y(), position.z());
	_steering = new CreepSteering(pd, steer_position, path, this, eventHandler);
	_eventHandler = eventHandler;

	_gameTimer = GameTimer::instance();

	updateRealPosition();
	updateRealHeading();
}

Creep::~Creep()
{
	delete _steering;
}

void Creep::OnUpdate()
{
	_steering->update(_gameTimer->elapsedTime());
	updateRealPosition();
	updateRealHeading();
}

void Creep::OnHit(Projectile* hitter)
{
	_health -= computeDamageReceived(hitter);

	if(_health <= 0)
	{
		_health = 0;
		_eventHandler->onDeath(this);
	}
}

int Creep::computeDamageReceived(Projectile* source)
{
	double physDmgFactor = 1.0 - _armor/(100.0+_armor);
	double magicDmgFactor = 1.0 - _magicResistance/(100.0+_magicResistance);

	int totalDamage = 0;
	totalDamage += source->physicalDamage() * physDmgFactor;
	totalDamage += source->magicalDamage() * magicDmgFactor;

	return totalDamage;
}

void Creep::setCreepStats(int maxHealth, int armor, int magicResistance, int speed, int bounty)
{
	_maxHealth = maxHealth;
	_armor = (double)armor;
	_magicResistance = (double)magicResistance;
	_steering->setSpeed((float)speed/100.0f);
	_bounty = bounty;
}

int Creep::health()
{
	return _health;
}

int Creep::maxHealth()
{
	return _maxHealth;
}

int Creep::armor()
{
	return (int)_armor;
}

int Creep::magicResistance()
{
	return (int)_magicResistance;
}

int Creep::speed()
{
	return _steering->speed()*100;
}

int Creep::bounty()
{
	return _bounty;
}

void Creep::updateRealPosition()
{
	this->setPosition(osg::Vec3(_steering->position().x, _steering->position().z, 0));
	osg::Vec3 pos = this->getPosition();
}

void Creep::updateRealHeading()
{
	//TODO: implement
}