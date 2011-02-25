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

void Creep::setCreepStats(int maxHealth, int armor, int magicResistance, int speed, int bounty)
{
	_maxHealth = maxHealth;
	_armor = armor;
	_magicResistance = magicResistance;
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
	return _armor;
}

int Creep::magicResistance()
{
	return _magicResistance;
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