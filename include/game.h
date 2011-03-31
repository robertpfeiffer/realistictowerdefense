#pragma once
#include <keyboardeventhandler.h>
#include <userinteractionhandler.h>
#include <string>

#include <osg/Node>
#include <osgGA/TerrainManipulator>
#include <osgViewer/Viewer>

class GameTimer;

/**
 * \class	Game
 *
 * \brief	Contains the game instance. 
 */
class Game : KeyboardEventHandler
{
public:
	void run(osg::ArgumentParser arguments);

	inline UserInteractionHandler* getInteractiontHandler();
private:
	void setGlobalLight();
	void setCameraManipulator();
	void setWindowTitle(const std::string& title);

	void onKeyDown(osgGA::GUIActionAdapter& aa, int eventId);

	osg::ref_ptr<osgViewer::Viewer> _viewer;
	osg::ref_ptr<UserInteractionHandler> _interactionHandler;
	GameTimer* _gameTimer;
};
