#pragma once
#include <creepattributes.h>
#include <creepsteering.h>
#include <game_timer.h>
#include <projectile.h>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <osg/Timer>

class World;

class Creep : public osg::PositionAttitudeTransform
{
public:
	Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylineSegmentedPathwaySingleRadius* path, World* eventHandler);
	void OnUpdate();
	void OnHit(Projectile* hitter);
	void setCreepStats(CreepAttributes* attributes);

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
	World* _eventHandler;

	int _health;
	CreepAttributes* _attributes;

	int computeDamageReceived(Projectile* source);

	void updateRealPosition();
	void updateRealHeading();
};