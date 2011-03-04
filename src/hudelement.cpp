#include <hudelement.h>

#include <osg/BlendFunc>

double HudElement::_backgroundPadding = 0.5;

HudElement::HudElement(osg::Vec2 position, std::string text)
{
	osg::StateSet* ss = this->getOrCreateStateSet();
	ss->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	this->setupBackground();
	this->setupText(position);

	this->addDrawable(_text);
	this->addDrawable(_background);

	this->setText(text);
}

void HudElement::setupBackground()
{
	_background = new osg::Geometry();
	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0,0.0,1.0));
	_background->setNormalArray(normals);
	_background->setNormalBinding(osg::Geometry::BIND_OVERALL);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0, 0.0, 0.0, 0.3));
	_background->setColorArray(colors);
	_background->setColorBinding(osg::Geometry::BIND_OVERALL);

	_background->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

	osg::StateSet* stateSet = _background->getOrCreateStateSet();
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
}

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

void HudElement::setText(const std::string text)
{
	_text->setText(osgText::String(text));
	updateBackgroundBounds();
}

void HudElement::updateBackgroundBounds()
{
	osg::BoundingBox bb = _text->getBound();

    osg::Vec3Array* vertices = new osg::Vec3Array;
	float depth = bb.zMin() - 0.1;
    vertices->push_back(osg::Vec3(bb.xMin() - _backgroundPadding, bb.yMax() + _backgroundPadding, depth));
    vertices->push_back(osg::Vec3(bb.xMin() - _backgroundPadding, bb.yMin() - _backgroundPadding, depth));
    vertices->push_back(osg::Vec3(bb.xMax() + _backgroundPadding, bb.yMin() - _backgroundPadding, depth));
    vertices->push_back(osg::Vec3(bb.xMax() + _backgroundPadding, bb.yMax() + _backgroundPadding, depth));
    _background->setVertexArray(vertices);
}