// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <towerbuildbutton.h>

#include <field.h>
#include <hud.h>
#include <tower.h>
#include <towerattributes.h>
#include <world.h>
#include <inscenetext.h>
#include <hatchery.h>

TowerBuildButton::TowerBuildButton(Field* field, TowerAttributes* attributes) : MenuButton(attributes->icon)
{
	_field = field;
	_towerattributes = attributes;
}

void TowerBuildButton::onClick(osgGA::GUIActionAdapter& aa)
{
	if(World::instance()->getMap()->getPlayer()->getMoney() < _towerattributes->cost)
	{
		InSceneText* text = new InSceneText("TOO EXPENSIVE", _field->getPosition());
		text->setColor(osg::Vec3(1.0, 0.0, 0.0));
		Hatchery::instance()->enqueueChild(text);
		return;
	}

	if(_towerattributes->stock <= 0)
	{
		InSceneText* text = new InSceneText("OUT OF STOCK", _field->getPosition());
		text->setColor(osg::Vec3(1.0, 0.0, 0.0));
		Hatchery::instance()->enqueueChild(text);
		return;
	}

	_towerattributes->stock--;

	bool wasBuilt = _field->setBuilding(new Tower(_field->getPosition(), _towerattributes));

	if(wasBuilt)
	{
		World::instance()->getMap()->getPlayer()->decreaseMoney(_towerattributes->cost);
		Hud::instance()->onPlayerUpdate();
	}
}
