#pragma once

#include <map.h>
#include <osg/Node>
#include <osgGA/TerrainManipulator>
#include <osgViewer/Viewer>

class Game
{
	public:
		void run();
	private:
		void activateAntialiasing(unsigned int samples);
		void setGlobalLight();
		void setCameraManipulator();
		void limitCamera(osgGA::TerrainManipulator* manipulator);
		void setWindowTitle(const std::string& title);

		osgViewer::Viewer _viewer;
};