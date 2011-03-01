// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <osg/CullFace>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/PositionAttitudeTransform>

#include <terrain.h>

Terrain::Terrain(Map* map) : osg::Group()
{
	_map = map;

	for(long x = 0; x < _map->getWidth(); x++)
	{
		for(long y = 0; y < _map->getHeight(); y++) {
			this->addChild(createTerrainBlock(x, y));
		}
	}

	this->addChild(createStrata());
}

osg::Node* Terrain::createTerrainBlock(int x, int y)
{
	osg::PositionAttitudeTransform* terrainBlockTransform = new osg::PositionAttitudeTransform();
	terrainBlockTransform->addChild(_map->getField(x, y));

	osg::Vec3 terrainBlockTranslation(x, -y, 0);
	terrainBlockTransform->setPosition(terrainBlockTranslation);

	return terrainBlockTransform;
}

osg::Node* Terrain::createStrata()
{
	osg::Geode* strata = new osg::Geode();

	osg::Geometry* geometry = new osg::Geometry();

	long w = _map->getWidth();
	long h = _map->getHeight();
	osg::Vec3Array* vertices = new osg::Vec3Array(8);
	(*vertices)[0].set(osg::Vec3(-0.5    , 0.5    ,  0.0));
	(*vertices)[1].set(osg::Vec3(-0.5    , 0.5    , -1.0));
	(*vertices)[2].set(osg::Vec3(-0.5 + w, 0.5    ,  0.0));
	(*vertices)[3].set(osg::Vec3(-0.5 + w, 0.5    , -1.0));	
	(*vertices)[4].set(osg::Vec3(-0.5 + w, 0.5 - h,  0.0));
	(*vertices)[5].set(osg::Vec3(-0.5 + w, 0.5 - h, -1.0));
	(*vertices)[6].set(osg::Vec3(-0.5    , 0.5 - h,  0.0));
	(*vertices)[7].set(osg::Vec3(-0.5    , 0.5 - h, -1.0));

	geometry->setVertexArray(vertices);

	osg::DrawElementsUInt* primitive = new osg::DrawElementsUInt(osg::PrimitiveSet::QUAD_STRIP, 10);
	for (int i = 0; i < 10; i++)
	{
		(*primitive)[i] = i % 8;
	}
	geometry->addPrimitiveSet(primitive);

	osg::Vec2Array* texcoords = new osg::Vec2Array(8);
	(*texcoords)[ 0].set(0.0, 0.0);
	(*texcoords)[ 1].set(0.0, 1.0);
	(*texcoords)[ 2].set(  w, 0.0);
	(*texcoords)[ 3].set(  w, 1.0);

	(*texcoords)[ 4].set(w + h, 0.0);
	(*texcoords)[ 5].set(w + h, 1.0);

	(*texcoords)[ 6].set(w, 0.0);
	(*texcoords)[ 7].set(w, 1.0);

	geometry->setTexCoordArray(0,texcoords);

	osg::Vec4Array* colors = new osg::Vec4Array(8);
	for (int i = 0; i < 4; i++)
	{
		(*colors)[i*2    ].set(1.0f, 1.0f, 1.0f, 1.0f);
		(*colors)[1*2 + 1].set(0.0f, 0.0f, 0.0f, 0.0f);
	}

	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);


	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0, 0.0, 1.0));
	geometry->setNormalArray(normals);
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	osg::StateSet* state = new osg::StateSet();
	state->setTextureAttributeAndModes(0, _map->getStrataTexture(), osg::StateAttribute::ON);
	state->setAttributeAndModes(new osg::CullFace(osg::CullFace::FRONT), osg::StateAttribute::ON);
	strata->setStateSet(state);

	strata->addDrawable(geometry);

	return strata;
}
