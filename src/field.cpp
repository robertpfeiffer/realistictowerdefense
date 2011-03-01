// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <field.h>
#include <modeldata.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <osg/PositionAttitudeTransform>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include <osg/Billboard>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osgDB/Registry>
#include <constants.h>
#include <menubutton.h>

Field::Field(FieldType* fieldType) : _isBuildable(fieldType->isBuildable()), _ground(fieldType->getGround()), _fieldType(fieldType)
{
	this->addChild(_ground.get());

	ModelData* modelData = fieldType->getModelData();

	_model = NULL;
	if (modelData != NULL)
	{
		if (modelData->probability >= (float) rand() / RAND_MAX)
		{
			osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform();
			transform->addChild(modelData->model);

			osg::StateSet* state = new osg::StateSet();
			state->setMode( GL_RESCALE_NORMAL, osg::StateAttribute::ON );
			transform->setStateSet(state);

			float scale = _getRandomFloat(modelData->minScale, modelData->maxScale);
			transform->setScale(osg::Vec3d(scale, scale, scale));

			float rotation =  _getRandomFloat(modelData->minRotation, modelData->maxRotation);
			transform->setAttitude(osg::Quat(osg::DegreesToRadians(rotation), osg::Vec3d(0.0, 0.0, 1.0)));

			this->addChild(transform);
		}
	}
}

MenuButton* Field::createMenuItem(osg::StateSet* bbState, int offset)
{
   // Standard size shrub
   float width = 1.0f;
   float height = 1.0f;

   // Declare and initialize geometry
   MenuButton* geometry = new MenuButton;

   // Declare an array of vertices, assign values so we can create a
   // quadrilateral centered relative to the Z axis
   osg::Vec3Array* verts = new osg::Vec3Array(4);

   float x = 1.5 *  sin(offset*PI/4);
   float y = 1.5 * -cos(offset*PI/4);

   if(offset == 0){
     x=0;
     y=0;
   }

   (*verts)[0] = osg::Vec3( x - width/2, 0, y - height/2);
   (*verts)[1] = osg::Vec3( x + width/2, 0, y - height/2);
   (*verts)[2] = osg::Vec3( x + width/2, 0, y + height/2);
   (*verts)[3] = osg::Vec3( x - width/2, 0, y + height/2);
   geometry->setVertexArray(verts);

   // Declare and assign texture coordinates.
   osg::Vec2Array* texCoords = new osg::Vec2Array(4);
   (*texCoords)[0].set(0.0f,0.0f);
   (*texCoords)[1].set(1.0f,0.0f);
   (*texCoords)[2].set(1.0f,1.0f);
   (*texCoords)[3].set(0.0f,1.0f);
   geometry->setTexCoordArray(0,texCoords);

   // Add a primitive set (QUADS) to the geometry
   geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

   // Make sure the geometry has the correct state
   geometry->setStateSet(bbState);

   osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
   geometry->setColorArray(colors);
   geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

   return geometry;
} 

void Field::addMenuEntry(osg::Billboard* billBoard,
			 const std::string texturepath,
                         int offset,
                         bool ignorez,
			 void (* _onClick)(osg::ref_ptr<MenuButton>))
{
	osg::Texture2D *texture = new osg::Texture2D;
	texture->setImage(osgDB::readImageFile(texturepath));
	texture->setMaxAnisotropy(AF_LEVEL);
 
	osg::BlendFunc *blendFunc = new osg::BlendFunc;
	osg::StateSet* billBoardStateSet = new osg::StateSet;

	billBoardStateSet->setTextureAttributeAndModes
		(0, texture, osg::StateAttribute::ON);
	billBoardStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	billBoardStateSet->setAttributeAndModes( blendFunc, osg::StateAttribute::ON );
	billBoardStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	if (ignorez)
	  billBoardStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	MenuButton* drawable = createMenuItem(billBoardStateSet, offset);

	drawable->_onClick=_onClick;

	billBoard->addDrawable(drawable);
}

void build_tower(osg::ref_ptr<MenuButton> button)
{
	Field* f = (dynamic_cast<Field*> (button->getParent(0)->getParent(0)));

	std::cout << "build tower" << std::endl;
	
	if(f != NULL){
		f->setBuilding();
	}
}

void Field::onFocus(osgGA::GUIActionAdapter& aa)
{
    osg::Billboard* billBoard = new osg::Billboard();
	billBoard->setMode(osg::Billboard::POINT_ROT_EYE);
	billBoard->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));

	addMenuEntry(billBoard,
				 "textures/select.png", 
				 0,
				 true,
				 NULL);

	addMenuEntry(billBoard,
				 "textures/tower.png", 
				 3,
				 true,
				 build_tower);

	addMenuEntry(billBoard,
				 "textures/x.png", 
				 4,
				 true,
				 NULL);

	this->addChild(billBoard);
	menu=billBoard;
	std::cout << "open menu" << std::endl;

}



void Field::onClick(osgGA::GUIActionAdapter& aa)
{
  //handled by menubutton
}

void Field::onBlur()
{
	std::cout << "close menu" << std::endl;
	if(menu != NULL)
		this->removeChild(menu);
	menu = NULL;
}

bool Field::setBuilding()
{
	if (_fieldType != NULL && !_fieldType->isBuildable()) //FIXME
	{
	  	return false;
	}
	osg::ref_ptr<osg::Node> n = osgDB::readNodeFile("models/towers/tower5.3ds"); //FIXME
	this->addChild(n);
	//TODO: now add building

	return true;
}

float Field::_getRandomFloat(float min, float max)
{
	return min + ((max - min) * (float) rand() / RAND_MAX);
}
