#pragma once
#include <creepsteering.h>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>

class Creep : public osg::PositionAttitudeTransform
{
public:
	Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylinePathway* path);
	~Creep();
private:
	CreepSteering* _steering;
};