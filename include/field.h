#pragma once

#include <osg/Geode>
#include <field_type.h>

class Field : public osg::Group
{
	public:
		Field(FieldType* fieldType);
		bool setBuilding();
	private:
		bool _isBuildable;

		osg::ref_ptr<osg::Node> _ground;
		osg::ref_ptr<osg::Node> _model;
		osg::ref_ptr<FieldType> _fieldType;
};