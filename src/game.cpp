// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-
#include <assetlibrary.h>
#include <constants.h>
#include <game.h>
#include <world.h>
#include <hud.h>
#include <graveyard.h>
#include <hatchery.h>
#include <tdviewer.h>
#include <userinteractionhandler.h>
#include <osgGA/TerrainManipulator>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Group>
#include <osgParticle/FireEffect>

void Game::setCameraManipulator()
{
    osgGA::TerrainManipulator* manipulator = new osgGA::TerrainManipulator();
	manipulator->setVerticalAxisFixed(true);
	manipulator->setWheelZoomFactor(WHEEL_ZOOM_FACTOR);
	manipulator->setAllowThrow(false);

	_viewer->setCameraManipulator(manipulator);
}

void Game::activateAntialiasing(unsigned int samples)
{
	osg::DisplaySettings::instance()->setNumMultiSamples(samples);
}

//FIXME: move into TDviewer
void Game::setGlobalLight()
{
	_viewer->setLightingMode(osg::View::SKY_LIGHT);
	osg::Light* globalLight = new osg::Light;
	globalLight->setLightNum(0);
	globalLight->setAmbient(osg::Vec4(1.0, 1.0, 1.0, 0.0));
	_viewer->setLight(globalLight);
}

void Game::setWindowTitle(const std::string& title)
{
	osgViewer::Viewer::Windows windows;
	_viewer->getWindows(windows);
	for (osgViewer::Viewer::Windows::iterator window = windows.begin(); window != windows.end(); ++window)
	{
		(*window)->setWindowName(title);
	}
}

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

void Game::run()
{
	AssetLibrary::instance()->unmark();
	osg::Group* root = new osg::Group();

	_viewer = new TDViewer();

	World::instance()->loadMap("maps/snake.map");
	root->addChild(World::instance());
	root->addChild(Hud::instance());

	if (getenv("NETBOOK") == NULL)
	{
		activateAntialiasing(NUM_MULTISAMPLES);	//this line must be currently called before calling setUpViewInWindow
		_viewer->setUpViewInWindow(100, 100, 1024, 768);
	} else {
		_viewer->setUpViewInWindow(100, 100, 600, 450); //maybe improve with osgViewer::WindowSizeHandler
	}

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

	//_viewer.addEventHandler( new osgViewer::StatsHandler());

	_viewer->run();
	AssetLibrary::instance()->sweep();
}
