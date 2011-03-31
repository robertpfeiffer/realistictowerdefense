// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <towerbuildbutton.h>

#include <field.h>
#include <hatchery.h>
#include <hud.h>
#include <inscenetext.h>
#include <map.h>
#include <tower.h>
#include <towerattributes.h>
#include <towerinfobox.h>
#include <world.h>

/**
 * \fn	TowerBuildButton::TowerBuildButton(Field* field, TowerAttributes* attributes)
 *
 * \brief	Create a build button for tower.
 *
 * \param	field	  	The field.
 * \param	attributes	The attributes.
 */
TowerBuildButton::TowerBuildButton(Field* field, TowerAttributes* attributes) : MenuButton(attributes->icon)
{
	_field = field;
	_towerattributes = attributes;
}

/**
 * \fn	void TowerBuildButton::onClick(osgGA::GUIActionAdapter& aa)
 *
 * \brief	Try to build the tower.
 *
 * \param [in,out]	aa	The GUIActionAdapter.
 */
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

	bool wasBuilt = _field->setBuilding(new Tower(_field->getPosition(), _towerattributes));

	if(wasBuilt)
	{
		World::instance()->getMap()->getPlayer()->decreaseMoney(_towerattributes->cost);
		_towerattributes->stock--;
	}
}

/**
 * \fn	void TowerBuildButton::onHover(osgGA::GUIActionAdapter& aa)
 *
 * \brief	Show information box, if mouse hover button.
 *
 * \param [in,out]	aa	The GUIActionAdapter.
 */

void TowerBuildButton::onHover(osgGA::GUIActionAdapter& aa)
{
	Hud::instance()->pushInfoBox(new TowerInfoBox(_towerattributes));
}

/**
 * \fn	void TowerBuildButton::onUnhover()
 *
 * \brief	Remove infobox, if mouse is moved away.
 */
void TowerBuildButton::onUnhover()
{
	Hud::instance()->popInfoBox();
}