// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#pragma once

#include <osg/Billboard>
#include <osg/Drawable>
#include <osg/Texture2D>
#include <updatablenode.h>

class HealthBar : public osg::Billboard, public UpdatableNode
{
public:
	HealthBar();
	void onUpdate();
	void setHealth(int health);
	void setMaxHealth(int health, int maxHealth);
private:
	void _setBillBoardStateSet();
	osg::Geometry* _createHealthGemoetry(osg::Texture2D* texture) const;
	osg::Geometry* _createBackgroundGemoetry() const;
	void _updateHealthBar();
	void _updateTextureSize(osg::Geometry* geometry, int health, float textureOffset) const;
	void _updateGeometrySize(osg::Geometry* geometry, float offset, float length, float height) const;
	int _maxHealth;
	int _health;
	int _damage;
	float _elapsedTimeSinceLastDamage;
	int _damageLeft();
	osg::ref_ptr<osg::Geometry> _healthGeometry;
	osg::ref_ptr<osg::Geometry> _damageGeometry;
};
