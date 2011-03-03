// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-

#include <healthbar.h>
#include <stdio.h>
#include <iostream>
#include <osg/BlendFunc>
#include <osg/Geometry>
#include <osg/AlphaFunc>
#include <osg/Texture2D>
#include <assetlibrary.h>

HealthBar::HealthBar()
{
	this->setMode(osg::Billboard::POINT_ROT_EYE);
	this->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
	this->setHealth(1.0);
}

osg::Drawable* HealthBar::createGeometry(osg::StateSet* bbState)
{
	// Standard size shrub
	float width = 1.0f;
	float height = 1.0f;
	
	// Declare and initialize geometry
    osg::Geometry* geometry = new osg::Geometry();
	
	// Declare an array of vertices, assign values so we can create a
	// quadrilateral centered relative to the Z axis
	osg::Vec3Array* verts = new osg::Vec3Array(4);
	
	float x = 0;
	float y = 2;
	
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

void HealthBar::setHealth(float health)
{

	char path[25];
	float h = health;

    int barstate = 0;
	do {
		barstate++;
		h -= 0.12;
	} while (h>=0);
	sprintf(path,"textures/lifebar/%d.png",barstate);

	AssetLibrary *lib = AssetLibrary::instance();
	osg::Texture2D *texture;
	texture = lib->getTexture(path);
	
	osg::BlendFunc *blendFunc = new osg::BlendFunc;
	osg::StateSet* billBoardStateSet = new osg::StateSet;
	
	billBoardStateSet->setTextureAttributeAndModes
		(0, texture, osg::StateAttribute::ON);
	billBoardStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	billBoardStateSet->setAttributeAndModes( blendFunc, osg::StateAttribute::ON );
	billBoardStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	
	osg::Drawable* drawable = createGeometry(billBoardStateSet);

	if(this->_lifeDrawable != NULL)
		this->removeDrawable(this->_lifeDrawable);
	
	this->_lifeDrawable = drawable;
	this->addDrawable(drawable);
}







