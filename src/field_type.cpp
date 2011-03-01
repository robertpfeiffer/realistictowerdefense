// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <field_type.h>
#include <osg/Geode>
#include <osg/CullFace>
#include <osg/Geometry>
#include <osg/Texture2D>


FieldType::FieldType(osg::Texture2D* texture, ModelData* modelData, bool isBuildable)
	: osg::Referenced(), _modelData(modelData), _isBuildable(isBuildable)
{
	//Create ground
	_ground = new osg::Geode();

	osg::Geometry* geometry = new osg::Geometry();
	   

	osg::Vec3Array* vertices = new osg::Vec3Array(4);
	(*vertices)[0].set(osg::Vec3(-0.5, -0.5, 0.0));
	(*vertices)[1].set(osg::Vec3( 0.5, -0.5, 0.0));
	(*vertices)[2].set(osg::Vec3( 0.5,  0.5, 0.0));
	(*vertices)[3].set(osg::Vec3(-0.5,  0.5, 0.0));

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

	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0, 0.0, 1.0));
	geometry->setNormalArray(normals);
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	osg::StateSet* state = new osg::StateSet();
	state->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	state->setAttributeAndModes(new osg::CullFace(osg::CullFace::FRONT), osg::StateAttribute::ON);
	_ground->setStateSet(state);

	_ground->addDrawable(geometry);
}
