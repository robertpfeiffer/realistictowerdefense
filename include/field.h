#pragma once
#include <mouseeventhandler.h>
#include <string>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>

class FieldType;
class Tower;

/**
 * \class	Field
 *
 * \brief	A single field of the terrain. 
 */
class Field : public osg::PositionAttitudeTransform, public MouseEventHandler
{
public:
	Field(FieldType* fieldType);
	bool setBuilding(Tower* tower);
	bool destroyBuilding();

	void reset();

	bool isBuildable();
	bool hasTower();
private:
	bool _isBuildable;

	float _getRandomFloat(float min, float max);
	void onFocus(osgGA::GUIActionAdapter& aa);
	void onBlur();

	osg::ref_ptr<osg::Node> _ground;
	osg::ref_ptr<osg::Node> _menu;
	osg::ref_ptr<osg::Node> _content;
	osg::ref_ptr<FieldType> _fieldType;
};
