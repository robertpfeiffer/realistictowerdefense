#pragma once

#include <osgGA/GUIActionAdapter>

/**
 * \class	KeyboardEventHandler
 *
 * \brief	Keyboard event handler. 
 */
class KeyboardEventHandler
{
public:
	virtual void onKeyDown(osgGA::GUIActionAdapter& aa, int eventId) {};
	virtual void onKeyUp(osgGA::GUIActionAdapter& aa, int eventId) {};
};