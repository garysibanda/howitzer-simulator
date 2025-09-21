/**********************************************************************
 * Header File:
 *    SIMULATION
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Execute one simulation of a projectile being fired.
 ************************************************************************/

#pragma once

#include "position.h"
#include "ground.h"
#include "howitzer.h"
#include "projectile.h"
#include "uiDraw.h"
#include "uiInteract.h"
#include <array>
#include <iomanip>

// Simulation constants
#define TRAIL_LENGTH 20
#define HIT_TOLERANCE 175.0  // meters
#define TIME_STEP 0.5        // seconds

/*********************************************
 * Simulator
 * Manages the complete artillery simulation
 * including physics, rendering, and user input
 *********************************************/
class Simulator
{
public:
   // Constructor
   Simulator(const Position& posUpperRight);
   
   // Main simulation loop functions
   void update(double timeStep);
   void handleInput(const Interface* pUI);
   void draw(ogstream& gout) const;
   
   // Getters
   Position getPosUpperRight() const { return posUpperRight; }
   double getSimulationTime() const { return time; }
   bool isProjectileFlying() const { return isFiring; }
   bool hasHitTarget() const { return isHit; }
   
   // Game state management
   void reset();
   void newGame();
   
   // Statistics
   int getScore() const { return score; }
   int getShotsAttempted() const { return shotsAttempted; }
   double getHitRate() const
   {
      return (shotsAttempted > 0) ? (double)score / shotsAttempted : 0.0;
   }
   
private:
   // Core simulation objects
   Ground ground;
   Howitzer howitzer;
   Projectile projectile;
   Position posUpperRight;
   
   // Projectile trail for visual effects
   std::array<Position, TRAIL_LENGTH> projectilePath;
   
   // Simulation state
   double time;              // Current simulation time
   bool isFiring;           // Is projectile currently in flight
   bool isHit;              // Did last shot hit the target
   
   // Game statistics
   int score;               // Number of hits
   int shotsAttempted;      // Total shots fired
   
   // Input handling
   void processMovementInput(const Interface* pUI);
   void processFireInput(const Interface* pUI);
   
   // Collision detection
   bool checkGroundCollision();
   bool checkTargetHit();
   
   // Trail management
   void updateProjectileTrail();
   void clearProjectileTrail();
   
   // Utility functions
   double calculateDistance(const Position& pos1, const Position& pos2) const;
   void displayGameStats(ogstream& gout) const;
   void displayHitStatus(ogstream& gout) const;
};
