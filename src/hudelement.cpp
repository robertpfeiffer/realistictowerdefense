#include <hudelement.h>

#include <osg/BlendFunc>

#include <hudbackground.h>

double HudElement::_backgroundPadding = 0.5;

/**
 * \fn	HudElement::HudElement(osg::Vec2 position, std::string text)
 *
 * \brief	Create an hud element with speciefied text on a background at speciefied position.
 *
 * \param	position	The position of the text.
 * \param	text		The text.
 */
HudElement::HudElement(osg::Vec2 position, std::string text)
{
	osg::StateSet* ss = this->getOrCreateStateSet();
	ss->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	this->setupText(position);
	_background = new HudBackground(_text->getBound(), _backgroundPadding);

	this->addDrawable(_text);
	this->addDrawable(_background);

	this->setText(text);
}

/**
 * \fn	void HudElement::setupText(osg::Vec2 position)
 *
 * \brief	Sets up the text.
 *
 * \param	position	The position of the text.
 */
void HudElement::setupText(osg::Vec2 position)
{
	_text = new osgText::Text();
	_text->setCharacterSize(4);
	_text->setFont("fonts/DejaVuSans.ttf");
	_text->setAxisAlignment(osgText::Text::SCREEN);
	_text->setAlignment(osgText::Text::RIGHT_TOP);
	_text->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	_text->setPosition(osg::Vec3(position.x(), position.y(), 0.0));
}

/**
 * \fn	void HudElement::setText(const std::string text)
 *
 * \brief	Set the text.
 *
 * \param	text	The text.
 */
void HudElement::setText(const std::string text)
{
	_text->setText(osgText::String(text));
	_background->setDimensions(_text->computeBound());
}