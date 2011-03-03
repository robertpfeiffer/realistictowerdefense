#pragma once
#include <fieldtype.h>
#include <mouseeventhandler.h>
#include <string>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>

class Tower;

class Field : public osg::PositionAttitudeTransform, public MouseEventHandler
{
public:
	Field(FieldType* fieldType);
	bool setBuilding(Tower* tower);
	bool isBuildable();
	bool hasTower();
	osg::Node* getContent();

private:
	bool _isBuildable;

	float _getRandomFloat(float min, float max);
	void onFocus(osgGA::GUIActionAdapter& aa);
	void onClick(osgGA::GUIActionAdapter& aa);
	void onBlur();

	osg::ref_ptr<osg::Node> _ground;
	osg::ref_ptr<osg::Node> _menu;
	osg::ref_ptr<osg::Node> _content;
	osg::ref_ptr<FieldType> _fieldType;
};
