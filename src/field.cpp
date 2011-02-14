#include <field.h>
#include <model_data.h>
#include <iostream>

#include <osg/PositionAttitudeTransform>

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

void Field::onClick(osgGA::GUIActionAdapter& aa)
{
	std::cout << "Click";
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