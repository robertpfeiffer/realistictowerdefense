#pragma once

#include "keyboard_event_handler.h"
#include <osgGA\GUIEventHandler>

class UserInteractionHandler : public osgGA::GUIEventHandler
{
	public:
		bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
		void registerKeyEvent(int modKeyMask, int key, KeyboardEventHandler* eventReceiver);
		void unregisterKeyEvent(int modKeyMask, int key);
	private:
		typedef std::map<int, KeyboardEventHandler*> keyboardKeyMap;
		typedef std::map<int, keyboardKeyMap> keyboardEventMap;

		keyboardEventMap _keyMapping;
};