// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-

#include <contextmenu.h>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osg/Texture2D>
#include <assetlibrary.h>


ContextMenu::ContextMenu()
{
	this->_numberOfEntries = 0;
	this->setMode(osg::Billboard::POINT_ROT_EYE);
	this->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
	this->addEntry(NULL,"select.png");
}

MenuButton* ContextMenu::createMenuItem(osg::StateSet* bbState)
{
    // Standard size shrub
	float width = 1.0f;
	float height = 1.0f;
	
	// Declare and initialize geometry
	MenuButton* geometry = new MenuButton(bbState);
	
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
	osg::StateSet* stateSet = createStateSetFromTexturePath(texturepath);
	MenuButton* drawable = createMenuItem(stateSet);

	drawable->_onClick = _onClick;
    this->_numberOfEntries++;
	this->addDrawable(drawable);
}

osg::StateSet* ContextMenu::createStateSetFromTexturePath(const std::string texturepath)
{
	osg::StateSet* stateSet = new osg::StateSet;

	stateSet->setTextureAttributeAndModes(0, AssetLibrary::instance()->getTexture(texturepath), osg::StateAttribute::ON);
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateSet->setAttributeAndModes(new osg::BlendFunc(), osg::StateAttribute::ON);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF); //always on top

	return stateSet;
}