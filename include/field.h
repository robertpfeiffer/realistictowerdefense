#pragma once

#include <osg/Geode>
#include <field_type.h>

class Field : public osg::Group
{
	public:
		Field(FieldType* fieldType);
		bool setBuilding();
	private:
		osg::ref_ptr<FieldType> _fieldType;
};