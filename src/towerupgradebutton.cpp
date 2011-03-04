#include <towerupgradebutton.h>

#include <hud.h>
#include <tower.h>
#include <towerattributes.h>
#include <world.h>

TowerUpgradeButton::TowerUpgradeButton(Tower* tower, TowerAttributes* attributes) : MenuButton(attributes->icon)
{
	_tower = tower;
	_towerattributes = attributes;
}

void TowerUpgradeButton::onClick(osgGA::GUIActionAdapter& aa)
{
	if(World::instance()->getMap()->getPlayer()->getMoney() < _towerattributes->cost)
	{
		return;
	}

	_tower->upgradeTo(_towerattributes);
	World::instance()->getMap()->getPlayer()->decreaseMoney(_towerattributes->cost);
	Hud::instance()->onPlayerUpdate();
}