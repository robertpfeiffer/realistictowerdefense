#pragma once

#include <gametimer.h>
#include <map.h>
#include <keyboardeventhandler.h>
#include <userinteractionhandler.h>
#include <string>

#include <osg/Node>
#include <osgGA/TerrainManipulator>
#include <osgViewer/Viewer>
#include <tdviewer.h>

class Game : KeyboardEventHandler
{
public:
	void run(std::string mapFilename);

	inline UserInteractionHandler* getInteractiontHandler() { return _interactionHandler.get(); };
private:
	void activateAntialiasing(unsigned int samples);
	void setGlobalLight();
	void setCameraManipulator();
	void setWindowTitle(const std::string& title);

	void onKeyDown(osgGA::GUIActionAdapter& aa, int eventId);

	osg::ref_ptr<osgViewer::Viewer> _viewer;
	osg::ref_ptr<UserInteractionHandler> _interactionHandler;
	GameTimer* _gameTimer;
};
