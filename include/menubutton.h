#pragma once
#include <mouseeventhandler.h>
#include <osg/Geometry>

class MenuButton : public osg::Geometry
{
public:
	MenuButton(const std::string texturepath);
	void setPosition(osg::Vec2 pos);
	void setCallback(void (* callback)(osg::ref_ptr<MenuButton>));

	void onClick(osgGA::GUIActionAdapter& aa);

private:
	void (* _onClick)(osg::ref_ptr<MenuButton> );

	osg::StateSet* createStateSetFromTexturePath(const std::string texturepath);
};

