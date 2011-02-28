#include <menubutton.h>
#include <iostream>

void MenuButton::onClick(osgGA::GUIActionAdapter& aa)
{
  std::cout << "click \n" << std::endl;
  if(this->_onClick != NULL)
      (this->_onClick)(this);
  
  
}
