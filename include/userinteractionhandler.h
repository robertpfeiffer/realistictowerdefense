#pragma once

#include "keyboardeventhandler.h"
#include <osgGA/GUIEventHandler>

class UserInteractionHandler : public osgGA::GUIEventHandler
{
public:
	UserInteractionHandler();
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

	osg::Vec2 _mouseHoverStartPoint;
	double _mouseHoverStartTime;
	bool _hoverTriggered;

	void setActiveMouseHandler(MouseEventHandler* handler, osgGA::GUIActionAdapter& aa);
	void blurActiveMouseHandler();
	KeyboardEvent* getKeyboardHandler(const osgGA::GUIEventAdapter& ea);

	MouseEventHandler* _activeMouseHandler;

	keyboardEventMap _keyMapping;
};
