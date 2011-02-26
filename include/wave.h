#include <queue>
#include <osgDB\Callbacks>

class CreepAttributes;
class World;

class Wave : public osg::NodeCallback
{
public:
	Wave(double waveOffset);

	void addCreeps(int count, CreepAttributes* attributes);
	void startSpawning(World* world);

	void operator()(osg::Node* node, osg::NodeVisitor* nv);

private:
	double _waveOffset;
	double _currentOffset;
	bool _doSpawn;
	std::queue<CreepAttributes*> _attributes;

	void prepareNextCreep(World* world);
	void spawnNextCreep(World* world);
};