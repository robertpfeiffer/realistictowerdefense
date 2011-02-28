#include <osg/PositionAttitudeTransform>

class CreepAttributes : osg::Referenced
{
public:
	int maxHealth;
	int armor;
	int magicResistance;
	float speed;
	int bounty;
	std::string name;
	osg::ref_ptr<osg::PositionAttitudeTransform> model;
	float spawnOffset;
};