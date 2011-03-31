// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <fieldtype.h>

/**
 * @class FieldType
 * A kind of map tile, defined in the xml config.
 * The letters in the 2d map define which tile has which kind.
 */

#include <osg/Geode>
#include <osg/CullFace>
#include <osg/Geometry>
#include <osg/Texture2D>

#include <modeldata.h>

/**
 * \fn	FieldType::FieldType(osg::Texture2D* texture, ModelData* modelData,
 * 		bool isBuildable) : osg::Referenced(), _modelData(modelData), _isBuildable(isBuildable)
 *
 * \brief	Creates a new template of a field.
 *
 * \param	texture  	If non-null, the texture.
 * \param	modelData	Information describing the model.
 * \param	isBuildable 	true if field is buildable.
 */
FieldType::FieldType(osg::Texture2D* texture, ModelData* modelData, bool isBuildable)
	: osg::Referenced(), _modelData(modelData), _isBuildable(isBuildable)
{
	/*
	  Create ground Geode 
	  Each kind of field shares a ground Geode.
	  The texture and the other xml config attributes are passed in from the map builder.
	*/
	_ground = new osg::Geode();

	//geometry is a square with a square texture thats fits it
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
	
	//the normal of the ground points up
	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0, 0.0, 1.0));
	geometry->setNormalArray(normals);
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//put the texture on the geometry
	osg::StateSet* state = new osg::StateSet();
	state->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

	//when lookung from below, the ground is transparent
	state->setAttributeAndModes(new osg::CullFace(osg::CullFace::FRONT), osg::StateAttribute::ON);
	_ground->setStateSet(state);
	
	_ground->addDrawable(geometry);
}
