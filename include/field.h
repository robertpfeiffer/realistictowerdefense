#pragma once
#include <fieldtype.h>
#include <mouseeventhandler.h>
#include <string>
#include <menubutton.h>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>

class Field : public osg::PositionAttitudeTransform, public MouseEventHandler
{
public:
	Field(FieldType* fieldType);
	bool setBuilding();
private:
	bool _isBuildable;

	float _getRandomFloat(float min, float max);
	void onFocus(osgGA::GUIActionAdapter& aa);
	void onClick(osgGA::GUIActionAdapter& aa);
	void onBlur();

	MenuButton* createMenuItem(osg::StateSet* state, int offset);
	void addMenuEntry(osg::Billboard* billBoard,
				const std::string texturepath,
				int offset,
				bool ignorez,
				void (*_onClick) (osg::ref_ptr<MenuButton>));

	osg::ref_ptr<osg::Node> menu;

	osg::ref_ptr<osg::Node> _ground;
	osg::ref_ptr<osg::Node> _menu;
	osg::ref_ptr<osg::PositionAttitudeTransform> _model;
	osg::ref_ptr<FieldType> _fieldType;
};
