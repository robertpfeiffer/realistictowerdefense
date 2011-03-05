// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#pragma once

#include <osg/Billboard>
#include <osg/Drawable>
#include <constants.h>

class HealthBar : public osg::Billboard
{
public:
	HealthBar();
	void setHealth(int health);
	void setMaxHealth(int health, int maxHealth);
private:
	void _setBillBoardStateSet();
	osg::Drawable* _createHealthGemoetry();
	osg::Drawable* _createBackgroundGemoetry();
	void _updateHealthBar();
	void _updateTextureSize(osg::Geometry* geometry, int health) const;
	void _updateGeometrySize(osg::Geometry* geometry, float offset, float length, float height) const;
	int _maxHealth;
	int _health;
	int _damage;
	osg::ref_ptr<osg::Geometry> _healthGeometry;
};
