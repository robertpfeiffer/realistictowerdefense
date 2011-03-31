// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-
#include <tdviewer.h>

#include <graveyard.h>
#include <hatchery.h>
#include <constants.h>
#include <gametimer.h>

/**
 * \fn	void TDViewer::limitCamera(osgGA::TerrainManipulator* manipulator)
 *
 * \brief	Limit camera distance and perspective, so the player can not look from below.
 *
 * \param [in,out]	manipulator	If non-null, the manipulator.
 */
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

/**
 * \fn	void TDViewer::frame(double time)
 *
 * \brief	Render next frame.
 * 			Update gametimer and remove at end of frame all unnecessary nodes.
 *
 * \param	time	The time.
 */
void TDViewer::frame(double time)
{
	// Call the superclass method
	Viewer::frame(time);

    // game-specific stuff that must be done every frame
	GameTimer::instance()->nextFrame();
    limitCamera((osgGA::TerrainManipulator*) getCameraManipulator());
    Graveyard::instance()->burryAll();
    Hatchery::instance()->releaseAll();
}

