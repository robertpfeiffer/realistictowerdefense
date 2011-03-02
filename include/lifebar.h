// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#pragma once

#include <osg/Billboard>
#include <osg/Drawable>
#include <constants.h>

class LifeBar : public osg::Billboard
{
public:
	LifeBar();
	osg::Drawable* createGeometry(osg::StateSet* state);
	osg::Drawable* _lifeDrawable;
	void setHealth(float health);
};
