#pragma once

#include <osgGA/GUIActionAdapter>

class KeyboardEventHandler
{
	public:
		virtual void onKeyDown(osgGA::GUIActionAdapter& aa, int eventId) {};
};