// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <userinteractionhandler.h>

#include <keyboardeventhandler.h>
#include <mouseeventhandler.h>
#include <menubutton.h>
#include <iostream>
#include <osgViewer/Viewer>

double UserInteractionHandler::_hoverDelay = 0.2;

/**
 * \fn	UserInteractionHandler::UserInteractionHandler()
 *
 * \brief	Create a new UserInteractionHandler.
 */
UserInteractionHandler::UserInteractionHandler()
{
	_focusedMouseHandler = NULL;
	_hoveredMouseHandler = NULL;
}

/**
 * \fn	void UserInteractionHandler::setFocusedMouseHandler(MouseEventHandler* handler,
 * 		osgGA::GUIActionAdapter& aa)
 *
 * \brief	called when a Node is focused
 *
 * \param	handler	If non-null, the MouseEventHandler.
 * \param	aa	   	The GUIActionAdapter.
 */
void UserInteractionHandler::setFocusedMouseHandler(MouseEventHandler* handler, osgGA::GUIActionAdapter& aa)
{
	if (_focusedMouseHandler == handler) return;
	blurActiveMouseHandler();

	_focusedMouseHandler = handler;
	handler->onFocus(aa);
}

/**
 * \fn	void UserInteractionHandler::blurActiveMouseHandler()
 *
 * \brief	Blur active mouse handler.
 */
void UserInteractionHandler::blurActiveMouseHandler()
{
	if (_focusedMouseHandler != NULL) 
	{
		_focusedMouseHandler->onBlur();
		_focusedMouseHandler = NULL;
	}
}

/**
 * \fn	UserInteractionHandler::KeyboardEvent* UserInteractionHandler::getKeyboardHandler(const osgGA::GUIEventAdapter& ea)
 *
 * \brief	Gets a keyboard handler.
 *
 * \param	ea	The GUIEventAdapter.
 *
 * \return	null if it fails, else the keyboard event handler.
 */
UserInteractionHandler::KeyboardEvent* UserInteractionHandler::getKeyboardHandler(const osgGA::GUIEventAdapter& ea)
{
	keyboardEventMap::iterator keyMask = _keyMapping.find(ea.getModKeyMask());
	if (keyMask == _keyMapping.end()) return NULL;
		
	keyboardKeyMap::iterator keyMapping = keyMask->second.find(ea.getKey());
	if (keyMapping == keyMask->second.end()) return NULL;
	
	return &keyMapping->second;
}

/**
 * \fn	MenuButton* findMenuButton(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
 *
 * \brief	Cast a ray to searches for the first menu button.
 *
 * \param	ea		  	The GUIEventAdapter.
 * \param [in,out]	aa	The GUIActionAdapter.
 *
 * \return	null if it fails, else the found menu button.
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

/**
 * \fn	MouseEventHandler* findEventHandler(const osgGA::GUIEventAdapter& ea,
 * 		osgGA::GUIActionAdapter& aa)
 *
 * \brief	Cast a ray to find a clickable node or a node with clickable Parents.
 *
 * \param	ea		  	The GUIEventAdapter.
 * \param [in,out]	aa	The GUIActionAdapter.
 *
 * \return	null if it fails, else the found event handler.
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

/**
 * \fn	bool UserInteractionHandler::handle(const osgGA::GUIEventAdapter& ea,
 * 		osgGA::GUIActionAdapter& aa)
 *
 * \brief	Handle key presses and mouse clicks.
 *
 * \param	ea		  	The GUIEventAdapter.
 * \param [in,out]	aa	The GUIActionAdapter.
 *
 * \return	return false.
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

/**
 * \fn	void UserInteractionHandler::registerKeyEvent(int modKeyMask, int key,
 * 		KeyboardEventHandler* eventReceiver, int eventId)
 *
 * \brief	Registers the key event for speciefied key combination.
 * 			If this key combination is already used, it overrides existing.
 *
 * \param	modKeyMask			 	The modifier key mask.
 * \param	key					 	The key.
 * \param	eventReceiver	If non-null, the event receiver.
 * \param	eventId				 	Identifier for the event.
 */
void UserInteractionHandler::registerKeyEvent(int modKeyMask, int key, KeyboardEventHandler* eventReceiver, int eventId)
{
	KeyboardEvent* keyboardEvent = &_keyMapping[modKeyMask][key];
	keyboardEvent->eventHandler = eventReceiver;
	keyboardEvent->eventId = eventId;
}

/**
 * \fn	void UserInteractionHandler::unregisterKeyEvent(int modKeyMask, int key)
 *
 * \brief	Unregisters the key event.
 *
 * \param	modKeyMask	The modifier key mask.
 * \param	key		  	The key.
 */
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
