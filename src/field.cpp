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
#include <contextmenu.h>

#include <osg/Geode>
#include <osg/ShapeDrawable>

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

void build_tower(osg::ref_ptr<MenuButton> button)
{
	Field* f = (dynamic_cast<Field*> (button->getParent(0)->getParent(0)));
	
	if(f != NULL){
		f->setBuilding();
	}
}

void fire_powerup(osg::ref_ptr<MenuButton> button)
{
	Field* f = (dynamic_cast<Field*> (button->getParent(0)->getParent(0)));
	Tower* t = (dynamic_cast<Tower*> (f->getContent()));

	if(f != NULL){

/*
        osgParticle::SmokeEffect* fire = new osgParticle::SmokeEffect(osg::Vec3(20.0f,20.0f,20.0f), 5.0f, 5.0f);
		fire->setWind(osg::Vec3(1.0f,1.0f,0.0f));
		f->addChild(fire);

        osg::Geode* geode = new osg::Geode;
        geode->addDrawable(fire->getParticleSystem());
		f->addChild(geode);*/
	}
}

void Field::onFocus(osgGA::GUIActionAdapter& aa)
{
	Contextmenu* aMenu= new Contextmenu();

	if (isBuildable()){
		aMenu->addEntry(build_tower, "tower.png");
	}

	if (hasTower()){
	aMenu->addEntry(fire_powerup, "fire.png");
	}

	aMenu->addEntry(NULL, "x.png");

	this->addChild(aMenu);
	this->_menu=aMenu;
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
	return _fieldType->isBuildable() && this->_content.get() == NULL;
}

bool Field::hasTower()
{
	return _fieldType->isBuildable() && (dynamic_cast<Tower*>(_content.get()) != NULL);
}

osg::Node* Field::getContent()
{
	return _content;
}

bool Field::setBuilding()
{
	if (!this->isBuildable())
	  	return false;

	osg::ref_ptr<Tower> t = new Tower(this->getPosition(), World::instance()->getMap()->getTowerAttributes()->front());
	_content = t;

	this->addChild(t);
	World::instance()->registerForUpdates(t);
	_isBuildable = false;

	return true;
}

float Field::_getRandomFloat(float min, float max)
{
	return min + ((max - min) * (float) rand() / RAND_MAX);
}
