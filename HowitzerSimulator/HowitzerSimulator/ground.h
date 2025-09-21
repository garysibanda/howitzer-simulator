/***********************************************************************
 * Header File:
 *    GROUND
 * Author:
 *    Br. Helfrich and Gary Sibanda
 * Summary:
 *    Represents the ground in the artillery simulation
 ************************************************************************/

#pragma once

#include "position.h"   // for Point
#include "uiDraw.h"
#include <memory>       // for smart pointers

// forward declaration for the Ground unit tests
class TestGround;

 /***********************************************************
  * GROUND
  * The ground class with proper memory management
  ***********************************************************/
class Ground
{
   // unit test access
   friend ::TestGround;

public:
   // the constructor generates the ground
   Ground(const Position &posUpperRight);
   Ground() : ground(nullptr), iHowitzer(0), iTarget(0) {}
   
   // FIXED: Added destructor for proper memory management
   ~Ground() { cleanup(); }
   
   // FIXED: Added copy constructor
   Ground(const Ground& other);
   
   // FIXED: Added assignment operator
   Ground& operator=(const Ground& other);
   
   // reset the game
   void reset(Position & posHowitzer);

   // draw the ground on the screen
   void draw(ogstream & gout) const;

   // determine how high the Point is off the ground
   double getElevationMeters(const Position & pos) const;

   // where the the target located?
   Position getTarget() const;
   
   // Utility functions
   int getWidth() const { return (int)posUpperRight.getPixelsX(); }
   double getGroundHeight(int pixelX) const;
   bool isValidPosition(const Position& pos) const;

private:
   double * ground;               // elevation of the ground, in pixels
   int iTarget;                   // the location of the target, in pixels
   int iHowitzer;                 // the location of the howitzer
   Position posUpperRight;        // size of the screen
   
   // Helper functions
   void cleanup();                // Clean up allocated memory
   void allocateGround();         // Allocate ground array
   void copyFrom(const Ground& other);  // Copy helper
};
