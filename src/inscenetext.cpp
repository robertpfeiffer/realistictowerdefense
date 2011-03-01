#include <inscenetext.h>
#include <gametimer.h>
#include <graveyard.h>

float InSceneText::_totalDistance = 10.0f;
float InSceneText::_stepSize = 0.25f;

InSceneText::InSceneText(osgText::String text, osg::Vec3 position)
{
	_distanceLeft = _totalDistance;

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
	float floatDistance = _stepSize * GameTimer::instance()->elapsedTime();
	this->setPosition(this->getPosition() + osg::Z_AXIS * floatDistance);

	_distanceLeft -= floatDistance;
	if(_distanceLeft <= 0)
	{
		Graveyard::instance()->killChild(this);
	}
}