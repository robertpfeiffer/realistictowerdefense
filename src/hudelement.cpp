#include <hudelement.h>

HudElement::HudElement()
{
	osg::StateSet* ss = this->getOrCreateStateSet();
	ss->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	osgText::Text* textOne = new osgText::Text();
	this->addDrawable( textOne );
	textOne->setCharacterSize(4);
	textOne->setFont("fonts/DejaVuSans.ttf");
	textOne->setText("Gold: 500");
	textOne->setAxisAlignment(osgText::Text::SCREEN);
	textOne->setAlignment(osgText::Text::RIGHT_TOP);
	textOne->setPosition( osg::Vec3(155.0, 98.0, 0.0) );
	textOne->setColor( osg::Vec4(1.0, 1.0, 1.0, 1.0) );
}