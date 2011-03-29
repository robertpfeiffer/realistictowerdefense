// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <field.h>

/*
  A field is a scenegraph node.
  It represents a ground tile and what is built on it.
 */

#include <iostream>
#include <stdlib.h>

#include <osg/Node>
#include <osg/Group>

#include <constants.h>
#include <fieldcontextmenu.h>
#include <fieldtype.h>
#include <modeldata.h>
#include <tower.h>
#include <towerattributes.h>
#include <world.h>

Field::Field(FieldType* fieldType) : _isBuildable(fieldType->isBuildable()), _ground(fieldType->getGround()), _fieldType(fieldType)
{
	this->addChild(_ground.get());

	ModelData* modelData = fieldType->getModelData();


	/*
	  Scenery like rocks or vegetation is randomly placed on the field.
	  It is randomly rotated and scaled, within configurable bounds.
	 */

	_content = NULL;
	if (modelData != NULL) // is scenery configured for this field type?
	{
		if (modelData->probability >= (float) rand() / RAND_MAX)
		{
			osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform();
			transform->addChild(modelData->model);

			osg::StateSet* state = new osg::StateSet();
			state->setMode( GL_RESCALE_NORMAL, osg::StateAttribute::ON );
			transform->setStateSet(state);

			float scale = _getRandomFloat(modelData->minScale, modelData->maxScale);
			transform->setScale(osg::Vec3d(scale, scale, scale));

			float rotation =  _getRandomFloat(modelData->minRotation, modelData->maxRotation);
			transform->setAttitude(osg::Quat(osg::DegreesToRadians(rotation), osg::Vec3d(0.0, 0.0, 1.0)));

			this->addChild(transform);
			_content = transform;
			_isBuildable = false;
		}
	}
}

void Field::onFocus(osgGA::GUIActionAdapter& aa)
{
    //add Context menu into the scenegraph if clicked
	this->_menu = new FieldContextMenu(this);
	this->addChild(_menu);
}

void Field::onClick(osgGA::GUIActionAdapter& aa)
{
    //handled by menubutton
}

void Field::onBlur()
{
    //remove Context menu on lost focus
	if(this->_menu != NULL)
		this->removeChild(this->_menu);
	this->_menu = NULL;
}

bool Field::isBuildable()
{
	return _fieldType->isBuildable() && !hasTower();
}

bool Field::hasTower()
{
	return _fieldType->isBuildable() && (dynamic_cast<Tower*>(_content.get()) != NULL);
}

osg::Node* Field::getContent()
{
	return _content;
}

bool Field::setBuilding(Tower* tower)
{
	if (!this->isBuildable() || this->hasTower())
		return false; // failure, can't build

	// remove scenery to make way for the building
	if(_content != NULL)
	{
		this->removeChild(_content);
	}

	_content = tower;
	this->addChild(tower);
	World::instance()->registerForUpdates(tower);

	reset();
	
	return true; //success
}

/*
  Put the tower into the scenegraph again.
  This is a HACK.
  We needed this to guarantee a certain traversal order
  for the animation and particle systems.
*/
void Field::reset(){
	this->getParent(0)->addChild(this);
	this->getParent(0)->removeChild(this);
}

bool Field::destroyBuilding()
{
	if (!this->hasTower())
	{
		return false;
	}

	Tower *tower = dynamic_cast<Tower*>(_content.get());	
	tower->getAttributes()->stock++;
	this->removeChild(tower);
	this->_content = NULL;

	return true;
}

float Field::_getRandomFloat(float min, float max)
{
	return min + ((max - min) * (float) rand() / RAND_MAX);
}
