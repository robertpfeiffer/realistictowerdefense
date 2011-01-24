#include <creep.h>

Creep::Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylinePathway* path)
{
	OpenSteer::Vec3 steer_position = OpenSteer::Vec3(position.x(), position.y(), position.z());
	_steering = new CreepSteering(pd, steer_position, path);
	_timer = new osg::Timer();
	_lastTick = _timer->tick();
}

Creep::~Creep()
{
	delete _steering;
	delete _timer;
}

void Creep::OnUpdate()
{
	osg::Timer_t now = _timer->tick();

	_steering->update(_timer->delta_s(now, _lastTick));

	_lastTick = now;
}