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

OpenSteer::AVGroup CreepSteering::neighbors;
OpenSteer::ObstacleGroup CreepSteering::gObstacles;
float CreepSteering::_followPathWeight		= 0.5;
float CreepSteering::_avoidCollisionWeight	= 10.0;
float CreepSteering::_avoidObstaclesWeight	= 1.0;

float CreepSteering::_pathFollowLeadTime		 = 3.0;
float CreepSteering::_obstacleAvoidanceLeadTime	 = 6.0;
float CreepSteering::_collisionAvoidanceLeadTime = 3.0;

CreepSteering::CreepSteering(ProximityDatabase& pd, OpenSteer::Vec3 startPosition, OpenSteer::PolylinePathway* runPath)
{
    proximityToken = pd.allocateToken (this);        

    init (startPosition, runPath);
}

// destructor
CreepSteering::~CreepSteering()
{
    // delete this boid's token in the proximity database
    delete proximityToken;
}

// reset all instance state
void CreepSteering::init (OpenSteer::Vec3 startPosition, OpenSteer::PolylinePathway* runPath)
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
	setForward((path->points[1]-path->points[0]).normalize());
	setSide(localRotateForwardToSide (forward()));

    // follow the path in pathDirection
    pathDirection = +1;

    // notify proximity database that our position has changed
    proximityToken->updateForNewPosition (position());
}

// per frame simulation update
void CreepSteering::update (const float elapsedTime)
{
    // apply steering force to our momentum
    applySteeringForce (determineCombinedSteering (elapsedTime),
                        elapsedTime);

    // notify proximity database that our position has changed
    proximityToken->updateForNewPosition (position());
}

// compute combined steering force: move forward, avoid obstacles
// or neighbors if needed, otherwise follow the path and wander
OpenSteer::Vec3 CreepSteering::determineCombinedSteering (const float elapsedTime)
{
    // move forward
    OpenSteer::Vec3 steeringForce = forward();

    // determine if obstacle avoidance is required
    OpenSteer::Vec3 obstacleAvoidance = steerToAvoidObstacles (_obstacleAvoidanceLeadTime, gObstacles);

    // if obstacle avoidance is needed, do it
    if (obstacleAvoidance != OpenSteer::Vec3::zero)
    {
        steeringForce += obstacleAvoidance * _avoidObstaclesWeight;
    }
    else
    {
        // otherwise consider avoiding collisions with others
        OpenSteer::Vec3 collisionAvoidance;

        // find all neighbors within maxRadius using proximity database
        // (radius is largest distance between vehicles traveling head-on
        // where a collision is possible within caLeadTime seconds.)
        const float maxRadius = _collisionAvoidanceLeadTime * maxSpeed() * 2;
        neighbors.clear();
        proximityToken->findNeighbors(position(), maxRadius, neighbors);

        collisionAvoidance = steerToAvoidNeighbors (_collisionAvoidanceLeadTime, neighbors);

        // if collision avoidance is needed, do it
        if (collisionAvoidance != OpenSteer::Vec3::zero)
        {
            steeringForce += collisionAvoidance * _avoidCollisionWeight;
        }
        else
        {
            // add in wander component
            //steeringForce += steerForWander (elapsedTime);

			//see whats the best mechanism to use here
			const OpenSteer::Vec3 pathFollow = steerToFollowPath (pathDirection, _pathFollowLeadTime, *path);
            //const OpenSteer::Vec3 pathFollow = steerToStayOnPath (pfLeadTime, *path);

            // add in to steeringForce
            steeringForce += pathFollow * _followPathWeight;
        }
    }

    // return steering constrained to global XZ "ground" plane
    return steeringForce.setYtoZero ();
}