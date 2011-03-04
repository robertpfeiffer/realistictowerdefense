// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-

#include <healthbar.h>
#include <sstream>
#include <osg/BlendFunc>
#include <osg/Geometry>
#include <osg/AlphaFunc>
#include <osg/Texture2D>
#include <assetlibrary.h>

HealthBar::HealthBar()
{
	this->setMode(osg::Billboard::POINT_ROT_EYE);
	this->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));

	_health = 1;
	_maxHealth = 1;
	this->addDrawable(_createHealthGemoetry());
}

osg::Drawable* HealthBar::_createHealthGemoetry()
{
	osg::Texture2D* healthTexture = AssetLibrary::instance()->getTexture("healthbar/hp.png");

	osg::StateSet* billBoardStateSet = new osg::StateSet;	
	billBoardStateSet->setTextureAttributeAndModes(0, healthTexture, osg::StateAttribute::ON);
	billBoardStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	billBoardStateSet->setAttributeAndModes( new osg::BlendFunc, osg::StateAttribute::ON );
	billBoardStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	
	// Declare and initialize geometry
    _healthGeometry = new osg::Geometry();

	//add vertices and texture
	_healthGeometry->setVertexArray(new osg::Vec3Array(4));	
	_healthGeometry->setTexCoordArray(0, new osg::Vec2Array(4));
	_updateHealthBar();

	// Add a primitive set (QUADS) to the geometry
	_healthGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	
	// Make sure the geometry has the correct state
	_healthGeometry->setStateSet(billBoardStateSet);
	
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_healthGeometry->setColorArray(colors);
	_healthGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	
	return _healthGeometry;
}

void HealthBar::_updateHealthBar()
{
	// Standard size shrub
	float width = 0.75f;
	float height = 0.1f;
	float y = 2.0;

	float healthWidth = width * static_cast<float>(_health) / static_cast<float>(_maxHealth);

	//update width of texture (reuse array)
	osg::Vec3Array* verts = (osg::Vec3Array*) _healthGeometry->getVertexArray();
	(*verts)[0] = osg::Vec3( -width/2, 0, y - height/2);
	(*verts)[1] = osg::Vec3( -width/2 + healthWidth, 0, y - height/2);
	(*verts)[2] = osg::Vec3( -width/2 + healthWidth, 0, y + height/2);
	(*verts)[3] = osg::Vec3( -width/2, 0, y + height/2);
	_healthGeometry->setVertexArray(verts);

	//update Texture width (reuse array)
	osg::Vec2Array* texCoords = (osg::Vec2Array*) _healthGeometry->getTexCoordArray(0);
	(*texCoords)[0].set(0.0f, 0.0f);
	(*texCoords)[1].set((float)_health / 100.0f, 0.0f);
	(*texCoords)[2].set((float)_health / 100.0f, 1.0f);
	(*texCoords)[3].set(0.0f, 1.0f);
	_healthGeometry->setTexCoordArray(0, texCoords);
}

void HealthBar::setHealth(int health)
{
	_health = health;
	_updateHealthBar();
}

void HealthBar::setMaxHealth(int health, int maxHealth)
{
	_health = health;
	_maxHealth = maxHealth;
	_updateHealthBar();
}
