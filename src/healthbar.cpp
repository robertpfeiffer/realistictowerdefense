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

	_health = 1;
	_maxHealth = 1;
	this->_setBillBoardStateSet();
	//this->addDrawable(_createBackgroundGemoetry());
	this->addDrawable(_createHealthGemoetry());
}

void HealthBar::_setBillBoardStateSet()
{
	osg::StateSet* billBoardStateSet = new osg::StateSet();	
	billBoardStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	billBoardStateSet->setAttributeAndModes( new osg::BlendFunc, osg::StateAttribute::ON );
	billBoardStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	// Make sure the geometry has the correct state
	this->setStateSet(billBoardStateSet);
}

osg::Drawable* HealthBar::_createBackgroundGemoetry()
{
	float width = 0.8f;
	float height = 0.12f;

	osg::Geometry* backgroundGeometry = new osg::Geometry();

	osg::Texture2D* backgroundTexture = AssetLibrary::instance()->getTexture("healthbar/background.png");
	
	// Declare and initialize geometry
    backgroundGeometry = new osg::Geometry();

	//add vertices and texture
	osg::Vec3Array* verts = new osg::Vec3Array(4);	
	(*verts)[0] = osg::Vec3( -width/2, 0, -height/2);
	(*verts)[1] = osg::Vec3( +width/2, 0, -height/2);
	(*verts)[2] = osg::Vec3( +width/2, 0, +height/2);
	(*verts)[3] = osg::Vec3( -width/2, 0, +height/2);
	backgroundGeometry->setVertexArray(verts);

	
	osg::Vec2Array* texCoords = new osg::Vec2Array(4);
	(*texCoords)[0].set(0.0f, 0.0f);
	(*texCoords)[1].set(1.0f, 0.0f);
	(*texCoords)[2].set(1.0f, 1.0f);
	(*texCoords)[3].set(0.0f, 1.0f);
	backgroundGeometry->setTexCoordArray(0, texCoords);

	osg::StateSet* stateSet = new osg::StateSet();
	stateSet->setTextureAttributeAndModes(0, backgroundTexture, osg::StateAttribute::ON);
	backgroundGeometry->setStateSet(stateSet);

	// Add a primitive set (QUADS) to the geometry
	backgroundGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	backgroundGeometry->setColorArray(colors);
	backgroundGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	
	return backgroundGeometry;
}

osg::Drawable* HealthBar::_createHealthGemoetry()
{
	osg::Texture2D* healthTexture = AssetLibrary::instance()->getTexture("healthbar/hp.png");
	
	// Declare and initialize geometry
    _healthGeometry = new osg::Geometry();

	//add vertices and texture
	_healthGeometry->setVertexArray(new osg::Vec3Array(4));	
	_healthGeometry->setTexCoordArray(0, new osg::Vec2Array(4));
	_updateHealthBar();
	osg::StateSet* stateSet = new osg::StateSet();
	stateSet->setTextureAttributeAndModes(0, healthTexture, osg::StateAttribute::ON);
	_healthGeometry->setStateSet(stateSet);

	// Add a primitive set (QUADS) to the geometry
	_healthGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	
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

	float healthWidth = width * static_cast<float>(_health) / static_cast<float>(_maxHealth);

	//update width of texture (reuse array)
	osg::Vec3Array* verts = (osg::Vec3Array*) _healthGeometry->getVertexArray();
	(*verts)[0] = osg::Vec3( -width/2, 0, -height/2);
	(*verts)[1] = osg::Vec3( -width/2 + healthWidth, 0, -height/2);
	(*verts)[2] = osg::Vec3( -width/2 + healthWidth, 0, height/2);
	(*verts)[3] = osg::Vec3( -width/2, 0, height/2);
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
