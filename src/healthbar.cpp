// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-
#include <healthbar.h>

/*
  Health bar for a creep.
  It is a node in the scene graph below the creep.
  Visually, the health bar floats over the creeps head.
 */

#include <sstream>
#include <osg/BlendFunc>
#include <osg/Geometry>
#include <osg/AlphaFunc>
#include <osg/Texture2D>

#include <constants.h>
#include <assetlibrary.h>
#include <world.h>
#include <gametimer.h>

/**
 * \fn	HealthBar::HealthBar()
 *
 * \brief	Create the health bar.
 */
HealthBar::HealthBar()
{
	this->setMode(osg::Billboard::POINT_ROT_EYE);

	_health = 1;
	_maxHealth = 1;
	this->_setBillBoardStateSet();

	this->addDrawable(_createBackgroundGeometry());

	_healthGeometry = _createHealthGeometry(AssetLibrary::instance()->getTexture("healthbar/hp.png"));
	this->addDrawable(_healthGeometry);

	_damageGeometry = _createHealthGeometry(AssetLibrary::instance()->getTexture("healthbar/damage.png"));
	this->addDrawable(_damageGeometry);

	_updateHealthBar();
	_elapsedTimeSinceLastDamage = 0;
	World::instance()->registerForUpdates(this);
}

/**
 * \fn	void HealthBar::onUpdate()
 *
 * \brief	Update the damage animation.
 */
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

/**
 * \fn	int HealthBar::_damageLeft()
 *
 * \brief	Calculcate the damage left after elapsed time.
 *
 * \return	The left damage.
 */
int HealthBar::_damageLeft()
{
	if (_elapsedTimeSinceLastDamage < DAMAGE_DISPLAY_DURATION) return _damage;

	return _damage * (DAMAGE_DISPLAY_DECREASE_DURATION - (_elapsedTimeSinceLastDamage - DAMAGE_DISPLAY_DURATION)) / DAMAGE_DISPLAY_DECREASE_DURATION;
}

/**
 * \fn	void HealthBar::_setBillBoardStateSet()
 *
 * \brief	Sets the bill board state set.
 */
void HealthBar::_setBillBoardStateSet()
{
	osg::StateSet* billBoardStateSet = new osg::StateSet();	
	billBoardStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	billBoardStateSet->setAttributeAndModes( new osg::BlendFunc, osg::StateAttribute::ON );
	billBoardStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	this->setStateSet(billBoardStateSet);
}

/**
 * \fn	osg::Geometry* HealthBar::_createBackgroundGeometry() const
 *
 * \brief	Creates the background geometry.
 *
 * \return	The geometrye.
 */
osg::Geometry* HealthBar::_createBackgroundGeometry() const
{
	float width = 0.8f;
	float height = 0.12f;

	osg::Geometry* geometry = new osg::Geometry();

	osg::Texture2D* backgroundTexture = AssetLibrary::instance()->getTexture("healthbar/background.png");
	
	// Declare and initialize geometry
    geometry = new osg::Geometry();

	//add vertices and texture
	//set the y coordinate farther away from the camera
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

/**
 * \fn	osg::Geometry* HealthBar::_createHealthGemoetry(osg::Texture2D* texture) const
 *
 * \brief	Creates a health geometry.
 *
 * \param	texture	If non-null, the texture.
 *
 * \return	The geometry.
 */
osg::Geometry* HealthBar::_createHealthGeometry(osg::Texture2D* texture) const
{
	// Declare and initialize geometry
    osg::Geometry* geometry = new osg::Geometry();
	// the coordinates are set in _updateHealthBar

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

/**
 * \fn	void HealthBar::_updateHealthBar()
 *
 * \brief	Updates the health bar.
 */
void HealthBar::_updateHealthBar()
{
	// Standard size shrub
	float width = 0.75f;
	float height = 0.1f;

	float healthWidth = width * static_cast<float>(_health) / static_cast<float>(_maxHealth);
	float halfWidth = width/2;	
	
	// the green bar indicates remeining health
	_updateGeometrySize(_healthGeometry, -halfWidth, healthWidth, height);
	_updateTextureSize(_healthGeometry, _health, 0.0f);

	// the red bar indicates damage taken recently
    // on the same scale as the helath bar
	float damageWidth = width * static_cast<float>(_damage) / static_cast<float>(_maxHealth);
	_updateGeometrySize(_damageGeometry, -halfWidth + healthWidth, damageWidth, height);
	_updateTextureSize(_damageGeometry, _damage, healthWidth);
}

/**
 * \fn	void HealthBar::_updateGeometrySize(osg::Geometry* geometry, float offset, float length,
 * 		float height) const
 *
 * \brief	Updates the geometry size.
 *
 * \param	geometry	The geometry.
 * \param	offset		The offset.
 * \param	length		The length.
 * \param	height		The height.
 */
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

/**
 * \fn	void HealthBar::_updateTextureSize(osg::Geometry* geometry, int health,
 * 		float textureOffset) const
 *
 * \brief	Updates the texture size of a geometry.
 *
 * \param	geometry			The geometry.
 * \param	health				The health.
 * \param	textureOffset   	The texture offset.
 */
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

/**
 * \fn	void HealthBar::setHealth(int health)
 *
 * \brief	Set current health.
 *
 * \param	health	Current health.
 */
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

/**
 * \fn	void HealthBar::setMaxHealth(int health, int maxHealth)
 *
 * \brief	Set the maximum health.
 *
 * \param	health   	Current health.
 * \param	maxHealth	The maximum health.
 */
void HealthBar::setMaxHealth(int health, int maxHealth)
{
	_damage = 0;
	_health = health;
	_maxHealth = maxHealth;
	_updateHealthBar();
}
