#pragma once

#include <osgGA/GUIActionAdapter>

/**
 * \class	MouseEventHandler
 *
 * \brief	Mouse event handler. 
 */
class MouseEventHandler
{
public:
	//triggered on first click on object
	virtual void onFocus(osgGA::GUIActionAdapter& aa) {};

	//triggered after objec loses focus
	virtual void onBlur() {};

	//triggered on each click on the object
	virtual void onClick(osgGA::GUIActionAdapter& aa) {};

	//triggered when the mouse hovers for a moment over a location
	virtual void onHover(osgGA::GUIActionAdapter& aa) {};

	//triggered when the mouse moves again after hovering
	virtual void onUnhover() {};
};