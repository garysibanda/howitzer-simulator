/**********************************************************************
 * Header File:
 *    PROJECTILE
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about a projectile
 ************************************************************************/

#pragma once

#include <list>
#include <vector>
#include "position.h"
#include "velocity.h"
#include "physics.h"
#include "uiDraw.h"

// Forward declarations
class TestProjectile;
class Angle;

// Default M795 projectile specifications
#define DEFAULT_PROJECTILE_WEIGHT 46.7       // kg
#define DEFAULT_PROJECTILE_RADIUS 0.077545   // m (155mm caliber)

/**********************************************************************
 * PositionVelocityTime
 * Structure to keep track of one moment in the path of the projectile
 ************************************************************************/
struct PositionVelocityTime
{
   PositionVelocityTime() : pos(), v(), t(0.0) {}
   PositionVelocityTime(const Position& p, const Velocity& vel, double time)
      : pos(p), v(vel), t(time) {}
   
   Position pos;
   Velocity v;
   double t;
};

/**********************************************************************
 * Projectile
 * Represents an artillery projectile with realistic physics
 * including altitude-dependent drag, gravity, and atmospheric effects
 ************************************************************************/
class Projectile
{
public:
   // Friend the unit test class
   friend ::TestProjectile;
   
   // Typedef for backward compatibility with test files
   typedef ::PositionVelocityTime PositionVelocityTime;
   
   // Create a new projectile with the default M795 specifications
   Projectile() : mass(DEFAULT_PROJECTILE_WEIGHT),
                  radius(DEFAULT_PROJECTILE_RADIUS),
                  isActive(false) {}
   
   // Create projectile with custom specifications
   Projectile(double mass, double radius) : mass(mass),
                                           radius(radius),
                                           isActive(false) {}
   
   // Advance the projectile forward until the next unit of time
   void advance(double simulationTime);
   
   // Reset the projectile to its default state
   void reset();
   
   // Fire the projectile with the given parameters
   void fire(const Position& pos, const Angle& angle, double muzzleVelocity, double time);
   
   // Draw the projectile and its trail
   void draw(ogstream& gout, double flightTime) const;
   
   // Get current position of the projectile
   Position getPosition() const;
   
   // Get current velocity of the projectile
   Velocity getVelocity() const;
   
   // Get flight path for analysis
   const std::list<PositionVelocityTime>& getFlightPath() const { return flightPath; }
   
   // Projectile state queries
   bool isFlying() const { return isActive && !flightPath.empty(); }
   double getFlightTime() const;
   double getMaxAltitude() const;
   double getTotalDistance() const;
   double getCurrentSpeed() const;
   double getCurrentAltitude() const;
   
   // Projectile specifications
   double getMass() const { return mass; }
   double getRadius() const { return radius; }
   void setMass(double newMass) { mass = (newMass > 0.0) ? newMass : mass; }
   void setRadius(double newRadius) { radius = (newRadius > 0.0) ? newRadius : radius; }
   
private:
   // Calculate drag acceleration at current conditions
   Acceleration calculateDragAcceleration(const PositionVelocityTime& pvt) const;
   
   // Calculate total acceleration (gravity + drag)
   Acceleration calculateTotalAcceleration(const PositionVelocityTime& pvt) const;
   
   // Validate projectile state
   bool isValidState() const;
   
   double mass;           // Weight of the projectile in kg
   double radius;         // Radius of projectile in meters
   bool isActive;         // Whether projectile is currently flying
   std::list<PositionVelocityTime> flightPath;  // Complete trajectory history
};
