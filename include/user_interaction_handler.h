#pragma once

#include "keyboard_event_handler.h"
#include <osgGA\GUIEventHandler>

class UserInteractionHandler : public osgGA::GUIEventHandler
{
	public:
		bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
		void registerKeyEvent(int modKeyMask, int key, KeyboardEventHandler* eventReceiver, int eventId);
		void unregisterKeyEvent(int modKeyMask, int key);
	private:
		struct KeyboardEvent{
			KeyboardEventHandler* eventHandler;
			int eventId;
		};

		typedef std::map<int, KeyboardEvent> keyboardKeyMap;
		typedef std::map<int, keyboardKeyMap> keyboardEventMap;

		keyboardEventMap _keyMapping;
};