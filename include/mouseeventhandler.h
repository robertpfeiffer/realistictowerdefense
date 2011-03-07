#pragma once

#include <osgGA/GUIActionAdapter>

class MouseEventHandler
{
public:
	//triggered on first click on object
	virtual void onFocus(osgGA::GUIActionAdapter& aa) {};

	//triggered after objec loses focus
	virtual void onBlur() {};

	//triggered on each click on the object
	virtual void onClick(osgGA::GUIActionAdapter& aa) {};

	virtual void onEnter(osgGA::GUIActionAdapter& aa) {};

	virtual void onLeave(osgGA::GUIActionAdapter& aa) {};
};