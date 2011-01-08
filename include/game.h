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

		osgViewer::Viewer _viewer;
};