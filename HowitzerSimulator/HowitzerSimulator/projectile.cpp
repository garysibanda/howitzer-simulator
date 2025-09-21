/***********************************************************************
 * Source File:
 *    PROJECTILE
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about a projectile
 ************************************************************************/

#include "projectile.h"
#include "angle.h"
#include "acceleration.h"
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

/*********************************************
 * PROJECTILE : RESET
 * Reset the projectile to its default state
 *********************************************/
void Projectile::reset()
{
   mass = DEFAULT_PROJECTILE_WEIGHT;
   radius = DEFAULT_PROJECTILE_RADIUS;
   isActive = false;
   flightPath.clear();
}

/*********************************************
 * PROJECTILE : FIRE
 * Launch the projectile with initial conditions
 *********************************************/
void Projectile::fire(const Position& pos, const Angle& angle, double muzzleVelocity, double time)
{
   assert(muzzleVelocity >= 0.0);
   assert(time >= 0.0);
   
   // Clear any existing flight path
   flightPath.clear();
   
   // Create initial state
   PositionVelocityTime pvt(pos, Velocity(), time);
   pvt.v.set(angle, muzzleVelocity);
   
   // Add to flight path and mark as active
   flightPath.push_back(pvt);
   isActive = true;
}

/*********************************************
 * PROJECTILE : ADVANCE
 * Update projectile position using realistic physics
 *********************************************/
void Projectile::advance(double simulationTime)
{
   // Check if projectile is active and has initial state
   if (!isActive || flightPath.empty())
      return;
   
   // Get current state
   const PositionVelocityTime& currentPvt = flightPath.back();
   double deltaTime = simulationTime - currentPvt.t;
   
   // Ensure positive time step
   if (deltaTime <= 0.0)
      return;
   
   // Calculate total acceleration (gravity + drag)
   Acceleration totalAcceleration = calculateTotalAcceleration(currentPvt);
   
   // Create new state using kinematic equations
   PositionVelocityTime newPvt;
   newPvt.pos = currentPvt.pos;
   newPvt.v = currentPvt.v;
   newPvt.t = simulationTime;
   
   // Update position: s = s₀ + v₀t + ½at²
   newPvt.pos.add(totalAcceleration, currentPvt.v, deltaTime);
   
   // Update velocity: v = v₀ + at
   newPvt.v.add(totalAcceleration, deltaTime);
   
   // Add new state to flight path
   flightPath.push_back(newPvt);
   
   // Check if projectile has hit the ground (simplified check)
   if (newPvt.pos.getMetersY() < 0.0)
   {
      isActive = false;
   }
}

/*********************************************
 * PROJECTILE : CALCULATE DRAG ACCELERATION
 * Calculate drag acceleration based on current conditions
 *********************************************/
Acceleration Projectile::calculateDragAcceleration(const PositionVelocityTime& pvt) const
{
   double altitude = max(0.0, pvt.pos.getMetersY());
   double speed = pvt.v.getSpeed();
   
   // Handle zero speed case
   if (speed == 0.0)
      return Acceleration(0.0, 0.0);
   
   // Calculate atmospheric properties
   double density = densityFromAltitude(altitude);
   double speedSound = speedSoundFromAltitude(altitude);
   double machNumber = speed / speedSound;
   double dragCoeff = dragFromMach(machNumber);
   
   // Calculate drag force
   double dragForce = forceFromDrag(density, dragCoeff, radius, speed);
   
   // Convert to acceleration magnitude
   double dragAccelMagnitude = accelerationFromForce(dragForce, mass);
   
   // Apply drag opposite to velocity direction
   double velX = pvt.v.getDX();
   double velY = pvt.v.getDY();
   
   double dragAccelX = -dragAccelMagnitude * (velX / speed);
   double dragAccelY = -dragAccelMagnitude * (velY / speed);
   
   return Acceleration(dragAccelX, dragAccelY);
}

/*********************************************
 * PROJECTILE : CALCULATE TOTAL ACCELERATION
 * Calculate total acceleration (gravity + drag)
 *********************************************/
Acceleration Projectile::calculateTotalAcceleration(const PositionVelocityTime& pvt) const
{
   double altitude = max(0.0, pvt.pos.getMetersY());
   
   // Gravity acceleration (always downward)
   double gravity = gravityFromAltitude(altitude);
   Acceleration gravityAccel(0.0, -gravity);
   
   // Drag acceleration (opposite to velocity)
   Acceleration dragAccel = calculateDragAcceleration(pvt);
   
   // Combine accelerations
   return gravityAccel + dragAccel;
}

/*********************************************
 * PROJECTILE : IS VALID STATE
 * Check if projectile is in a valid state
 *********************************************/
bool Projectile::isValidState() const
{
   return (mass > 0.0) && (radius > 0.0);
}

/*********************************************
 * PROJECTILE : DRAW
 * Draw the projectile and its trail
 *********************************************/
void Projectile::draw(ogstream& gout, double flightTime) const
{
   if (!isActive || flightPath.empty())
      return;
   
   // Draw current projectile position
   gout.drawProjectile(flightPath.back().pos, flightTime);
}

/*********************************************
 * PROJECTILE : GET POSITION
 * Return the current position of the projectile
 *********************************************/
Position Projectile::getPosition() const
{
   if (!flightPath.empty())
   {
      return flightPath.back().pos;
   }
   return Position(); // Return origin if no flight path
}

/*********************************************
 * PROJECTILE : GET VELOCITY
 * Return the current velocity of the projectile
 *********************************************/
Velocity Projectile::getVelocity() const
{
   if (!flightPath.empty())
   {
      return flightPath.back().v;
   }
   return Velocity(); // Return zero velocity if no flight path
}

/*********************************************
 * PROJECTILE : GET FLIGHT TIME
 * Return total flight time
 *********************************************/
double Projectile::getFlightTime() const
{
   if (flightPath.size() < 2)
      return 0.0;
   
   return flightPath.back().t - flightPath.front().t;
}

/*********************************************
 * PROJECTILE : GET MAX ALTITUDE
 * Find the maximum altitude reached during flight
 *********************************************/
double Projectile::getMaxAltitude() const
{
   double maxAlt = 0.0;
   
   for (const auto& pvt : flightPath)
   {
      maxAlt = max(maxAlt, pvt.pos.getMetersY());
   }
   
   return maxAlt;
}

/*********************************************
 * PROJECTILE : GET TOTAL DISTANCE
 * Calculate total horizontal distance traveled
 *********************************************/
double Projectile::getTotalDistance() const
{
   if (flightPath.size() < 2)
      return 0.0;
   
   double startX = flightPath.front().pos.getMetersX();
   double endX = flightPath.back().pos.getMetersX();
   
   return abs(endX - startX);
}

/*********************************************
 * PROJECTILE : GET CURRENT SPEED
 * Return current speed of the projectile
 *********************************************/
double Projectile::getCurrentSpeed() const
{
   if (!flightPath.empty())
   {
      return flightPath.back().v.getSpeed();
   }
   return 0.0;
}

/*********************************************
 * PROJECTILE : GET CURRENT ALTITUDE
 * Return current altitude of the projectile
 *********************************************/
double Projectile::getCurrentAltitude() const
{
   if (!flightPath.empty())
   {
      return max(0.0, flightPath.back().pos.getMetersY());
   }
   return 0.0;
}
