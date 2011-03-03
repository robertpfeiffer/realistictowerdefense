#pragma once
#include <mouseeventhandler.h>
#include <osg/Geometry>

class MenuButton : public osg::Geometry
{
public:
	MenuButton(const std::string texturepath);

	void (* _onClick)(osg::ref_ptr<MenuButton> );
	void onClick(osgGA::GUIActionAdapter& aa);

private:
	osg::StateSet* createStateSetFromTexturePath(const std::string texturepath);
};

