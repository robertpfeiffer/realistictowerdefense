#include <mouse_event_handler.h>
#include <osg/Geometry>
#include <field.h>

class MenuButton : public osg::Geometry
{
 public: 
  void *(* _onClick)(void);
  void onClick(osgGA::GUIActionAdapter& aa);
};

