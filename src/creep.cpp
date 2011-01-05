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
#include "OpenSteer/Pathway.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/OpenSteerDemo.h"
#include "OpenSteer/Proximity.h"




using namespace OpenSteer;


// ----------------------------------------------------------------------------


typedef AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
typedef AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;


// ----------------------------------------------------------------------------


// creates a path for the PlugIn
ObstacleGroup gObstacles;
Vec3 gEndpoint0;
Vec3 gEndpoint1;
bool gUseDirectedPathFollowing = true;

// this was added for debugging tool, but I might as well leave it in
bool gWanderSwitch = true;


// ----------------------------------------------------------------------------


class Creep : public SimpleVehicle
{
public:

    // type for a group of Pedestrians
    typedef std::vector<Pedestrian*> groupType;

    // constructor
    Creep (ProximityDatabase& pd)
    {
        // allocate a token for this boid in the proximity database
        proximityToken = NULL;
        newPD (pd);

        // reset Pedestrian state
        reset ();
    }

    // destructor
    virtual ~Creep ()
    {
        // delete this boid's token in the proximity database
        delete proximityToken;
    }

    // reset all instance state
    void reset (void)
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
        path = getTestPath ();

        // set initial position
        // (random point on path + random horizontal offset)
        const float d = path->getTotalPathLength() * frandom01();
        const float r = path->radius;
        const Vec3 randomOffset = randomVectorOnUnitRadiusXZDisk () * r;
        setPosition (path->mapPathDistanceToPoint (d) + randomOffset);

        // randomize 2D heading
        randomizeHeadingOnXZPlane ();

        // pick a random direction for path following (upstream or downstream)
        pathDirection = (frandom01() > 0.5) ? -1 : +1;

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

        // reverse direction when we reach an endpoint
        if (gUseDirectedPathFollowing)
        {
            const Vec3 darkRed (0.7f, 0, 0);

            if (Vec3::distance (position(), gEndpoint0) < path->radius)
            {
                pathDirection = +1;
                annotationXZCircle (path->radius, gEndpoint0, darkRed, 20);
            }
            if (Vec3::distance (position(), gEndpoint1) < path->radius)
            {
                pathDirection = -1;
                annotationXZCircle (path->radius, gEndpoint1, darkRed, 20);
            }
        }

        // annotation
        annotationVelocityAcceleration (5, 0);
        recordTrailVertex (currentTime, position());

        // notify proximity database that our position has changed
        proximityToken->updateForNewPosition (position());
    }

    // compute combined steering force: move forward, avoid obstacles
    // or neighbors if needed, otherwise follow the path and wander
    Vec3 determineCombinedSteering (const float elapsedTime)
    {
        // move forward
        Vec3 steeringForce = forward();

        // probability that a lower priority behavior will be given a
        // chance to "drive" even if a higher priority behavior might
        // otherwise be triggered.
        const float leakThrough = 0.1f;

        // determine if obstacle avoidance is required
        Vec3 obstacleAvoidance;
        if (leakThrough < frandom01())
        {
            const float oTime = 6; // minTimeToCollision = 6 seconds
            obstacleAvoidance = steerToAvoidObstacles (oTime, gObstacles);
        }

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

            if (leakThrough < frandom01())
                collisionAvoidance =
                    steerToAvoidNeighbors (caLeadTime, neighbors) * 10;

            // if collision avoidance is needed, do it
            if (collisionAvoidance != Vec3::zero)
            {
                steeringForce += collisionAvoidance;
            }
            else
            {
                // add in wander component (according to user switch)
                if (gWanderSwitch)
                    steeringForce += steerForWander (elapsedTime);

                // do (interactively) selected type of path following
                const float pfLeadTime = 3;
                const Vec3 pathFollow =
                    (gUseDirectedPathFollowing ?
                     steerToFollowPath (pathDirection, pfLeadTime, *path) :
                     steerToStayOnPath (pfLeadTime, *path));

                // add in to steeringForce
                steeringForce += pathFollow * 0.5;
            }
        }

        // return steering constrained to global XZ "ground" plane
        return steeringForce.setYtoZero ();
    }

    // switch to new proximity database -- just for demo purposes
    void newPD (ProximityDatabase& pd)
    {
        // delete this boid's token in the old proximity database
        delete proximityToken;

        // allocate a token for this boid in the proximity database
        proximityToken = pd.allocateToken (this);
    }

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
};


AVGroup Pedestrian::neighbors;