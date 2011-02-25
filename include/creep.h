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
	void setCreepStats(int maxHealth, int armor, int magicResistance, int speed, int bounty);

	int health();
	int maxHealth();
	int armor();
	int magicResistance();
	int speed();
	int bounty();

	~Creep();
private:
	CreepSteering* _steering;
	GameTimer* _gameTimer;
	CreepEventHandler* _eventHandler;

	int _health;
	int _maxHealth;
	int _armor;
	int _magicResistance;
	int _bounty;

	void updateRealPosition();
	void updateRealHeading();
};