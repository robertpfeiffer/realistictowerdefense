// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <field.h>
#include <modeldata.h>
#include <tower.h>
#include <world.h>
#include <iostream>
#include <stdlib.h>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgDB/ReadFile>

#include <osgDB/Registry>
#include <constants.h>
#include <contextmenu.h>

Field::Field(FieldType* fieldType) : _isBuildable(fieldType->isBuildable()), _ground(fieldType->getGround()), _fieldType(fieldType)
{
	this->addChild(_ground.get());

	ModelData* modelData = fieldType->getModelData();

	_model = NULL;
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
		}
	}
}

void build_tower(osg::ref_ptr<MenuButton> button)
{
	Field* f = (dynamic_cast<Field*> (button->getParent(0)->getParent(0)));

	std::cout << "build tower" << std::endl;
	
	if(f != NULL){
		f->setBuilding();
	}
}

void Field::onFocus(osgGA::GUIActionAdapter& aa)
{
	Contextmenu* aMenu= new Contextmenu();

	aMenu->addEntry(build_tower,
					"textures/tower.png");

	aMenu->addEntry(NULL,
					"textures/x.png");

	this->addChild(aMenu);
	this->_menu=aMenu;
	std::cout << "open menu" << std::endl;

}

void Field::onClick(osgGA::GUIActionAdapter& aa)
{
  //handled by menubutton
}

void Field::onBlur()
{
	std::cout << "close menu" << std::endl;
	if(this->_menu != NULL)
		this->removeChild(this->_menu);
	this->_menu = NULL;
}

bool Field::setBuilding()
{
	if (!_fieldType->isBuildable())
	{
	  	return false;
	}
	Tower* t = new Tower(this->getPosition(), World::instance()->getMap()->getTowerAttributes()->front());
	this->addChild(t);
	World::instance()->registerForUpdates(t);

	return true;
}

float Field::_getRandomFloat(float min, float max)
{
	return min + ((max - min) * (float) rand() / RAND_MAX);
}
