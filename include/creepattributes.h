#pragma once
#include <osg/PositionAttitudeTransform>

/**
 * \class	CreepAttributes
 *
 * \brief	Creep attributes. 
 */
class CreepAttributes : osg::Referenced
{
public:
	int maxHealth;
	int armor;
	int magicResistance;
	double speed;
	int bounty;
	std::string name;
	osg::ref_ptr<osg::PositionAttitudeTransform> model;
	double spawnOffset;
};
