#include <towerupgradebutton.h>

#include <hatchery.h>
#include <hud.h>
#include <inscenetext.h>
#include <map.h>
#include <tower.h>
#include <towerattributes.h>
#include <towerinfobox.h>
#include <world.h>

/**
 * \fn	TowerUpgradeButton::TowerUpgradeButton(Tower* tower, TowerAttributes* attributes)
 *
 * \brief	Create a upgrade button for a twoer.
 *
 * \param	tower	  	The tower.
 * \param	attributes	The attributes.
 */
TowerUpgradeButton::TowerUpgradeButton(Tower* tower, TowerAttributes* attributes) : MenuButton(attributes->icon)
{
	_tower = tower;
	_towerattributes = attributes;
}

/**
 * \fn	void TowerUpgradeButton::onClick(osgGA::GUIActionAdapter& aa)
 *
 * \brief	Execute the click action.
 *
 * \param	aa	The GUIActionAdapter.
 */
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

/**
 * \fn	void TowerUpgradeButton::onHover(osgGA::GUIActionAdapter& aa)
 *
 * \brief	Show information box, if mouse hover button.
 *
 * \param [in,out]	aa	The GUIActionAdapter.
 */
void TowerUpgradeButton::onHover(osgGA::GUIActionAdapter& aa)
{
	Hud::instance()->pushInfoBox(new TowerInfoBox(_towerattributes));
}

/**
 * \fn	void TowerUpgradeButton::onUnhover()
 *
 * \brief	Remove infobox, if mouse is moved away.
 */
void TowerUpgradeButton::onUnhover()
{
	Hud::instance()->popInfoBox();
}