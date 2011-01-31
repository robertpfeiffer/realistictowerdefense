#include <creep.h>

Creep::Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylinePathway* path)
{
	OpenSteer::Vec3 steer_position = OpenSteer::Vec3(position.x(), position.y(), position.z());
	_steering = new CreepSteering(pd, steer_position, path);
	_timer = osg::Timer::instance();
	_lastTick = _timer->tick();

	updateRealPosition();
	updateRealHeading();
}

Creep::~Creep()
{
	delete _steering;
}

void Creep::OnUpdate()
{
	osg::Timer_t now = _timer->tick();

	_steering->update(_timer->delta_s(_lastTick, now));
	updateRealPosition();
	updateRealHeading();

	_lastTick = now;
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