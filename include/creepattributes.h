#include <osg/Node>

class CreepAttributes : osg::Referenced
{
public:
	int maxHealth;
	int armor;
	int magicResistance;
	float speed;
	int bounty;
	std::string name;
	osg::Node* model;
	float spawnOffset;
};