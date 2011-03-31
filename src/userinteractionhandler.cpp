// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <userinteractionhandler.h>

#include <keyboardeventhandler.h>
#include <mouseeventhandler.h>
#include <menubutton.h>
#include <iostream>
#include <osgViewer/Viewer>

double UserInteractionHandler::_hoverDelay = 0.2;

UserInteractionHandler::UserInteractionHandler()
{
	_focusedMouseHandler = NULL;
	_hoveredMouseHandler = NULL;
}

/** called when a Node is focused
 */
void UserInteractionHandler::setFocusedMouseHandler(MouseEventHandler* handler, osgGA::GUIActionAdapter& aa)
{
	if (_focusedMouseHandler == handler) return;
	blurActiveMouseHandler();

	_focusedMouseHandler = handler;
	handler->onFocus(aa);
}

/** called when the focused Node has lost the focus
 */
void UserInteractionHandler::blurActiveMouseHandler()
{
	if (_focusedMouseHandler != NULL) 
	{
		_focusedMouseHandler->onBlur();
		_focusedMouseHandler = NULL;
	}
}

UserInteractionHandler::KeyboardEvent* UserInteractionHandler::getKeyboardHandler(const osgGA::GUIEventAdapter& ea)
{
	keyboardEventMap::iterator keyMask = _keyMapping.find(ea.getModKeyMask());
	if (keyMask == _keyMapping.end()) return NULL;
		
	keyboardKeyMap::iterator keyMapping = keyMask->second.find(ea.getKey());
	if (keyMapping == keyMask->second.end()) return NULL;
	
	return &keyMapping->second;
}

/** Cast a ray to find a menu button.
 */
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

/** Cast a ray to find a clickable Node or a Node with clickable Parents.
 */
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

/** Handle key presses and mouse clicks
 */
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
				setFocusedMouseHandler(handler, aa);
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
		KeyboardEvent* keyboardEvent = getKeyboardHandler(ea);
		if (keyboardEvent != NULL)
		{
			keyboardEvent->eventHandler->onKeyDown(aa, keyboardEvent->eventId);
		}
		return false;
	}
	case(osgGA::GUIEventAdapter::KEYUP):
	{
		KeyboardEvent* keyboardEvent = getKeyboardHandler(ea);
		if (keyboardEvent != NULL)
		{
			keyboardEvent->eventHandler->onKeyUp(aa, keyboardEvent->eventId);
		}
		return false;
	}
	case(osgGA::GUIEventAdapter::FRAME):
	{
		osg::Vec2 currentPoint = osg::Vec2(ea.getX(), ea.getY());
		if(currentPoint != _mouseHoverStartPoint)
		{
			if(_hoverTriggered)
			{
				if(_hoveredMouseHandler.get() != NULL)
				{
					//dynamic cast will work, because we casted from MouseEventHandler to Referenced before
					dynamic_cast<MouseEventHandler*>(_hoveredMouseHandler.get())->onUnhover();
					_hoveredMouseHandler = NULL;
				}
				_hoverTriggered = false;
			}

			_mouseHoverStartPoint = currentPoint;
			_mouseHoverStartTime = ea.getTime();
			return false;
		}

		if(!_hoverTriggered && (ea.getTime() - _mouseHoverStartTime) > _hoverDelay)
		{
			_hoverTriggered = true;
			MouseEventHandler* handler = findMenuButton(ea, aa); //buttons are prioritized
			if(handler == NULL)
				handler = findEventHandler(ea, aa);

			if(handler != NULL)
			{
				_hoveredMouseHandler = dynamic_cast<osg::Referenced*>(handler);
				if(_hoveredMouseHandler != NULL) //only hover if we can unhover later
				{
					handler->onHover(aa);
				}
			}
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
