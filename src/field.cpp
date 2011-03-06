// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <field.h>
#include <modeldata.h>
#include <tower.h>
#include <world.h>
#include <iostream>
#include <stdlib.h>

#include <osg/Node>
#include <osg/Group>

#include <constants.h>
#include <fieldcontextmenu.h>

Field::Field(FieldType* fieldType) : _isBuildable(fieldType->isBuildable()), _ground(fieldType->getGround()), _fieldType(fieldType)
{
	this->addChild(_ground.get());

	ModelData* modelData = fieldType->getModelData();

	_content = NULL;
	if (modelData != NULL)
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
	this->_menu = new FieldContextMenu(this);
	this->addChild(_menu);
}

void Field::onClick(osgGA::GUIActionAdapter& aa)
{
  //handled by menubutton
}

void Field::onBlur()
{
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
	  	return false;

	if(_content != NULL)
	{
		this->removeChild(_content);
	}

	_content = tower;
	this->addChild(tower);
	World::instance()->registerForUpdates(tower);
	_isBuildable = false;

	return true;
}

float Field::_getRandomFloat(float min, float max)
{
	return min + ((max - min) * (float) rand() / RAND_MAX);
}
