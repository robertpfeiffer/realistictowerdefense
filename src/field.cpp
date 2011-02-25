#include <field.h>
#include <model_data.h>
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
#include <osgGA/TerrainManipulator>

#include <osg/Billboard>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osgDB/Registry>
#include <constants.h>
#define PI 3.14159265

Field::Field(FieldType* fieldType) : _isBuildable(fieldType->isBuildable()), _ground(fieldType->getGround())
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

osg::Drawable* Field::createMenuItem(osg::StateSet* bbState)
{
   // Standard size shrub
   float width = 1.0f;
   float height = 1.0f;

   // Declare and initialize geometry
   osg::Geometry* geometry = new osg::Geometry;

   // Declare an array of vertices, assign values so we can create a
   // quadrilateral centered relative to the Z axis
   osg::Vec3Array* verts = new osg::Vec3Array(4);
   (*verts)[0] = osg::Vec3(-width/2.0f, 0, 0);
   (*verts)[1] = osg::Vec3( width/2.0f, 0, 0);
   (*verts)[2] = osg::Vec3( width/2.0f, 0, height);
   (*verts)[3] = osg::Vec3(-width/2.0f, 0, height);
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
			 osg::Vec3 pos)
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

	osg::Drawable* drawable = createMenuItem(billBoardStateSet);

	billBoard->addDrawable(drawable , pos);

}

void Field::onFocus(osgGA::GUIActionAdapter& aa)
{
        osg::Billboard* billBoard = new osg::Billboard();
	billBoard->setMode(osg::Billboard::POINT_ROT_EYE);
	billBoard->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));


	addMenuEntry(billBoard,
		     "textures/arrow.png", 
		     osg::Vec3( 0,
				0, 
				0.1));


	addMenuEntry(billBoard,
		     "textures/tower.png", 
		     osg::Vec3( 1,
				0, 
				0.1));

	addMenuEntry(billBoard,
		     "textures/select.png", 
		     osg::Vec3( 0,
				1, 
				0.1));


	addMenuEntry(billBoard,
		     "textures/x.png", 
		     osg::Vec3( 1,
				1, 
				0.1));
        this->addChild(billBoard);
        menu=billBoard;
}

void Field::onClick(osgGA::GUIActionAdapter& aa)
{

}

void Field::onBlur()
{
	std::cout << "Blur field\r\n";
        if(menu != NULL)
	  this->removeChild(menu);
        menu = NULL;
}

bool Field::setBuilding()
{
	FieldType* fieldType = _fieldType.get();
	if (!fieldType->isBuildable())
	{
		return false;
	}

	//TODO: now add building

	return true;
}

float Field::_getRandomFloat(float min, float max)
{
	return min + ((max - min) * (float) rand() / RAND_MAX);
}
