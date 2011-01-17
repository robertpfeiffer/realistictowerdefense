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




using namespace OpenSteer;


// ----------------------------------------------------------------------------


typedef AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
typedef AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;

// ----------------------------------------------------------------------------


class CreepSteering : public SimpleVehicle
{
public:
	// creates a path for the PlugIn
	static ObstacleGroup gObstacles;

    // type for a group of Pedestrians
    typedef std::vector<CreepSteering*> groupType;

	// a pointer to this boid's interface object for the proximity database
    ProximityToken* proximityToken;

    // allocate one and share amoung instances just to save memory usage
    // (change to per-instance allocation to be more MP-safe)
    static AVGroup neighbors;

    // path to be followed by this pedestrian
    // XXX Ideally this should be a generic Pathway, but we use the
    // XXX getTotalPathLength and radius methods (currently defined only
    // XXX on PolylinePathway) to set random initial positions.  Could
    // XXX there be a "random position inside path" method on Pathway?
    PolylinePathway* path;

    // direction for path following (upstream or downstream)
    int pathDirection;

    CreepSteering(ProximityDatabase& pd, PolylinePathway* runPath)
    {
        proximityToken = pd.allocateToken (this);        

        init (runPath);
    }

    // destructor
    virtual ~CreepSteering()
    {
        // delete this boid's token in the proximity database
        delete proximityToken;
    }

    // reset all instance state
    void init (PolylinePathway* runPath)
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
        setPosition (path->mapPathDistanceToPoint (0));

        // randomize 2D heading
        randomizeHeadingOnXZPlane ();

        // follow the path in pathDirection
        pathDirection = +1;

        // trail parameters: 3 seconds with 60 points along the trail
        setTrailParameters (3, 60);

        // notify proximity database that our position has changed
        proximityToken->updateForNewPosition (position());
    }

    // per frame simulation update
    void update (const float currentTime, const float elapsedTime)
    {
        // apply steering force to our momentum
        applySteeringForce (determineCombinedSteering (elapsedTime),
                            elapsedTime);

        // notify proximity database that our position has changed
        proximityToken->updateForNewPosition (position());
    }

    // compute combined steering force: move forward, avoid obstacles
    // or neighbors if needed, otherwise follow the path and wander
    Vec3 determineCombinedSteering (const float elapsedTime)
    {
        // move forward
        Vec3 steeringForce = forward();

        // determine if obstacle avoidance is required
        const float oTime = 6; // minTimeToCollision = 6 seconds
        Vec3 obstacleAvoidance = steerToAvoidObstacles (oTime, gObstacles);

        // if obstacle avoidance is needed, do it
        if (obstacleAvoidance != Vec3::zero)
        {
            steeringForce += obstacleAvoidance;
        }
        else
        {
            // otherwise consider avoiding collisions with others
            Vec3 collisionAvoidance;
            const float caLeadTime = 3;

            // find all neighbors within maxRadius using proximity database
            // (radius is largest distance between vehicles traveling head-on
            // where a collision is possible within caLeadTime seconds.)
            const float maxRadius = caLeadTime * maxSpeed() * 2;
            neighbors.clear();
            proximityToken->findNeighbors (position(), maxRadius, neighbors);

            collisionAvoidance = steerToAvoidNeighbors (caLeadTime, neighbors) * 10;

            // if collision avoidance is needed, do it
            if (collisionAvoidance != Vec3::zero)
            {
                steeringForce += collisionAvoidance;
            }
            else
            {
                // add in wander component
                //steeringForce += steerForWander (elapsedTime);

                // do (interactively) selected type of path following
                const float pfLeadTime = 3;

				//see whats the best mechanism to use here
                const Vec3 pathFollow = steerToFollowPath (pathDirection, pfLeadTime, *path);
                //const Vec3 pathFollow = steerToStayOnPath (pfLeadTime, *path));

                // add in to steeringForce
                steeringForce += pathFollow * 0.5;
            }
        }

        // return steering constrained to global XZ "ground" plane
        return steeringForce.setYtoZero ();
    }
};

AVGroup CreepSteering::neighbors;
ObstacleGroup CreepSteering::gObstacles;