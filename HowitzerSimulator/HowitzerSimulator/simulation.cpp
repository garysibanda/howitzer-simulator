/**********************************************************************
 * Source File:
 *    SIMULATION
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Execute one simulation of a projectile being fired.
 ************************************************************************/

#include "simulation.h"
#include <iomanip>
#include <cmath>
#include <cassert>

/*********************************************
 * SIMULATOR : CONSTRUCTOR
 * Initialize the simulation with given screen bounds
 *********************************************/
Simulator::Simulator(const Position& posUpperRight)
   : ground(posUpperRight), howitzer(), posUpperRight(posUpperRight),
     time(0.0), isFiring(false), isHit(false),
     score(0), shotsAttempted(0)
{
   // Set horizontal position of the howitzer to center
   Position howitzerPos;
   howitzerPos.setPixelsX(posUpperRight.getPixelsX() / 2.0);
   
   // Generate ground and set howitzer vertical position
   ground.reset(howitzerPos);
   howitzer.setPosition(howitzerPos);
   
   // Initialize projectile trail
   clearProjectileTrail();
}

/*********************************************
 * SIMULATOR : UPDATE
 * Update the simulation state for one time step
 *********************************************/
void Simulator::update(double timeStep)
{
   assert(timeStep > 0.0);
   
   if (isFiring)
   {
      time += timeStep;
      projectile.advance(time);
      
      // Update projectile trail
      updateProjectileTrail();
      
      // Check for ground collision
      if (checkGroundCollision())
      {
         isFiring = false;
         
         // Check if target was hit
         if (checkTargetHit())
         {
            isHit = true;
            score++;
            // Generate new terrain for next round
            Position newHowitzerPos = howitzer.getPosition();
            ground.reset(newHowitzerPos);
            howitzer.setPosition(newHowitzerPos);
         }
         else
         {
            isHit = false;
         }
         
         // Reset projectile for next shot
         projectile.reset();
         clearProjectileTrail();
      }
   }
}

/*********************************************
 * SIMULATOR : HANDLE INPUT
 * Process user input for controlling the simulation
 *********************************************/
void Simulator::handleInput(const Interface* pUI)
{
   assert(pUI != nullptr);
   
   // Process movement input
   processMovementInput(pUI);
   
   // Process firing input
   processFireInput(pUI);
}

/*********************************************
 * SIMULATOR : PROCESS MOVEMENT INPUT
 * Handle howitzer movement controls with angle limits (0° to 90°)
 *********************************************/
void Simulator::processMovementInput(const Interface* pUI)
{
   // Get current angle in degrees for limit checking
   double currentAngle = howitzer.getElevation().getDegrees();
   
   // Rotation controls with limits (0° to 90° range only)
   if (pUI->isRight() && currentAngle < MAX_ELEVATION_ANGLE)
      howitzer.rotate(0.05);
   if (pUI->isLeft() && currentAngle > 0.0)  // Changed from MIN_ELEVATION_ANGLE to 0.0
      howitzer.rotate(-0.05);
   
   // Elevation controls (kept for backward compatibility but with limits)
   if (pUI->isUp() && currentAngle < MAX_ELEVATION_ANGLE)
      howitzer.raise(0.003);
   if (pUI->isDown() && currentAngle > 0.0)  // Changed from MIN_ELEVATION_ANGLE to 0.0
      howitzer.raise(-0.003);
}

/*********************************************
 * SIMULATOR : PROCESS FIRE INPUT
 * Handle firing controls
 *********************************************/
void Simulator::processFireInput(const Interface* pUI)
{
   // Fire projectile when space is pressed and not currently firing
   if (pUI->isSpace() && !isFiring && howitzer.canFire())
   {
      time = 0.0;
      shotsAttempted++;
      
      // Fire projectile from howitzer position
      projectile.fire(howitzer.getPosition(),
                     howitzer.getElevation(),
                     howitzer.getMuzzleVelocity(),
                     time);
      
      // Record firing
      howitzer.recordFiring(time);
      isFiring = true;
      isHit = false;
   }
}

/*********************************************
 * SIMULATOR : CHECK GROUND COLLISION
 * Check if projectile has hit the ground
 *********************************************/
bool Simulator::checkGroundCollision()
{
   Position projectilePos = projectile.getPosition();
   double groundElevation = ground.getElevationMeters(projectilePos);
   
   return (projectilePos.getMetersY() <= groundElevation);
}

/*********************************************
 * SIMULATOR : CHECK TARGET HIT
 * Check if projectile hit close enough to target
 *********************************************/
