#pragma once
#include <mouseeventhandler.h>
#include <osg/Geometry>
#include <osg/Texture2D>

class MenuButton : public osg::Geometry
{
public:
	MenuButton(const std::string texturepath);
	MenuButton(osg::Texture2D* texture);
	void init(osg::Texture2D* texture);
	void setPosition(osg::Vec2 pos);
	virtual void setCallback(void (* callback)(osg::ref_ptr<MenuButton>));

	virtual void onClick(osgGA::GUIActionAdapter& aa);

private:
	void (* _onClick)(osg::ref_ptr<MenuButton> );

	osg::StateSet* createStateSetFromTexture(osg::Texture2D* texture);
};

