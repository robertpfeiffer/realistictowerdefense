#include <towercontextmenu.h>

#include <tower.h>
#include <towerattributes.h>
#include <towerupgradebutton.h>
#include <world.h>
 
TowerContextMenu::TowerContextMenu(Tower* tower)
{
	_tower = tower;

	addAllItems();
}

void TowerContextMenu::addAllItems()
{
	std::vector< osg::ref_ptr<TowerAttributes> >::iterator it;
	for(it = _tower->getAttributes()->upgrades.begin();
		it != _tower->getAttributes()->upgrades.end();
		it++)
	{
		this->addEntry(new TowerUpgradeButton(_tower, it->get()));
	}
}
