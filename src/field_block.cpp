#include <field_block.h>
#include <osg/Geometry>
#include <osg/Texture2D>


FieldBlock::FieldBlock(bool isBuildable, bool isAccessible, osg::Texture2D* texture) : _isBuildable(isBuildable), _isAccessible(isAccessible)
{
	osg::Geometry* geometry = new osg::Geometry();
	   

	osg::Vec3Array* vertices = new osg::Vec3Array(4);
	(*vertices)[0].set(osg::Vec3(0.0, 0.0, 0.0));
	(*vertices)[1].set(osg::Vec3(1.0, 0.0, 0.0));
	(*vertices)[2].set(osg::Vec3(1.0, 1.0, 0.0));
	(*vertices)[3].set(osg::Vec3(0.0, 1.0, 0.0));

	geometry->setVertexArray(vertices);

	osg::DrawElementsUInt* primitive = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	primitive->push_back(3);
	primitive->push_back(2);
	primitive->push_back(1);
	primitive->push_back(0);
	geometry->addPrimitiveSet(primitive);

	osg::Vec2Array* texcoords = new osg::Vec2Array(4);
	(*texcoords)[0].set(0.0, 0.0);
	(*texcoords)[1].set(1.0, 0.0);
	(*texcoords)[2].set(1.0, 1.0);
	(*texcoords)[3].set(0.0, 1.0);
	geometry->setTexCoordArray(0,texcoords);

	osg::StateSet* state = new osg::StateSet();
	state->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	this->setStateSet(state);

	this->addDrawable(geometry);
}

bool FieldBlock::isBuildable()
{
	return _isBuildable;
}

bool FieldBlock::isAccessible()
{
	return _isAccessible;
}