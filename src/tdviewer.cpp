// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-
#include <graveyard.h>
#include <hatchery.h>
#include <tdviewer.h>
#include <constants.h>
#include <gametimer.h>

void TDViewer::limitCamera(osgGA::TerrainManipulator* manipulator)
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

void TDViewer::frame(double time)
{
	//ViewerBase::frame(time);
	Viewer::frame(time);
	GameTimer::instance()->nextFrame();
    limitCamera((osgGA::TerrainManipulator*) getCameraManipulator());
    Graveyard::instance()->burryAll();
    Hatchery::instance()->releaseAll();
}

