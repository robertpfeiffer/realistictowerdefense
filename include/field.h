#pragma once

#include <field_type.h>

#include <osg/Geode>
#include <osg/PositionAttitudeTransform>

class Field : public osg::Group
{
	public:
		Field(FieldType* fieldType);
		bool setBuilding();
	private:
		bool _isBuildable;

		float _getRandomFloat(float min, float max);

		osg::ref_ptr<osg::Node> _ground;
		osg::ref_ptr<osg::PositionAttitudeTransform> _model;
		osg::ref_ptr<FieldType> _fieldType;
};