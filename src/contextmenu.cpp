// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-

#include <contextmenu.h>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osg/Texture2D>
#include <assetlibrary.h>


Contextmenu::Contextmenu()
{
	this->numberOfEntries = 0;
	this->setMode(osg::Billboard::POINT_ROT_EYE);
	this->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
	this->addEntry(NULL,"select.png");
}

MenuButton* Contextmenu::createMenuItem(osg::StateSet* bbState, int offset)
{
    // Standard size shrub
	float width = 1.0f;
	float height = 1.0f;
	
	// Declare and initialize geometry
	MenuButton* geometry = new MenuButton;
	
	// Declare an array of vertices, assign values so we can create a
	// quadrilateral centered relative to the Z axis
	osg::Vec3Array* verts = new osg::Vec3Array(4);
	
	float x;
	float y;
	
	if(offset == 0){
		x=0;
		y=0;
	} else {
		x = 1.5 *  sin(offset*PI/4);
		y = 1.5 * -cos(offset*PI/4);
	}
	
	(*verts)[0] = osg::Vec3( x - width/2, 0, y - height/2);
	(*verts)[1] = osg::Vec3( x + width/2, 0, y - height/2);
	(*verts)[2] = osg::Vec3( x + width/2, 0, y + height/2);
	(*verts)[3] = osg::Vec3( x - width/2, 0, y + height/2);
	geometry->setVertexArray(verts);
	
	// Declare and assign texture coordinates.
	osg::Vec2Array* texCoords = new osg::Vec2Array(4);
	(*texCoords)[0].set(0.0f,0.0f);
	(*texCoords)[1].set(1.0f,0.0f);
	(*texCoords)[2].set(1.0f,1.0f);
	(*texCoords)[3].set(0.0f,1.0f);
	geometry->setTexCoordArray(0,texCoords);
	
	// Add a primitive set (QUADS) to the geometry
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	
	// Make sure the geometry has the correct state
	geometry->setStateSet(bbState);
	
	osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	
	return geometry;
}

void Contextmenu::addEntry(void (* _onClick)(osg::ref_ptr<MenuButton>),
						   const std::string texturepath)
{
	AssetLibrary *lib = AssetLibrary::instance();
	osg::Texture2D *texture;
	texture = lib->getTexture(texturepath);
 
	osg::BlendFunc *blendFunc = new osg::BlendFunc;
	osg::StateSet* billBoardStateSet = new osg::StateSet;

	billBoardStateSet->setTextureAttributeAndModes
		(0, texture, osg::StateAttribute::ON);
	billBoardStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	billBoardStateSet->setAttributeAndModes( blendFunc, osg::StateAttribute::ON );
	billBoardStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	
    //Always on top
	billBoardStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	MenuButton* drawable = createMenuItem(billBoardStateSet, this->numberOfEntries);

	drawable->_onClick=_onClick;
    this->numberOfEntries++;
	this->addDrawable(drawable);
}
