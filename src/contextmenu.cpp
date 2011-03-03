// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-

#include <contextmenu.h>
#include <osg/AlphaFunc>
#include <osg/Texture2D>


ContextMenu::ContextMenu()
{
	this->_numberOfEntries = 0;
	this->setMode(osg::Billboard::POINT_ROT_EYE);
	this->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
	this->addEntry(NULL,"select.png");
}

MenuButton* ContextMenu::createMenuItem(const std::string texturepath)
{
	// Declare and initialize geometry
	MenuButton* geometry = new MenuButton(texturepath);
	
	float x;
	float y;
	
	if(_numberOfEntries == 0){
		x=0;
		y=0;
	} else {
		x = 1.5 *  sin(_numberOfEntries * PI/4);
		y = 1.5 * -cos(_numberOfEntries * PI/4);
	}
	
	geometry->setPosition(osg::Vec2(x, y));
	
	return geometry;
}

void ContextMenu::addEntry(void (* _onClick)(osg::ref_ptr<MenuButton>),
						   const std::string texturepath)
{
	MenuButton* drawable = createMenuItem(texturepath);

	drawable->_onClick = _onClick;
    this->_numberOfEntries++;
	this->addDrawable(drawable);
}