#include <menubutton.h>
#include <iostream>

void MenuButton::onClick(osgGA::GUIActionAdapter& aa)
{
  if(this->_onClick != NULL)
      (this->_onClick)(this);
  
  
}
