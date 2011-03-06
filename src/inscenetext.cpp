#include <inscenetext.h>
#include <gametimer.h>
#include <graveyard.h>

float InSceneText::_totalDistance = 10.0f;
float InSceneText::_stepSize = 0.25f;

InSceneText::InSceneText(const std::string text, osg::Vec3 position)
{
	_distanceLeft = _totalDistance;

	_text = new osgText::Text();
    _text->setCharacterSize(0.4);
    _text->setFont("fonts/DejaVuSans.ttf");
    _text->setText(osgText::String(text));
	_text->setAxisAlignment(osgText::Text::SCREEN);
	_text->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(_text);

	this->addChild(geode);
	this->setPosition(position);
}

void InSceneText::setColor(osg::Vec3 color)
{
	_text->setColor(osg::Vec4(color.x(), color.y(), color.z(), _text->getColor().a()));
}

void InSceneText::onUpdate()
{
	float floatDistance = _stepSize * GameTimer::instance()->elapsedTime();
	float opacityStep = floatDistance / _totalDistance;
	this->setPosition(this->getPosition() + osg::Z_AXIS * floatDistance);
	_text->setColor(osg::Vec4(  _text->getColor().r(),
								_text->getColor().g(),
								_text->getColor().b(),
								_text->getColor().a() - opacityStep));

	_distanceLeft -= floatDistance;
	if(_distanceLeft <= 0)
	{
		Graveyard::instance()->killChild(this);
	}
}