#include <constants.h>
#include <game.h>
#include <world.h>
#include <hud.h>
#include <graveyard.h>
#include <user_interaction_handler.h>
#include <osgGA/TerrainManipulator>

void Game::setCameraManipulator()
{
    osgGA::TerrainManipulator* manipulator = new osgGA::TerrainManipulator();
	manipulator->setVerticalAxisFixed(true);
	manipulator->setWheelZoomFactor(WHEEL_ZOOM_FACTOR);
	manipulator->setAllowThrow(false);

	_viewer.setCameraManipulator(manipulator);
}

void Game::limitCamera(osgGA::TerrainManipulator* manipulator)
{
	if(manipulator->getElevation() > -MIN_ELEVATION) {
		manipulator->setElevation(MIN_ELEVATION);
	}

	if(manipulator->getDistance() < MIN_DISTANCE){
		manipulator->setDistance(MIN_DISTANCE);
	}
	else if(manipulator->getDistance() > MAX_DISTANCE){
		manipulator->setDistance(MAX_DISTANCE);
	}
}

void Game::activateAntialiasing(unsigned int samples)
{
	osg::DisplaySettings::instance()->setNumMultiSamples(samples);
}

void Game::setGlobalLight()
{
	_viewer.setLightingMode(osg::View::SKY_LIGHT);
	osg::Light* globalLight = new osg::Light;
	globalLight->setLightNum(0);
	globalLight->setAmbient(osg::Vec4(1.0, 1.0, 1.0, 0.0));
	_viewer.setLight(globalLight);
}

void Game::setWindowTitle(const std::string& title)
{
	osgViewer::Viewer::Windows windows;	
	_viewer.getWindows(windows);
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
		default:
			break;
	}
}

void Game::run()
{
	osg::Group* root = new osg::Group();
	root->addChild(new World("maps/default.map"));
	root->addChild(new Hud());

	activateAntialiasing(NUM_MULTISAMPLES);	//this line must be currently called before calling setUpViewInWindow
	_viewer.setUpViewInWindow(100, 100, 1024, 768); //maybe improve with osgViewer::WindowSizeHandler

		
	setCameraManipulator();
	setGlobalLight();

	_viewer.getCamera()->setClearColor(osg::Vec4(0, 0, 0, 0));

	_viewer.setSceneData(root);

	//this must be called after realize()
	setWindowTitle("Towerdefense");

	_gameTimer = GameTimer::instance();

	_interactionHandler = new UserInteractionHandler();
	_viewer.addEventHandler(getInteractiontHandler());

	//_eventHandler->registerKeyDownEvent(osgGA::GUIEventAdapter::MODKEY_LEFT_ALT, osgGA::GUIEventAdapter::KEY_Return, this, EVENT_FULLSCREEN);
	_interactionHandler->registerKeyEvent(0, 'p', this, EVENT_PAUSE);

	//viewer.run();
    while (!_viewer.done())
    {
		limitCamera((osgGA::TerrainManipulator*) _viewer.getCameraManipulator());
		_viewer.frame(_gameTimer->nextFrame());
		Graveyard::instance()->burryAll();
    }
}