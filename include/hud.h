#pragma once
#include <osg/Camera>
#include <hudelement.h>
#include <infobox.h>
#include <player.h>
#include <stack>
#include <updatablenode.h>

class Hud : public osg::Camera, public UpdatableNode
{
public:
	Hud();
	static Hud* instance();

	void setPlayer(Player* player);
	void pushInfoBox(InfoBox*);
	void popInfoBox();

	void onUpdate();
	void onGameEnd(bool won);

private:
	Player* _player;

	osg::ref_ptr<HudElement> _goldDisplay;
	osg::ref_ptr<HudElement> _lifeDisplay;
	std::stack< osg::ref_ptr<InfoBox> > _infoBoxes;
};