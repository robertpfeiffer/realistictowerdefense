#include <osg/Node>

class CreepAttributes
{
public:
	int maxHealth;
	int armor;
	int magicResistance;
	int speed;
	int bounty;

	osg::Node* style;

	double spawnOffset;
};