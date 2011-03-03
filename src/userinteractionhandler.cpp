// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <keyboardeventhandler.h>
#include <mouseeventhandler.h>
#include <userinteractionhandler.h>
#include <menubutton.h>
#include <iostream>
#include <osgViewer/Viewer>

UserInteractionHandler::UserInteractionHandler()
{
	_activeMouseHandler = NULL;
}

void UserInteractionHandler::setActiveMouseHandler(MouseEventHandler* handler, osgGA::GUIActionAdapter& aa)
{
	if (_activeMouseHandler == handler) return;
	blurActiveMouseHandler();

	_activeMouseHandler = handler;
	handler->onFocus(aa);
}

void UserInteractionHandler::blurActiveMouseHandler()
{
	if (_activeMouseHandler != NULL) 
	{
		_activeMouseHandler->onBlur();
		_activeMouseHandler = NULL;
	}
}

UserInteractionHandler::KeyboardEvent* UserInteractionHandler::getKeyBoardHandler(const osgGA::GUIEventAdapter& ea)
{
	keyboardEventMap::iterator keyMask = _keyMapping.find(ea.getModKeyMask());
	if (keyMask == _keyMapping.end()) return NULL;
		
	keyboardKeyMap::iterator keyMapping = keyMask->second.find(ea.getKey());
	if (keyMapping == keyMask->second.end()) return NULL;
	
	return &keyMapping->second;
}

MenuButton* findMenuButton(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if (view)
	{
		osgUtil::LineSegmentIntersector::Intersections intersections;
		if (view->computeIntersections(ea.getX(), ea.getY(), intersections))
		{
			for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end();	++hitr)
			{
				MenuButton* button = dynamic_cast<MenuButton*>(hitr->drawable.get());			
				if(button != NULL)
					return button;
			}
		}
	}
	return NULL;
}

MouseEventHandler* findEventHandler(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if (view)
	{
		osgUtil::LineSegmentIntersector::Intersections intersections;
		if (view->computeIntersections(ea.getX(), ea.getY(), intersections))
		{
			for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end();	++hitr)
			{

				for(osg::NodePath::const_reverse_iterator hitNodeIt = hitr->nodePath.rbegin(); hitNodeIt != hitr->nodePath.rend(); ++hitNodeIt)
				{
					MouseEventHandler* handler = dynamic_cast<MouseEventHandler*>(*hitNodeIt);
					if (handler != NULL)
					{
						return handler;
					}
				}
			}
		}
	}
	return NULL;
}

bool UserInteractionHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{

	osgGA::GUIEventAdapter::EventType eType = ea.getEventType();

	switch(eType)
	{
	case( osgGA::GUIEventAdapter::PUSH):
	{
		if(ea.getButtonMask()==osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
		{
			blurActiveMouseHandler();
			MouseEventHandler* handler = findEventHandler(ea, aa);
			if (handler != NULL)
				setActiveMouseHandler(handler, aa);
			return false;
		}
		
		MenuButton* button = findMenuButton(ea, aa);		
		if(button != NULL) {
			button->onClick(aa);
			blurActiveMouseHandler();
			return false;
		} 

		MouseEventHandler* handler = findEventHandler(ea, aa);
		if (handler != NULL) {
			handler->onClick(aa);
			return false;
		}

		return false;
	}
	case(osgGA::GUIEventAdapter::KEYDOWN):
	{
		KeyboardEvent* keyboardEvent = getKeyBoardHandler(ea);
		if (keyboardEvent != NULL)
		{
			keyboardEvent->eventHandler->onKeyDown(aa, keyboardEvent->eventId);
		}
		return false;
	}
	case(osgGA::GUIEventAdapter::KEYUP):
	{
		KeyboardEvent* keyboardEvent = getKeyBoardHandler(ea);
		if (keyboardEvent != NULL)
		{
			keyboardEvent->eventHandler->onKeyUp(aa, keyboardEvent->eventId);
		}
		return false;
	}
	default:
		return false;
	}
}

void UserInteractionHandler::registerKeyEvent(int modKeyMask, int key, KeyboardEventHandler* eventReceiver, int eventId)
{
	KeyboardEvent* keyboardEvent = &_keyMapping[modKeyMask][key];
	keyboardEvent->eventHandler = eventReceiver;
	keyboardEvent->eventId = eventId;
}

void UserInteractionHandler::unregisterKeyEvent(int modKeyMask, int key)
{
	keyboardEventMap::iterator keyMask = _keyMapping.find(modKeyMask);
	if (keyMask != _keyMapping.end())
	{
		keyMask->second.erase(key);

		if (keyMask->second.size() == 0)
		{
			_keyMapping.erase(keyMask);
		}
	}
}
