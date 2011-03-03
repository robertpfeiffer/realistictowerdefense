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

#include <osgParticle/SmokeEffect>
#include <osgParticle/FireEffect>
#include <osg/Geode>
#include <osg/ShapeDrawable>

#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>



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
	//Tower* t = (dynamic_cast<Tower*> (f->getContent()));

	if(f != NULL){

    // Ok folks, this is the first particle system we build; it will be
    // very simple, with no textures and no special effects, just default
    // values except for a couple of attributes.

    // First of all, we create the ParticleSystem object; it will hold
    // our particles and expose the interface for managing them; this object
    // is a Drawable, so we'll have to add it to a Geode later.

		osgParticle::ParticleSystem *ps = new osgParticle::ParticleSystem;

    // As for other Drawable classes, the aspect of graphical elements of
    // ParticleSystem (the particles) depends on the StateAttribute's we
    // give it. The ParticleSystem class has an helper function that let
    // us specify a set of the most common attributes: setDefaultAttributes().
    // This method can accept up to three parameters; the first is a texture
    // name (std::string), which can be empty to disable texturing, the second
    // sets whether particles have to be "emissive" (additive blending) or not;
    // the third parameter enables or disables lighting.

		ps->setDefaultAttributes("", true, false);

    // Now that our particle system is set we have to create an emitter, that is
    // an object (actually a Node descendant) that generate new particles at 
    // each frame. The best choice is to use a ModularEmitter, which allow us to
    // achieve a wide variety of emitting styles by composing the emitter using
    // three objects: a "counter", a "placer" and a "shooter". The counter must
    // tell the ModularEmitter how many particles it has to create for the
    // current frame; then, the ModularEmitter creates these particles, and for
    // each new particle it instructs the placer and the shooter to set its
    // position vector and its velocity vector, respectively.
    // By default, a ModularEmitter object initializes itself with a counter of
    // type RandomRateCounter, a placer of type PointPlacer and a shooter of
    // type RadialShooter (see documentation for details). We are going to leave
    // these default objects there, but we'll modify the counter so that it
    // counts faster (more particles are emitted at each frame).

		osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;

    // the first thing you *MUST* do after creating an emitter is to set the
    // destination particle system, otherwise it won't know where to create
    // new particles.

		emitter->setParticleSystem(ps);
	
    // Ok, get a pointer to the emitter's Counter object. We could also
    // create a new RandomRateCounter object and assign it to the emitter,
    // but since the default counter is already a RandomRateCounter, we
    // just get a pointer to it and change a value.

		osgParticle::RandomRateCounter *rrc = 
			static_cast<osgParticle::RandomRateCounter *>(emitter->getCounter());

    // Now set the rate range to a better value. The actual rate at each frame
    // will be chosen randomly within that range.

		rrc->setRateRange(20, 30);    // generate 20 to 30 particles per second

    // The emitter is done! Let's add it to the scene graph. The cool thing is
    // that any emitter node will take into account the accumulated local-to-world
    // matrix, so you can attach an emitter to a transform node and see it move.

		f->addChild(emitter);

    // Ok folks, we have almost finished. We don't add any particle modifier 
    // here (see ModularProgram and Operator classes), so all we still need is
    // to create a Geode and add the particle system to it, so it can be
    // displayed.

		osg::Geode *geode = new osg::Geode;    
		geode->addDrawable(ps);

    // add the geode to the scene graph

		f->addChild(geode);
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
		aMenu->addEntry(build_tower,
					"textures/tower.png");
	}

	//if (hasTower()){
	aMenu->addEntry(fire_powerup,
					"textures/fire.png");
	//}

	aMenu->addEntry(NULL,
					"textures/x.png");

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
