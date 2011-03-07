#pragma once
#include <creepsteering.h>
#include <healthbar.h>
#include <mouseeventhandler.h>
#include <updatablenode.h>

#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <osg/Timer>

class CreepAttributes;
class GameTimer;
class ProjectileAttributes;

class Creep : public osg::PositionAttitudeTransform, public UpdatableNode, public MouseEventHandler
{
public:
	Creep(ProximityDatabase& pd, osg::Vec3 position, OpenSteer::PolylineSegmentedPathwaySingleRadius* path);
	void onUpdate();
	void OnHit(ProjectileAttributes* hitter);
        osg::Vec3 getHitPosition();
	void setCreepStats(CreepAttributes* attributes);
	void setModel(osg::Node* model);

	bool isAlive();
	bool isLeaked();

	int health();
	int maxHealth();
	int armor();
	int magicResistance();
	int speed();
	int bounty();

	virtual void onFocus(osgGA::GUIActionAdapter& aa);
	virtual void onBlur();

	~Creep();
private:
	CreepSteering* _steering;
	GameTimer* _gameTimer;

	double _hitHeight;

	osg::ref_ptr<HealthBar> _healthBar;
	int _health;
	CreepAttributes* _attributes;

	int computeDamageReceived(ProjectileAttributes* source);

	void updateRealPosition();
	void updateRealHeading();
	
	osg::ref_ptr<osg::Node> _model;
	osg::ref_ptr<osg::PositionAttitudeTransform> _healthBarTransform;
};
