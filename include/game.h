#pragma once

#include <map.h>
#include <event_handler.h>

#include <osg/Node>
#include <osgGA/TerrainManipulator>
#include <osgViewer/Viewer>

class Game : EventHandler
{
	public:
		void run();
	private:
		void activateAntialiasing(unsigned int samples);
		void setGlobalLight();
		void setCameraManipulator();
		void limitCamera(osgGA::TerrainManipulator* manipulator);
		void setWindowTitle(const std::string& title);

		void onKeyDown(osgGA::GUIActionAdapter& aa);

		osgViewer::Viewer _viewer;
};