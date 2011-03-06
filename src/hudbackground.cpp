#include <hudbackground.h>

HudBackground::HudBackground(osg::BoundingBox dimensions, double padding)
{
	_padding = padding;

	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0,0.0,1.0));
	this->setNormalArray(normals);
	this->setNormalBinding(osg::Geometry::BIND_OVERALL);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0, 0.0, 0.0, 0.5));
	this->setColorArray(colors);
	this->setColorBinding(osg::Geometry::BIND_OVERALL);

	this->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

	osg::StateSet* stateSet = this->getOrCreateStateSet();
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	setDimensions(dimensions);
}

void HudBackground::setDimensions(osg::BoundingBox dimensions)
{
	osg::Vec3Array* vertices = new osg::Vec3Array;
	float depth = dimensions.zMin() - 0.1;
    vertices->push_back(osg::Vec3(dimensions.xMin() - _padding, dimensions.yMax() + _padding, depth));
    vertices->push_back(osg::Vec3(dimensions.xMin() - _padding, dimensions.yMin() - _padding, depth));
    vertices->push_back(osg::Vec3(dimensions.xMax() + _padding, dimensions.yMin() - _padding, depth));
    vertices->push_back(osg::Vec3(dimensions.xMax() + _padding, dimensions.yMax() + _padding, depth));
    this->setVertexArray(vertices);
}