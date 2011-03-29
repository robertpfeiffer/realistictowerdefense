// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <contextmenu.h>

/*
  The Context Menu Widget.
  Context menues are usually directly attached into the scenegraph.
  The visual style is a radial Menu of Buttons around the object.
  The Buttons are facing the Player around the Object.
 */

#include <osg/AlphaFunc>
#include <osg/Texture2D>

#include <constants.h>
#include <menubutton.h>

ContextMenu::ContextMenu()
{
	this->_numberOfEntries = 0;
	this->setMode(osg::Billboard::POINT_ROT_EYE);
	this->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
	this->addEntry(NULL,"select.png");
       	this->getOrCreateStateSet()->setRenderBinDetails( 11, "RenderBin");
}

/*
  arranges menu buttons radially.
  The first menu item is drawn in the center of the circle.
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

void ContextMenu::addEntry(void (* callback)(osg::ref_ptr<MenuButton>),
						   const std::string texturepath)
{
	MenuButton* button = new MenuButton(texturepath);

	button->setCallback(callback);
	this->addEntry(button);
}

void ContextMenu::addEntry(MenuButton* button)
{
	button->setPosition(positionForNextButton());
	this->_numberOfEntries++;
	this->addDrawable(button);
}
