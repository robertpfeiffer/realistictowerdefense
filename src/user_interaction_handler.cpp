#include <keyboard_event_handler.h>
#include <user_interaction_handler.h>

#include <osgViewer/Viewer>
#include <iostream>

bool UserInteractionHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
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
							if (!(*hitNodeIt)->getName().empty())
							{
								std::cout << (*hitNodeIt)->getName();
								return false;
							}
						}
					}
				}
			}
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
					KeyboardEventHandler* handler = keyMapping->second;
					handler->onKeyDown(aa);
					return false;
				}
			}
			return false;
		}
		default:
			return false;
	}
}

void UserInteractionHandler::registerKeyEvent(int modKeyMask, int key, KeyboardEventHandler* eventReceiver)
{
	_keyMapping[modKeyMask][key] = eventReceiver;
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