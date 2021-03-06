// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <contextmenu.h>

#include <osg/AlphaFunc>
#include <osg/Texture2D>

#include <constants.h>
#include <menubutton.h>

/**
 * \fn	ContextMenu::ContextMenu()
 *
 * \brief	Create a context menu.
 */

ContextMenu::ContextMenu()
{
	this->_numberOfEntries = 0;
	this->setMode(osg::Billboard::POINT_ROT_EYE);
	this->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
	this->addEntry(NULL,"select.png");
	// always on top
	this->getOrCreateStateSet()->setRenderBinDetails(11, "RenderBin");
}

/**
 * \fn	osg::Vec2 ContextMenu::positionForNextButton()
 *
 * \brief	Gets the position for next button.
 * 			Arrange menu buttons radially.
 * 			The first menu item is drawn in the center of the circle
 *
 * \return	The position for the button relative to the context menu billboard.
 */
osg::Vec2 ContextMenu::positionForNextButton()
{	
	float x;
	float y;
	
	if(_numberOfEntries == 0){
		x=0;
		y=0;
	} else {
		// the entries are positioned 1.5 units from the center and
		// 45 degrees away from each other
		// this means there can only be 8 menu buttons or they overlap
		x = 1.5 *  sin(_numberOfEntries * PI/4);
		y = 1.5 * -cos(_numberOfEntries * PI/4);
	}
	
	return osg::Vec2(x, y);
}

/**
 * \fn	void ContextMenu::addEntry(void (* callback)(osg::ref_ptr<MenuButton>),
 * 		const std::string texturepath)
 *
 * \brief	Create a menubutton in the menu with an Icon and an action when clicked.
 *
 * \param	callback	The callback.
 * \param	texturepath	The path of texture.
 */
void ContextMenu::addEntry(void (* callback)(osg::ref_ptr<MenuButton>), const std::string texturepath)
{
	MenuButton* button = new MenuButton(texturepath);

	button->setCallback(callback);
	this->addEntry(button);
}

/**
 * \fn	void ContextMenu::addEntry(MenuButton* button)
 *
 * \brief	Adds an entry to the context menu. 
 *
 * \param	button	The button.
 */
void ContextMenu::addEntry(MenuButton* button)
{
	button->setPosition(positionForNextButton());
	this->_numberOfEntries++;
	this->addDrawable(button);
}
