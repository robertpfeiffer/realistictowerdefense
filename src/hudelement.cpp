#include <hudelement.h>

HudElement::HudElement(osg::Vec2 position, std::string text)
{
	_text = new osgText::Text();
	osg::StateSet* ss = this->getOrCreateStateSet();
	ss->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	this->addDrawable(_text);

	_text->setCharacterSize(4);
	_text->setFont("fonts/DejaVuSans.ttf");
	_text->setAxisAlignment(osgText::Text::SCREEN);
	_text->setAlignment(osgText::Text::RIGHT_TOP);
	_text->setPosition(osg::Vec3(position.x(), position.y(), 0.0));
	_text->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	this->setText(text);
}

void HudElement::setText(const std::string text)
{
	_text->setText(osgText::String(text));
}