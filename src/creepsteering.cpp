// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
// ----------------------------------------------------------------------------
// The following code is based on the pedestrian PlugIn in opensteer.
// The original copyright-header follows
// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2003, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------
//
//
// An autonomous "pedestrian":
// follows paths, avoids collisions with obstacles and other pedestrians
//
// 10-29-01 cwr: created
//
//
// ----------------------------------------------------------------------------


#include <creepsteering.h>
#include <world.h>

OpenSteer::AVGroup CreepSteering::neighbors;

float CreepSteering::_followPathWeight		= 8.0f;
float CreepSteering::_avoidCollisionWeight	= 5.0f;

float CreepSteering::_pathFollowLeadTime		 = 0.6f;
float CreepSteering::_collisionAvoidanceLeadTime = 4.0f;

CreepSteering::CreepSteering(ProximityDatabase& pd, OpenSteer::Vec3 startPosition, OpenSteer::PolylineSegmentedPathwaySingleRadius* runPath, Creep* creep, World* eventHandler)
{
    proximityToken = pd.allocateToken (this);        
	_eventHandler = eventHandler;
	_creep = creep;

    init (startPosition, runPath);
}

// destructor
CreepSteering::~CreepSteering()
{
    // delete this boid's token in the proximity database
    delete proximityToken;
}

// reset all instance state
void CreepSteering::init (OpenSteer::Vec3 startPosition, OpenSteer::PolylineSegmentedPathwaySingleRadius* runPath)
{
    // reset the vehicle 
    SimpleVehicle::reset ();

    // max speed and max steering force (maneuverability) 
    setMaxSpeed (2.0);
    setMaxForce (8.0);

    // initially stopped
    setSpeed (0);

    // size of bounding sphere, for obstacle avoidance, etc.
    setRadius (0.5); // width = 0.7, add 0.3 margin, take half

    // set the path for this Pedestrian to follow
    path = runPath;

    // set initial position (path-beginning)
    setPosition (startPosition);

    // head into path-direction
	setUp(OpenSteer::Vec3::up);
	setForward((path->point(1)-path->point(0)).normalize());
	setSide(localRotateForwardToSide (forward()));

    // follow the path in pathDirection
    pathDirection = +1;

    // notify proximity database that our position has changed
    proximityToken->updateForNewPosition (position());
}

//needs to be overridden
void CreepSteering::update (const float, const float elapsedTime)
{
	update(elapsedTime);
}

// per frame simulation update
void CreepSteering::update (const float elapsedTime)
{
    // apply steering force to our momentum
    applySteeringForce (determineCombinedSteering (elapsedTime),
                        elapsedTime);

    // notify proximity database that our position has changed
    proximityToken->updateForNewPosition (position());

	if(path->mapPointToPathDistance(position()) >= path->length())
	{
		RaiseLeakEvent();
	}
}

void CreepSteering::RaiseLeakEvent()
{
	_eventHandler->onLeak(_creep);
}

// compute combined steering force: move forward, avoid obstacles
// or neighbors if needed, otherwise follow the path and wander
OpenSteer::Vec3 CreepSteering::determineCombinedSteering (const float elapsedTime)
{
    // move forward
    OpenSteer::Vec3 steeringForce = forward();
	
	// follow the path

	const OpenSteer::Vec3 pathFollow = steerToFollowPath (pathDirection, _pathFollowLeadTime, *path);
	//const OpenSteer::Vec3 pathFollow = steerToStayOnPath (_pathFollowLeadTime, *path);

	if (pathFollow != OpenSteer::Vec3::zero)
    {
		steeringForce += pathFollow * _followPathWeight;
	}
	else
	{
		// consider avoiding collisions with others
		OpenSteer::Vec3 collisionAvoidance;

		//find neighbours in a circle in front of this creep
		//the circle only contains neighbours, that are reachable within
		// the collisionAvoidanceLeadTime (considering actual speed)
		const float radius = _collisionAvoidanceLeadTime * speed() * 0.5;
		neighbors.clear();
		proximityToken->findNeighbors(position()+(forward()*radius), radius, neighbors);

		collisionAvoidance = steerToAvoidNeighbors (_collisionAvoidanceLeadTime, neighbors);

		// if collision avoidance is needed, do it
		if (collisionAvoidance != OpenSteer::Vec3::zero)
		{
			steeringForce += collisionAvoidance * _avoidCollisionWeight;
		}
	}

    // return steering constrained to global XZ "ground" plane
    return steeringForce.setYtoZero ();
}
