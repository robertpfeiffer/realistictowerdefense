// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-
#include <hud.h>

#include <sstream>

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osgText/Text>

Hud* Hud::instance()
{
	static osg::ref_ptr<Hud> hud_ptr;
	if(hud_ptr.get() == NULL)
	{
		hud_ptr = new Hud();
	}
	return hud_ptr.get();
}

Hud::Hud()
{
	this->setProjectionMatrix(osg::Matrix::ortho2D(0,160,0,100));
	this->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	this->setViewMatrix(osg::Matrix::identity());
	this->setClearMask(GL_DEPTH_BUFFER_BIT);
	this->setRenderOrder(osg::Camera::POST_RENDER);
	this->setAllowEventFocus(false);

	_goldDisplay = new HudElement(osg::Vec2(155.0, 95.0), "---");
	_lifeDisplay = new HudElement(osg::Vec2(155.0, 90.0), "---");

	this->addChild(_goldDisplay);
	this->addChild(_lifeDisplay);
}

void Hud::setPlayer(Player* player)
{
	_player = player;
	onPlayerUpdate();
}

void Hud::onPlayerUpdate() 
{
	std::stringstream moneytext; 
	moneytext << _player->getMoney() << " Gold"; 
	_goldDisplay->setText(moneytext.str()); 

	std::stringstream livestext; 
	livestext << _player->getLives() << " Lives"; 
	_lifeDisplay->setText(livestext.str()); 
}

void Hud::onGameEnd(bool won)
{

}