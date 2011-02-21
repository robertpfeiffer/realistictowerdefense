#pragma once
#include <creep_event_handler.h>
#include <creepsteering.h>
#include <game_timer.h>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <osg/Timer>

class Creep : public osg::PositionAttitudeTransform
{
public:
	Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylineSegmentedPathwaySingleRadius* path, CreepEventHandler* eventHandler);
	void OnUpdate();
	~Creep();
private:
	CreepSteering* _steering;
	GameTimer* _gameTimer;
	CreepEventHandler* _eventHandler;

	void updateRealPosition();
	void updateRealHeading();
};