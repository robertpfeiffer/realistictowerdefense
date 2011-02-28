#pragma once
#include <creepsteering.h>
#include <updatablenode.h>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <osg/Timer>

class CreepAttributes;
class GameTimer;
class ProjectileAttributes;
class World;

class Creep : public osg::PositionAttitudeTransform, public UpdatableNode
{
public:
	Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylineSegmentedPathwaySingleRadius* path, World* eventHandler);
	void onUpdate();
	void OnHit(ProjectileAttributes* hitter);
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

	int computeDamageReceived(ProjectileAttributes* source);

	void updateRealPosition();
	void updateRealHeading();
};