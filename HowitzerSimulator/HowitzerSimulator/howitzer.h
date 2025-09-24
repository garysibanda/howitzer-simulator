/**********************************************************************
 * Header File:
 *    HOWITZER
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about a howitzer (aka the gun)
 ************************************************************************/

#pragma once

#include <list>
#include "angle.h"
#include "position.h"
#include "velocity.h"
#include "physics.h"
#include "uiDraw.h"

// Default M777 Howitzer specifications
#define DEFAULT_MUZZLE_VELOCITY   827.00     // m/s
#define DEFAULT_ELEVATION_ANGLE   45.0       // degrees
#define MIN_ELEVATION_ANGLE       0.0        // degrees (CHANGED: was -5.0)
#define MAX_ELEVATION_ANGLE      85.0        // degrees

// Forward declaration for unit tests
class TestHowitzer;

/*********************************************
 * Howitzer
 * Represents an M777 155mm howitzer with
 * realistic operational constraints
 *********************************************/
class Howitzer
{
public:
   // for unit tests
   friend ::TestHowitzer;
   
   // Default constructor with M777 specifications
   Howitzer() : muzzleVelocity(DEFAULT_MUZZLE_VELOCITY),
                elevation(DEFAULT_ELEVATION_ANGLE),
                lastFireTime(-1.0),
                roundsFired(0) {}
   
   // Constructor with custom specifications
   Howitzer(double muzzleVel, double elevAngle) :
      muzzleVelocity(muzzleVel),
      elevation(elevAngle),
      lastFireTime(-1.0),
      roundsFired(0) {}
   
   // Drawing
   void draw(ogstream& gout, double flightTime) const
   {
      gout.drawHowitzer(position, elevation.getRadians(), flightTime);
   }
   
   // Position management
   Position& getPosition() { return position; }
   const Position& getPosition() const { return position; }
   void setPosition(const Position& pos) { position = pos; }
   
   // Generate a new random position for the howitzer
   void generatePosition(const Position& posUpperRight)
   {
      double xPixels = random(posUpperRight.getPixelsX() * 0.1,
                              posUpperRight.getPixelsX() * 0.9);
      position.setPixelsX(xPixels);
      position.setPixelsY(0.0); // Always on ground level
   }
   
   // Muzzle velocity management
   double getMuzzleVelocity() const { return muzzleVelocity; }
   void setMuzzleVelocity(double velocity)
   {
      muzzleVelocity = (velocity > 0.0) ? velocity : muzzleVelocity;
   }
   
   // Elevation management
   const Angle& getElevation() const { return elevation; }
   void setElevation(const Angle& angle)
   {
      elevation = angle;
      constrainElevation();
   }
   void setElevation(double degrees)
   {
      elevation.setDegrees(degrees);
      constrainElevation();
   }
   
   // Rotation controls - with pre-checking to prevent skipping
   void rotate(double radians)
   {
      // Convert radians to degrees for checking
      double deltaeDegrees = radians * (180.0 / M_PI);
      double currentDegrees = elevation.getDegrees();
      double newDegrees = currentDegrees + deltaeDegrees;
      
      // Only allow the rotation if it stays within bounds
      if (newDegrees >= MIN_ELEVATION_ANGLE && newDegrees <= MAX_ELEVATION_ANGLE)
      {
         elevation.add(radians);
      }
      // If it would go out of bounds, clamp to the nearest limit
      else if (newDegrees < MIN_ELEVATION_ANGLE)
      {
         elevation.setDegrees(MIN_ELEVATION_ANGLE);
      }
      else if (newDegrees > MAX_ELEVATION_ANGLE)
      {
         elevation.setDegrees(MAX_ELEVATION_ANGLE);
      }
   }
   
   // Elevation controls (raise/lower) - with pre-checking
   void raise(double radians)
   {
      // Convert radians to degrees for checking
      double deltaeDegrees = radians * (180.0 / M_PI);
      double currentDegrees = elevation.getDegrees();
      
      // Apply the logic based on direction
      double actualDelta;
      if (elevation.isRight())
         actualDelta = -deltaeDegrees;  // Moving toward vertical
      else
         actualDelta = deltaeDegrees;   // Moving toward vertical
      
      double newDegrees = currentDegrees + actualDelta;
      
      // Only allow the change if it stays within bounds
      if (newDegrees >= MIN_ELEVATION_ANGLE && newDegrees <= MAX_ELEVATION_ANGLE)
      {
         if (elevation.isRight())
            elevation.add(-radians);  // Moving toward vertical
         else
            elevation.add(radians);   // Moving toward vertical
      }
      // If it would go out of bounds, clamp to the nearest limit
      else if (newDegrees < MIN_ELEVATION_ANGLE)
      {
         elevation.setDegrees(MIN_ELEVATION_ANGLE);
      }
      else if (newDegrees > MAX_ELEVATION_ANGLE)
      {
         elevation.setDegrees(MAX_ELEVATION_ANGLE);
      }
   }
   
   // Quick elevation presets
   void setMaxElevation() { elevation.setDegrees(MAX_ELEVATION_ANGLE); }
   void setMinElevation() { elevation.setDegrees(MIN_ELEVATION_ANGLE); }
   void setHorizontal() { elevation.setDegrees(0.0); }
   
   // Firing information
   int getRoundsFired() const { return roundsFired; }
   double getLastFireTime() const { return lastFireTime; }
   void recordFiring(double currentTime)
   {
      lastFireTime = currentTime;
      roundsFired++;
   }
   
   // Range estimation (simplified ballistic calculation)
   double estimateRange(double targetElevation = 0.0) const;
   
   // Angle estimation for hitting a target at given range
   Angle estimateAngleForRange(double range, double targetElevation = 0.0) const;
   
   // Utility functions
   bool canFire() const { return muzzleVelocity > 0.0; }
   double getBarrelLength() const { return 6.0; } // M777 barrel length in meters
   Position getMuzzlePosition() const;
   Velocity getMuzzleVelocityVector() const;
   
   // Reset howitzer state
   void reset()
   {
      elevation.setDegrees(DEFAULT_ELEVATION_ANGLE);
      muzzleVelocity = DEFAULT_MUZZLE_VELOCITY;
      lastFireTime = -1.0;
      roundsFired = 0;
   }
   
private:
   Position position;      // Position of the howitzer
   double muzzleVelocity;  // Muzzle velocity in m/s
   Angle elevation;        // Elevation angle (0 = up, positive = right)
   double lastFireTime;    // Time of last firing
   int roundsFired;        // Total rounds fired
   
   // Constrain elevation to realistic limits (0° to 85°)
   void constrainElevation()
   {
      double degrees = elevation.getDegrees();
      
      // Simple clamping within the allowed range
      if (degrees < MIN_ELEVATION_ANGLE)
         elevation.setDegrees(MIN_ELEVATION_ANGLE);
      else if (degrees > MAX_ELEVATION_ANGLE)
         elevation.setDegrees(MAX_ELEVATION_ANGLE);
   }
};
