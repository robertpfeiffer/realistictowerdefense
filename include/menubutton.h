#pragma once
#include <mouseeventhandler.h>
#include <osg/Geometry>

class MenuButton : public osg::Geometry
{
public: 
	void (* _onClick)(osg::ref_ptr<MenuButton> );
	void onClick(osgGA::GUIActionAdapter& aa);
};

