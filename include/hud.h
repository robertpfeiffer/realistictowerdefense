#pragma once
#include <osg/Camera>
#include <hudelement.h>
#include <player.h>

class Hud : public osg::Camera
{
public:
	Hud();
	static Hud* instance();

	void setPlayer(Player* player);

	void onPlayerUpdate();

private:
	Player* _player;

	osg::ref_ptr<HudElement> _goldDisplay;
};