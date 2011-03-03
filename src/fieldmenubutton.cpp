#include <fieldmenubutton.h>

#include <field.h>
#include <tower.h>
#include <towerattributes.h>
#include <world.h>

FieldMenuButton::FieldMenuButton(Field* field, TowerAttributes* attributes) : MenuButton(attributes->icon)
{
	_field = field;
	_towerattributes = attributes;
}

void FieldMenuButton::onClick(osgGA::GUIActionAdapter& aa)
{
	if(World::instance()->getMap()->getPlayer()->getMoney() < _towerattributes->cost)
	{
		return;
	}

	bool wasBuilt = _field->setBuilding(new Tower(_field->getPosition(), _towerattributes));

	if(wasBuilt)
	{
		World::instance()->getMap()->getPlayer()->decreaseMoney(_towerattributes->cost);
	}
}