#include <osg/Node>
#include <updatablenode.h>

class Creep;
class TowerAttributes;

class Tower : public osg::Node, public UpdatableNode
{
public:
	Tower(osg::Vec3 position, TowerAttributes* attributes);
	void onUpdate();
private:
	void shootAt(Creep* creep);

	float _currentCooldown;
	osg::Vec3 _position;

	osg::ref_ptr<TowerAttributes> _attributes;
};