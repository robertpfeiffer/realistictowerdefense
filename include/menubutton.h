#pragma once
#include <mouseeventhandler.h>
#include <osg/Geometry>
#include <osg/Texture2D>

/**
 * \class	MenuButton
 *
 * \brief	Menu button. 
 */
class MenuButton : public osg::Geometry, public MouseEventHandler
{
public:
	MenuButton(const std::string texturepath);
	MenuButton(osg::Texture2D* texture);
	void init(osg::Texture2D* texture);

	void setPosition(osg::Vec2 pos);
	void setEnabled(bool enabled);

	virtual void setCallback(void (* callback)(osg::ref_ptr<MenuButton>));

	virtual void onClick(osgGA::GUIActionAdapter& aa);

private:
	void (* _onClick)(osg::ref_ptr<MenuButton> );
	bool _enabled;

	osg::StateSet* createStateSetFromTexture(osg::Texture2D* texture);
};

