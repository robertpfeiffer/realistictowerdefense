#pragma once
#include <creepsteering.h>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <osg/Timer>

class Creep : public osg::PositionAttitudeTransform
{
public:
	Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylinePathway* path);
	void OnUpdate();
	~Creep();
private:
	CreepSteering* _steering;
	osg::Timer* _timer;
	osg::Timer_t _lastTick;
};