#pragma once
#include <osg/Projection>
#include <player.h>

class Hud : public osg::Projection
{
public:
	static Hud* instance();

	void setPlayer(Player* player);

	void onPlayerUpdate();

private:
	Player* _player;
};