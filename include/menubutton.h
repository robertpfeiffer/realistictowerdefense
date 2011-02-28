#pragma once
#include <mouse_event_handler.h>
#include <osg/Geometry>

class MenuButton : public osg::Geometry
{
 public: 
  void (* _onClick)(osg::ref_ptr<MenuButton> );
  void onClick(osgGA::GUIActionAdapter& aa);
};

