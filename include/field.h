#pragma once

#include <field_type.h>
#include <keyboard_event_handler.h>

#include <osg/Geode>
#include <osg/PositionAttitudeTransform>

class Field : public osg::Group, KeyboardEventHandler
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