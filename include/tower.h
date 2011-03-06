#pragma once

#include <osg/Group>
#include <mouseeventhandler.h>
#include <updatablenode.h>

class ContextMenu;
class Creep;
class TowerAttributes;

class Tower : public osg::Group, public UpdatableNode, public MouseEventHandler
{
public:
	Tower(osg::Vec3 position, TowerAttributes* attributes);
	void onUpdate();
	void upgradeTo(TowerAttributes* attributes);

	TowerAttributes* getAttributes();
	osg::Vec3 getPosition();

	virtual void onFocus(osgGA::GUIActionAdapter& aa);
	virtual void onClick(osgGA::GUIActionAdapter& aa);
	virtual void onBlur();

private:
	bool findNewTarget();
	bool isInRange(Creep* creep);
	void shootAtTarget();

	float _currentCooldown;
	osg::ref_ptr<Creep> _target;

	osg::Vec3 _position;

	osg::ref_ptr<TowerAttributes> _attributes;

	osg::ref_ptr<ContextMenu> _menu;
};
