#pragma once

#include <gametimer.h>
#include <map.h>
#include <keyboardeventhandler.h>
#include <userinteractionhandler.h>

#include <osg/Node>
#include <osgGA/TerrainManipulator>
#include <osgViewer/Viewer>

class Game : KeyboardEventHandler
{
	public:
		void run();

		inline UserInteractionHandler* getInteractiontHandler() { return _interactionHandler.get(); };
	private:
		void activateAntialiasing(unsigned int samples);
		void setGlobalLight();
		void setCameraManipulator();
		void limitCamera(osgGA::TerrainManipulator* manipulator);
		void setWindowTitle(const std::string& title);

		void onKeyDown(osgGA::GUIActionAdapter& aa, int eventId);

		osgViewer::Viewer _viewer;
		osg::ref_ptr<UserInteractionHandler> _interactionHandler;
		GameTimer* _gameTimer;
};