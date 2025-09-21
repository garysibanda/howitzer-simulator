/***********************************************************************
 * Header File:
 *    PHYSICS
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Laws of motion, effects of gravity, wind resistance, etc.
 ************************************************************************/

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>

/*******************************************************
 * AREA FROM RADIUS
 * Compute the area of a circle from the radius
 *    area = π * radius²
 ********************************************************/
inline double areaFromRadius(double radius)
{
   assert(radius >= 0.0);  // Radius cannot be negative
   return M_PI * (radius * radius);
}

/**********************************************************
 * FORCE FROM DRAG
 * Determine the drag on a shell based on several factors:
 *    density  : The density of the air (kg/m³)
 *    drag     : The drag coefficient of the shell (dimensionless)
 *    radius   : The radius of the shell (m)
 *    velocity : The velocity of the shell (m/s)
 * This force is determined by:
 *    force = ½ * density * drag * area * velocity²
 ************************************************************/
inline double forceFromDrag(double density, double drag, double radius, double velocity)
{
   assert(density >= 0.0);   // Density cannot be negative
   assert(drag >= 0.0);      // Drag coefficient cannot be negative
   assert(radius >= 0.0);    // Radius cannot be negative
   assert(velocity >= 0.0);  // We use speed (magnitude), so no negative
   
   return 0.5 * density * drag * areaFromRadius(radius) * (velocity * velocity);
}

/**********************************************************
 * ACCELERATION FROM FORCE
 * How much acceleration can we expect from a given force
 *    force        : N (Newtons)
 *    mass         : kg
 *    acceleration : m/s²
 * Equation: F = ma, so a = F/m
 ************************************************************/
inline double accelerationFromForce(double force, double mass)
{
   assert(mass > 0.0);  // Mass must be positive
   return force / mass;
}

/***********************************************************
 * VELOCITY FROM ACCELERATION
 * Determine how much velocity will be imparted in a given amount of time
 * from a given acceleration
 *    acceleration : m/s²
 *    time         : s
 * Equation: v = at
 ************************************************************/
inline double velocityFromAcceleration(double acceleration, double time)
{
   assert(time >= 0.0);  // Time cannot be negative
   return acceleration * time;
}

/*********************************************************
 * LINEAR INTERPOLATION
 * From two points, linear interpolate to find a value
 * between them
 *
 *    |        *(d1,r1)
 *    |       /
 *    |      *(d,r)
 *   r|     /
 *    |    *(d0,r0)
 *    |
 *    +-------------
 *           d
 * equation:
 *   (r - r0) / (d - d0) = (r1 - r0) / (d1 - d0)
 * Thus:
 *   r = r0 + (r1 - r0) * (d - d0) / (d1 - d0)
 *********************************************************/
inline double linearInterpolation(double d0, double r0, double d1, double r1, double d)
{
   assert(d1 != d0);  // Avoid division by zero
   return r0 + (r1 - r0) * (d - d0) / (d1 - d0);
}

/*********************************************************
 * MAPPING
 * A structure to represent domain and range pairs
 * Used for linear interpolation with lookup tables
 *********************************************************/
struct Mapping
{
   double domain;
   double range;
   
   // Constructor for convenience
   Mapping(double domain = 0.0, double range = 0.0) : domain(domain), range(range) {}
};

/*********************************************************
 * LINEAR INTERPOLATION WITH MAPPING
 * From a list of domain/range pairs, linear interpolate
 *********************************************************/
double linearInterpolation(const Mapping mapping[], int numMapping, double domain);

/*********************************************************
 * GRAVITY FROM ALTITUDE
 * Determine gravity coefficient based on the altitude
 * Uses realistic atmospheric model
 *********************************************************/
double gravityFromAltitude(double altitude);

/*********************************************************
 * DENSITY FROM ALTITUDE
 * Determine the density of air based on the altitude
 * Uses standard atmospheric model
 *********************************************************/
double densityFromAltitude(double altitude);

/*********************************************************
 * SPEED OF SOUND FROM ALTITUDE
 * Determine the speed of sound for a given altitude
 * Accounts for temperature variation with altitude
 *********************************************************/
double speedSoundFromAltitude(double altitude);

/*********************************************************
 * DRAG FROM MACH
 * Determine the drag coefficient as a function of Mach number
 * Specific to M795 artillery shell characteristics
 *********************************************************/
double dragFromMach(double speedMach);

/*********************************************************
 * MACH FROM SPEED
 * Calculate Mach number from speed and altitude
 *********************************************************/
inline double machFromSpeed(double speed, double altitude)
{
   double speedSound = speedSoundFromAltitude(altitude);
   assert(speedSound > 0.0);  // Speed of sound should be positive
   return speed / speedSound;
}
