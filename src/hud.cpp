#include <hud.h>

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

	_goldDisplay = new HudElement();

	this->addChild(_goldDisplay);
}

void Hud::setPlayer(Player* player)
{
	_player = player;
}

void Hud::onPlayerUpdate()
{

}