#include <hud.h>

Hud* Hud::instance()
{
	static osg::ref_ptr<Hud> hud_ptr;
	if(hud_ptr.get() == NULL)
	{
		hud_ptr = new Hud();
	}
	return hud_ptr.get();
}

void Hud::setPlayer(Player* player)
{
	_player = player;
}

void Hud::onPlayerUpdate()
{

}