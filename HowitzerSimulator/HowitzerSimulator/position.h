/***********************************************************************
 * Header File:
 *    POSITION
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about a location on the screen.
 ************************************************************************/

#pragma once

#include <iostream>
#include <cmath>

// Forward declarations
class TestPosition;
class Acceleration;
class Velocity;
class TestGround;
class TestHowitzer;
class TestProjectile;
class Angle;

/*********************************************
 * Position
 * A single position on the field in Meters
 * with automatic conversion to/from pixels
 *********************************************/
class Position
{
public:
   friend ::TestPosition;
   friend ::TestGround;
   friend ::TestHowitzer;
   friend ::TestProjectile;

   // Constructors
   Position() : x(0.0), y(0.0) {}
   Position(double x, double y);
   Position(const Position& pt) : x(pt.x), y(pt.y) {}
   
   // Assignment operators
   Position& operator=(const Position& pt);

   // Getters - Meters
   double getMetersX() const { return x; }
   double getMetersY() const { return y; }
   
   // Getters - Pixels
   double getPixelsX() const { return x / metersFromPixels; }
   double getPixelsY() const { return y / metersFromPixels; }
   
   // Getters - Utility
   double getZoom() const { return metersFromPixels; }
   double getDistanceTo(const Position& other) const;  // NEW: distance calculation
   // Angle getAngleTo(const Position& other) const;      // NEW: angle to another position (commented out to avoid circular dependency)

   // Setters - Zoom
   void setZoom(double z) { metersFromPixels = z; }
   
   // Setters - Meters
   void setMeters(double xMeters, double yMeters)
   {
      x = xMeters;
      y = yMeters;
   }
   void setMetersX(double xMeters) { x = xMeters; }
   void setMetersY(double yMeters) { y = yMeters; }
   
   // Setters - Pixels
   void setPixelsX(double xPixels) { x = xPixels * metersFromPixels; }
   void setPixelsY(double yPixels) { y = yPixels * metersFromPixels; }
   
   // Mutators - Meters
   void addMetersX(double x) { this->x += x; }
   void addMetersY(double y) { this->y += y; }
   void addMeters(double x, double y) { this->x += x; this->y += y; }
   
   // Mutators - Pixels
   void addPixelsX(double x) { this->x += x * metersFromPixels; }
   void addPixelsY(double y) { this->y += y * metersFromPixels; }
   void addPixels(double x, double y)
   {
      this->x += x * metersFromPixels;
      this->y += y * metersFromPixels;
   }
   
   // Physics update
   void add(const Acceleration& a, const Velocity& v, double t);
   
   // Utility
   void reverse() { x = -x; y = -y; }
   void reset() { x = 0.0; y = 0.0; }  // NEW: reset to origin
   bool isOrigin() const;              // NEW: check if at origin
   
   // Operators
   Position operator+(const Position& rhs) const;
   Position operator-(const Position& rhs) const;
   Position operator*(double scalar) const;
   Position& operator+=(const Position& rhs);
   Position& operator-=(const Position& rhs);
   Position& operator*=(double scalar);
   bool operator==(const Position& rhs) const;
   bool operator!=(const Position& rhs) const;

private:
   double x;                           // horizontal position in meters
   double y;                           // vertical position in meters
   static double metersFromPixels;     // conversion factor
};

// Stream I/O useful for debugging
std::ostream& operator<<(std::ostream& out, const Position& pt);
std::istream& operator>>(std::istream& in, Position& pt);

/*********************************************
 * PT
 * Trivial point structure for simple operations
 *********************************************/
struct PT
{
   double x;
   double y;
   
   PT() : x(0.0), y(0.0) {}
   PT(double x, double y) : x(x), y(y) {}
};
