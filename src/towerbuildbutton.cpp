#include <towerbuildbutton.h>

#include <field.h>
#include <hud.h>
#include <tower.h>
#include <towerattributes.h>
#include <world.h>

TowerBuildButton::TowerBuildButton(Field* field, TowerAttributes* attributes) : MenuButton(attributes->icon)
{
	_field = field;
	_towerattributes = attributes;
}

void TowerBuildButton::onClick(osgGA::GUIActionAdapter& aa)
{
	if(World::instance()->getMap()->getPlayer()->getMoney() < _towerattributes->cost)
	{
		return;
	}

	bool wasBuilt = _field->setBuilding(new Tower(_field->getPosition(), _towerattributes));

	if(wasBuilt)
	{
		World::instance()->getMap()->getPlayer()->decreaseMoney(_towerattributes->cost);
		Hud::instance()->onPlayerUpdate();
	}
}