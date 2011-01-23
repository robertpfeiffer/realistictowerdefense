#include <constants.h>
#include <game.h>
#include <world.h>
#include <hud.h>
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
	osg::DisplaySettings* displaySettings = new osg::DisplaySettings;
    displaySettings->setNumMultiSamples(samples);
	_viewer.setDisplaySettings(displaySettings);
}

void Game::setGlobalLight()
{
	_viewer.setLightingMode(osg::View::SKY_LIGHT);
	osg::Light* globalLight = new osg::Light;
	globalLight->setLightNum(0);
	globalLight->setAmbient(osg::Vec4(1.0, 1.0, 1.0, 0.0));
	_viewer.setLight(globalLight);
}

void Game::run()
{
	osg::Group* root = new osg::Group();
	root->addChild(new World("maps/default.map"));
	root->addChild(new Hud());

	_viewer.setUpViewInWindow( 100, 100, 1024, 768 ); //maybe improve with osgViewer::WindowSizeHandler

	activateAntialiasing(NUM_MULTISAMPLES);
	setCameraManipulator();
	setGlobalLight();

	_viewer.getCamera()->setClearColor(osg::Vec4(0, 0, 0, 0));

	_viewer.setSceneData(root);
	_viewer.realize();

	//viewer.run();
    while( !_viewer.done() )
    {
		limitCamera((osgGA::TerrainManipulator*) _viewer.getCameraManipulator());
        _viewer.frame();
    }
}