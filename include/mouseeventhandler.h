#pragma once

#include <osgGA/GUIActionAdapter>

class MouseEventHandler
{
public:
	virtual void onFocus(osgGA::GUIActionAdapter& aa) {};
	virtual void onClick(osgGA::GUIActionAdapter& aa) {};
	virtual void onBlur() {};
};