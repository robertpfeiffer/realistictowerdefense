#include <towerupgradebutton.h>

#include <hud.h>
#include <tower.h>
#include <towerattributes.h>
#include <world.h>
#include <inscenetext.h>
#include <hatchery.h>


TowerUpgradeButton::TowerUpgradeButton(Tower* tower, TowerAttributes* attributes) : MenuButton(attributes->icon)
{
	_tower = tower;
	_towerattributes = attributes;
}

void TowerUpgradeButton::onClick(osgGA::GUIActionAdapter& aa)
{
	if(World::instance()->getMap()->getPlayer()->getMoney() < _towerattributes->cost)
	{
		InSceneText* text = new InSceneText("TOO EXPENSIVE", _tower->getPosition());
		text->setColor(osg::Vec3(1.0, 0.0, 0.0));
		Hatchery::instance()->enqueueChild(text);
		return;
	}

	if(_towerattributes->stock <= 0)
	{
		InSceneText* text = new InSceneText("OUT OF STOCK", _tower->getPosition());
		text->setColor(osg::Vec3(1.0, 0.0, 0.0));
		Hatchery::instance()->enqueueChild(text);
		return;
	}
	_tower->getAttributes()->stock++;
	_towerattributes->stock--;
	_tower->upgradeTo(_towerattributes);
	World::instance()->getMap()->getPlayer()->decreaseMoney(_towerattributes->cost);
}
