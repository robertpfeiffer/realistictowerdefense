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

void Creep::updateRealPosition()
{
	this->setPosition(osg::Vec3(_steering->position().x, _steering->position().z, 0));
	osg::Vec3 pos = this->getPosition();
}

void Creep::updateRealHeading()
{
	//TODO: implement
}