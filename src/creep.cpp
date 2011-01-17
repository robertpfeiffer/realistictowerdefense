#include <creep.h>

Creep::Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylinePathway* path)
{
	OpenSteer::Vec3 steer_position = OpenSteer::Vec3(position.x(), position.y(), position.z());
	_steering = new CreepSteering(pd, steer_position, path);
}

Creep::~Creep()
{
	delete _steering;
}