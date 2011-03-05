// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-

#include <sstream>
#include <osg/BlendFunc>
#include <osg/Geometry>
#include <osg/AlphaFunc>
#include <osg/Texture2D>

#include <constants.h>
#include <healthbar.h>
#include <assetlibrary.h>
#include <world.h>
#include <gametimer.h>

HealthBar::HealthBar()
{
	this->setMode(osg::Billboard::POINT_ROT_EYE);

	_health = 1;
	_maxHealth = 1;
	this->_setBillBoardStateSet();

	this->addDrawable(_createBackgroundGemoetry());

	_healthGeometry = _createHealthGemoetry(AssetLibrary::instance()->getTexture("healthbar/hp.png"));
	this->addDrawable(_healthGeometry);

	_damageGeometry = _createHealthGemoetry(AssetLibrary::instance()->getTexture("healthbar/damage.png"));
	this->addDrawable(_damageGeometry);

	_updateHealthBar();
	_elapsedTimeSinceLastDamage = 0;
	World::instance()->registerForUpdates(this);
}

void HealthBar::onUpdate()
{
	if (_damage == 0) return;

	_elapsedTimeSinceLastDamage += GameTimer::instance()->elapsedTime();

	if (_elapsedTimeSinceLastDamage > DAMAGE_DISPLAY_DURATION)
	{
		_damage = 0;
	}

	_updateHealthBar();
}

int HealthBar::_damageLeft()
{
	if (_elapsedTimeSinceLastDamage < DAMAGE_DISPLAY_DURATION) return _damage;

	return _damage * (DAMAGE_DISPLAY_DECREASE_DURATION - (_elapsedTimeSinceLastDamage - DAMAGE_DISPLAY_DURATION)) / DAMAGE_DISPLAY_DECREASE_DURATION;
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

osg::Geometry* HealthBar::_createBackgroundGemoetry() const
{
	float width = 0.8f;
	float height = 0.12f;

	osg::Geometry* geometry = new osg::Geometry();

	osg::Texture2D* backgroundTexture = AssetLibrary::instance()->getTexture("healthbar/background.png");
	
	// Declare and initialize geometry
    geometry = new osg::Geometry();

	//add vertices and texture
	osg::Vec3Array* verts = new osg::Vec3Array(4);	
	(*verts)[0] = osg::Vec3( -width/2, 0.01f, -height/2);
	(*verts)[1] = osg::Vec3( +width/2, 0.01f, -height/2);
	(*verts)[2] = osg::Vec3( +width/2, 0.01f, +height/2);
	(*verts)[3] = osg::Vec3( -width/2, 0.01f, +height/2);
	geometry->setVertexArray(verts);

	
	osg::Vec2Array* texCoords = new osg::Vec2Array(4);
	(*texCoords)[0].set(0.0f, 0.0f);
	(*texCoords)[1].set(1.0f, 0.0f);
	(*texCoords)[2].set(1.0f, 1.0f);
	(*texCoords)[3].set(0.0f, 1.0f);
	geometry->setTexCoordArray(0, texCoords);

	osg::StateSet* stateSet = new osg::StateSet();
	stateSet->setTextureAttributeAndModes(0, backgroundTexture, osg::StateAttribute::ON);
	geometry->setStateSet(stateSet);

	// Add a primitive set (QUADS) to the geometry
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	
	return geometry;
}

osg::Geometry* HealthBar::_createHealthGemoetry(osg::Texture2D* texture) const
{
	// Declare and initialize geometry
    osg::Geometry* geometry = new osg::Geometry();

	//add vertices and texture
	geometry->setVertexArray(new osg::Vec3Array(4));	
	geometry->setTexCoordArray(0, new osg::Vec2Array(4));
	
	osg::StateSet* stateSet = new osg::StateSet();
	stateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	geometry->setStateSet(stateSet);

	// Add a primitive set (QUADS) to the geometry
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	
	return geometry;
}

void HealthBar::_updateHealthBar()
{
	// Standard size shrub
	float width = 0.75f;
	float height = 0.1f;

	float healthWidth = width * static_cast<float>(_health) / static_cast<float>(_maxHealth);
	float halfWidth = width/2;	

	_updateGeometrySize(_healthGeometry, -halfWidth, healthWidth, height);
	_updateTextureSize(_healthGeometry, _health, 0.0f);

	float damageWidth = width * static_cast<float>(_damage) / static_cast<float>(_maxHealth);
	_updateGeometrySize(_damageGeometry, -halfWidth + healthWidth, damageWidth, height);
	_updateTextureSize(_damageGeometry, _damage, healthWidth);
}

void HealthBar::_updateGeometrySize(osg::Geometry* geometry, float offset, float length, float height) const
{
	float halfHeight = height/2;

	//update width of texture (reuse array, because we know how much vertices it has)
	osg::Vec3Array* verts = (osg::Vec3Array*) geometry->getVertexArray();	
	(*verts)[0] = osg::Vec3( offset + length, 0, -halfHeight);
	(*verts)[1] = osg::Vec3( offset + length, 0,  halfHeight);
	(*verts)[2] = osg::Vec3( offset, 0,  halfHeight);
	(*verts)[3] = osg::Vec3( offset, 0, -halfHeight);
	geometry->setVertexArray(verts);
}

void HealthBar::_updateTextureSize(osg::Geometry* geometry, int health, float textureOffset) const
{
	//update Texture width (reuse array, because we know how much vertices it has)
	osg::Vec2Array* texCoords = (osg::Vec2Array*) geometry->getTexCoordArray(0);	
	(*texCoords)[0].set(textureOffset + (float)health / 100.0f, 0.0f);
	(*texCoords)[1].set(textureOffset + (float)health / 100.0f, 1.0f);
	(*texCoords)[2].set(textureOffset, 1.0f);
	(*texCoords)[3].set(textureOffset, 0.0f);
	geometry->setTexCoordArray(0, texCoords);
}

void HealthBar::setHealth(int health)
{
	if (health < _health)
	{
		_damage = _damageLeft() + (_health - health);
		_elapsedTimeSinceLastDamage = 0;
	}
	_health = health;
	_updateHealthBar();
}

void HealthBar::setMaxHealth(int health, int maxHealth)
{
	_damage = 0;
	_health = health;
	_maxHealth = maxHealth;
	_updateHealthBar();
}
