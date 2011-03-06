// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-
#include <hud.h>

#include <hudbackground.h>

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
	//TODO: clean up that mess

	osg::Geode* endScreen = new osg::Geode();

	osg::BoundingBox bb;
	bb.expandBy(osg::Vec3(0.0, 0.0, 0.0));
	bb.expandBy(osg::Vec3(160.0, 100.0, 0.0));

	endScreen->addDrawable(new HudBackground(bb, 0.0));
	this->addChild(endScreen);

	osgText::Text* text = new osgText::Text();
	text->setCharacterSize(24);
	text->setFont("fonts/orbitron-black.ttf");
	text->setFontResolution(40,40);
	text->setAxisAlignment(osgText::Text::SCREEN);
	text->setAlignment(osgText::Text::CENTER_CENTER);
	text->setPosition(osg::Vec3(80.0, 50.0, 0.0));

	if(won)
	{
		text->setColor(osg::Vec4(1.0, 1.0, 0, 1.0));
		text->setText(osgText::String("EPIC\nWIN"));
	}
	else
	{
		text->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));
		text->setText(osgText::String("EPIC\nFAIL"));
	}

	endScreen->addDrawable(text);
}