bool Simulator::checkTargetHit()
{
   Position projectilePos = projectile.getPosition();
   Position targetPos = ground.getTarget();
   
   double distance = calculateDistance(projectilePos, targetPos);
   return (distance < HIT_TOLERANCE);
}

/*********************************************
 * SIMULATOR : UPDATE PROJECTILE TRAIL
 * Update the visual trail behind the projectile
 *********************************************/
void Simulator::updateProjectileTrail()
{
   // Shift old positions down the array
   for (int i = TRAIL_LENGTH - 1; i > 0; i--)
   {
      projectilePath[i] = projectilePath[i - 1];
   }
   
   // Store current projectile position at the beginning
   projectilePath[0] = projectile.getPosition();
}

/*********************************************
 * SIMULATOR : CLEAR PROJECTILE TRAIL
 * Clear the projectile trail array
 *********************************************/
void Simulator::clearProjectileTrail()
{
   for (int i = 0; i < TRAIL_LENGTH; i++)
   {
      projectilePath[i] = Position();
   }
}

/*********************************************
 * SIMULATOR : CALCULATE DISTANCE
 * Calculate distance between two positions
 *********************************************/
double Simulator::calculateDistance(const Position& pos1, const Position& pos2) const
{
   double dx = pos1.getMetersX() - pos2.getMetersX();
   double dy = pos1.getMetersY() - pos2.getMetersY();
   return sqrt(dx * dx + dy * dy);
}

/*********************************************
 * SIMULATOR : DRAW
 * Render the entire simulation
 *********************************************/
void Simulator::draw(ogstream& gout) const
{
   // Draw ground and target
   ground.draw(gout);
   
   // Draw howitzer
   howitzer.draw(gout, time);
   
   // Draw projectile trail
   for (int i = 0; i < TRAIL_LENGTH; ++i)
   {
      if (projectilePath[i].getMetersX() != 0.0 || projectilePath[i].getMetersY() != 0.0)
      {
         gout.drawProjectile(projectilePath[i], i * 0.5);
      }
   }
   
   // Display game information
   displayGameStats(gout);
   displayHitStatus(gout);
}

/*********************************************
 * SIMULATOR : DISPLAY GAME STATS
 * Display current game statistics with ANGLE instead of ELEVATION
 *********************************************/
void Simulator::displayGameStats(ogstream& gout) const
{
   // Set up formatting
   gout.setf(std::ios::fixed | std::ios::showpoint);
   gout.precision(1);
   
   // Display flight time
   gout << std::setw(85) << std::setfill(' ') << ""
        << "Flight time: " << time << "s\n";
   
   // Display howitzer angle (CHANGED FROM "Elevation" TO "Angle")
   gout << std::setw(95) << std::setfill(' ') << ""
        << "Angle: " << howitzer.getElevation().getDegrees() << "°\n";
   
   // Display score and accuracy
   gout << std::setw(85) << std::setfill(' ') << ""
        << "Score: " << score << "/" << shotsAttempted;
   
   if (shotsAttempted > 0)
   {
      gout << " (" << std::setprecision(0) << (getHitRate() * 100.0) << "%)\n";
   }
   else
   {
      gout << "\n";
   }
}

/*********************************************
 * SIMULATOR : DISPLAY HIT STATUS
 * Display hit/miss status
 *********************************************/
void Simulator::displayHitStatus(ogstream& gout) const
{
   if (!isFiring)
   {
      gout << std::setw(95) << std::setfill(' ') << "";
      
      if (shotsAttempted > 0)
      {
         if (isHit)
            gout << "Target: HIT!";
         else
            gout << "Target: Miss";
      }
      else
      {
         gout << "Press SPACE to fire";
      }
   }
   else
   {
      gout << std::setw(100) << std::setfill(' ') << ""
           << "Projectile in flight...";
   }
}

/*********************************************
 * SIMULATOR : RESET
 * Reset the current simulation state
 *********************************************/
void Simulator::reset()
{
   time = 0.0;
   isFiring = false;
   isHit = false;
   
   projectile.reset();
   clearProjectileTrail();
   
   // Reset ground and howitzer position
   Position howitzerPos = howitzer.getPosition();
   ground.reset(howitzerPos);
   howitzer.setPosition(howitzerPos);
}

/*********************************************
 * SIMULATOR : NEW GAME
 * Start a completely new game
 *********************************************/
void Simulator::newGame()
{
   score = 0;
   shotsAttempted = 0;
   
   // Generate new howitzer position
   howitzer.generatePosition(posUpperRight);
   howitzer.reset();
   
   reset();
}
