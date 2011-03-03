#pragma once
#include <mouseeventhandler.h>
#include <osg/Geometry>

class MenuButton : public osg::Geometry
{
public:
	MenuButton(osg::StateSet* ss);

	void (* _onClick)(osg::ref_ptr<MenuButton> );
	void onClick(osgGA::GUIActionAdapter& aa);
};

