#pragma once
#include <creepsteering.h>
#include <game_timer.h>
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
	GameTimer* _gameTimer;

	void updateRealPosition();
	void updateRealHeading();
};