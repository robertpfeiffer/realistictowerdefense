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
    // Standard size shrub
	float width = 1.0f;
	float height = 1.0f;
	
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
	
	// Declare an array of vertices, assign values so we can create a
	// quadrilateral centered relative to the Z axis
	osg::Vec3Array* verts = new osg::Vec3Array(4);

	(*verts)[0] = osg::Vec3( x - width/2, 0, y - height/2);
	(*verts)[1] = osg::Vec3( x + width/2, 0, y - height/2);
	(*verts)[2] = osg::Vec3( x + width/2, 0, y + height/2);
	(*verts)[3] = osg::Vec3( x - width/2, 0, y + height/2);
	geometry->setVertexArray(verts);
	
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