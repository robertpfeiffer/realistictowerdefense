// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-
#include <game.h>

#include <osgGA/TerrainManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Group>
#include <osgParticle/FireEffect>
#include <osgViewer/Viewer>

#include <assetlibrary.h>
#include <constants.h>
#include <gametimer.h>
#include <graveyard.h>
#include <hatchery.h>
#include <hud.h>
#include <map.h>
#include <tdviewer.h>
#include <world.h>

/**
 * \fn	void Game::setCameraManipulator()
 *
 * \brief	Sets the camera manipulator.
 */
void Game::setCameraManipulator()
{
	osgGA::TerrainManipulator* manipulator = new osgGA::TerrainManipulator();
	manipulator->setVerticalAxisFixed(true);
	manipulator->setWheelZoomFactor(WHEEL_ZOOM_FACTOR);
	manipulator->setAllowThrow(false);

	_viewer->setCameraManipulator(manipulator);
}

/**
 * \fn	void Game::setGlobalLight()
 *
 * \brief	Sets the global light.
 */
//FIXME: move into TDviewer
void Game::setGlobalLight()
{
	_viewer->setLightingMode(osg::View::SKY_LIGHT);
	osg::Light* globalLight = new osg::Light;
	globalLight->setLightNum(0);
	globalLight->setAmbient(osg::Vec4(0.5, 0.5, 0.5, 1.0));
	globalLight->setDiffuse(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	globalLight->setSpecular(osg::Vec4(5.0, 5.0, 5.0, 1.0));
	//globalLight->setConstantAttenuation(0.05);
	//light->setLinearAttenuation(0.05);
	//globalLight->setQuadraticAttenuation(0.05);
	_viewer->setLight(globalLight);
}

/**
 * \fn	void Game::setWindowTitle(const std::string& title)
 *
 * \brief	Change the window title.
 *
 * \param	title	The new window title.
 */
void Game::setWindowTitle(const std::string& title)
{
	osgViewer::Viewer::Windows windows;
	_viewer->getWindows(windows);
	for (osgViewer::Viewer::Windows::iterator window = windows.begin(); window != windows.end(); ++window)
	{
		(*window)->setWindowName(title);
	}
}

/**
 * \fn	void Game::onKeyDown(osgGA::GUIActionAdapter& aa, int eventId)
 *
 * \brief	Handle the key down action.
 * \param	aa	The GUIActionAdapter.
 * \param	eventId   	Identifier for the event.
 */
void Game::onKeyDown(osgGA::GUIActionAdapter& aa, int eventId)
{
	switch (eventId)
	{
		case EVENT_PAUSE:
			_gameTimer->togglePause();
			break;
		case EVENT_INCREASE_SPEED:
			_gameTimer->increaseSpeed();
			break;
		case EVENT_NORMALIZE_SPEED:
			_gameTimer->normalizeSpeed();
			break;
		case EVENT_DECREASE_SPEED:
			_gameTimer->decreaseSpeed();
			break;
		default:
			break;
	}
}

/**
 * \fn	void Game::run(osg::ArgumentParser arguments)
 *
 * \brief	Prepares the world and after this start the rendering loop.
 *
 * \param	arguments	The arguments.
 */
void Game::run(osg::ArgumentParser arguments)
{
	AssetLibrary::instance()->unmark();
	
	std::string mapFilename = "";
	while(arguments.read("--map", mapFilename)) {}
	World::instance()->loadMap(mapFilename);

	_viewer = new TDViewer(arguments);
	osg::Group* root = new osg::Group();
	root->addChild(World::instance());
	root->addChild(Hud::instance());
	
	setCameraManipulator();
	setGlobalLight();

	_viewer->getCamera()->setClearColor(osg::Vec4(0, 0, 0, 0));

	_viewer->setSceneData(root);

	//this must be called after realize()
	setWindowTitle("Towerdefense");

	_gameTimer = GameTimer::instance();

	_interactionHandler = new UserInteractionHandler();
	_viewer->addEventHandler(getInteractiontHandler());

	//_eventHandler->registerKeyDownEvent(osgGA::GUIEventAdapter::MODKEY_LEFT_ALT, osgGA::GUIEventAdapter::KEY_Return, this, EVENT_FULLSCREEN);
	_interactionHandler->registerKeyEvent(0, 'p', this, EVENT_PAUSE);
	_interactionHandler->registerKeyEvent(0, '-', this, EVENT_DECREASE_SPEED);
	_interactionHandler->registerKeyEvent(0, 'n', this, EVENT_NORMALIZE_SPEED);
	_interactionHandler->registerKeyEvent(0, '+', this, EVENT_INCREASE_SPEED);

	_viewer->addEventHandler( new osgViewer::StatsHandler());

	_viewer->run();
	AssetLibrary::instance()->sweep();
}
