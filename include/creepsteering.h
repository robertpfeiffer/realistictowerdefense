#pragma once
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


#include <iomanip>
#include <sstream>
#include <OpenSteer/Pathway.h>
#include <OpenSteer/SimpleVehicle.h>
#include <OpenSteer/Proximity.h>

// ----------------------------------------------------------------------------


typedef OpenSteer::AbstractProximityDatabase<OpenSteer::AbstractVehicle*> ProximityDatabase;
typedef OpenSteer::AbstractTokenForProximityDatabase<OpenSteer::AbstractVehicle*> ProximityToken;

// ----------------------------------------------------------------------------


class CreepSteering : public OpenSteer::SimpleVehicle
{
public:
	// creates a path for the PlugIn
	static OpenSteer::ObstacleGroup gObstacles;

    // type for a group of Pedestrians
    typedef std::vector<CreepSteering*> groupType;

	// a pointer to this boid's interface object for the proximity database
    ProximityToken* proximityToken;

    // allocate one and share amoung instances just to save memory usage
    // (change to per-instance allocation to be more MP-safe)
    static OpenSteer::AVGroup neighbors;

    // path to be followed by this pedestrian
    // XXX Ideally this should be a generic Pathway, but we use the
    // XXX getTotalPathLength and radius methods (currently defined only
    // XXX on PolylinePathway) to set random initial positions.  Could
    // XXX there be a "random position inside path" method on Pathway?
    OpenSteer::PolylinePathway* path;

    // direction for path following (upstream or downstream)
    int pathDirection;

	CreepSteering(ProximityDatabase& pd, OpenSteer::Vec3 startPosition, OpenSteer::PolylinePathway* runPath);

    // destructor
    virtual ~CreepSteering();

    // reset all instance state
    void init (OpenSteer::Vec3 startPosition, OpenSteer::PolylinePathway* runPath);

    // per frame simulation update
    void update (const float currentTime, const float elapsedTime);

    // compute combined steering force: move forward, avoid obstacles
    // or neighbors if needed, otherwise follow the path and wander
    OpenSteer::Vec3 determineCombinedSteering (const float elapsedTime);
};