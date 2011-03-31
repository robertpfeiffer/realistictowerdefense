// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-
#include <tdviewer.h>

#include <graveyard.h>
#include <hatchery.h>
#include <constants.h>
#include <gametimer.h>

// do not zoom too far in/out and do not look at the field from below
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

// we use the osg time in the viewer for rendering
// our own timer in only for the game logic

/**
 * \fn	void TDViewer::frame(double time)
 *
 * \brief	Render next frame.
 * 			Update gametimer and remove at end of frame all unnecessary nodes.
 *
 * \author	Sun Black
 * \date	31.03.2011
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

