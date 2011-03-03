#include <fieldmenubutton.h>

#include <field.h>
#include <tower.h>
#include <towerattributes.h>

FieldMenuButton::FieldMenuButton(Field* field, TowerAttributes* attributes) : MenuButton(attributes->icon)
{
	_field = field;
	_towerattributes = attributes;
}

void FieldMenuButton::onClick(osgGA::GUIActionAdapter& aa)
{
	_field->setBuilding(new Tower(_field->getPosition(), _towerattributes));
}