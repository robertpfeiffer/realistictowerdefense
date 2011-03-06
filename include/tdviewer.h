// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-
#pragma once

#include <osgViewer/Viewer>
#include <osgGA/TerrainManipulator>

class TDViewer : public osgViewer::Viewer
{
public:
	TDViewer(osg::ArgumentParser& arguments): osgViewer::Viewer(arguments) {};

	virtual void frame(double time);

	void limitCamera(osgGA::TerrainManipulator* manipulator);
};
