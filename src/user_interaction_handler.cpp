#include <keyboard_event_handler.h>
#include <mouse_event_handler.h>
#include <user_interaction_handler.h>

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

bool UserInteractionHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	switch(ea.getEventType())
    {
        case(osgGA::GUIEventAdapter::PUSH):
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
								setActiveMouseHandler(handler, aa);								
								handler->onClick(aa);
								return false;
							}
						}
					}
				}
			}
			blurActiveMouseHandler();
            return false;
        }    
        case(osgGA::GUIEventAdapter::KEYDOWN):
        {
			keyboardEventMap::iterator keyMask = _keyMapping.find(ea.getModKeyMask());
			if (keyMask != _keyMapping.end())
			{
				keyboardKeyMap::iterator keyMapping = keyMask->second.find(ea.getKey());
				if (keyMapping != keyMask->second.end())
				{
					KeyboardEvent* keyboardEvent = &keyMapping->second;
					keyboardEvent->eventHandler->onKeyDown(aa, keyboardEvent->eventId);
					return false;
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