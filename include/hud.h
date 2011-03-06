#pragma once
#include <osg/Camera>
#include <hudelement.h>
#include <infobox.h>
#include <player.h>
#include <updatablenode.h>

class Hud : public osg::Camera, public UpdatableNode
{
public:
	Hud();
	static Hud* instance();

	void setPlayer(Player* player);
	void setInfoBox(InfoBox*);

	void onUpdate();
	void onGameEnd(bool won);

private:
	Player* _player;

	osg::ref_ptr<HudElement> _goldDisplay;
	osg::ref_ptr<HudElement> _lifeDisplay;
	osg::ref_ptr<InfoBox> _infoBox;
};