#pragma once
#include <mouseeventhandler.h>
#include <osg/Geometry>

class MenuButton : public osg::Geometry
{
public:
	MenuButton(const std::string texturepath);
	void setPosition(osg::Vec2 pos);

	void onClick(osgGA::GUIActionAdapter& aa);

	void (* _onClick)(osg::ref_ptr<MenuButton> );

private:
	osg::StateSet* createStateSetFromTexturePath(const std::string texturepath);
};

