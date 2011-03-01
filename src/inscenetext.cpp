#include <inscenetext.h>
#include <game_timer.h>
#include <graveyard.h>

InSceneText::InSceneText(osgText::String text, osg::Vec3 position)
{
	_distanceLeft = 1.2f;

	osgText::Text* myText = new osgText::Text();
    myText->setCharacterSize(0.4);
    myText->setFont("fonts/DejaVuSans.ttf");
    myText->setText(text);
	myText->setAxisAlignment(osgText::Text::SCREEN);
	myText->setColor(osg::Vec4(255, 255, 255, 1));

	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(myText);

	this->addChild(geode);
	this->setPosition(position);
}

void InSceneText::onUpdate()
{
	float floatDistance = 0.25f * GameTimer::instance()->elapsedTime();
	this->setPosition(this->getPosition() + osg::Z_AXIS * floatDistance);

	_distanceLeft -= floatDistance;
	if(_distanceLeft <= 0)
	{
		Graveyard::instance()->killChild(this);
	}
}