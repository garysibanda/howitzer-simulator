/***********************************************************************
 * Header File:
 *    VELOCITY
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about speed
 ************************************************************************/

#pragma once

// Forward declarations for unit tests
class TestPosition;
class TestVelocity;
class TestAcceleration;
class TestProjectile;

// Forward declarations for dependencies
class Acceleration;
class Angle;

/*********************************************
 * Velocity
 * Represents velocity in 2D space with
 * horizontal (dx) and vertical (dy) components
 *********************************************/
class Velocity
{
   // for unit tests
   friend TestPosition;
   friend TestVelocity;
   friend TestProjectile;
   
public:
   // Constructors
   Velocity() : dx(0.0), dy(0.0) {}
   Velocity(double dx, double dy) : dx(dx), dy(dy) {}
   Velocity(const Velocity& other) : dx(other.dx), dy(other.dy) {}

   // Assignment operator
   Velocity& operator=(const Velocity& rhs);

   // Getters
   double getDX() const { return dx; }
   double getDY() const { return dy; }
   double getSpeed() const;
   Angle  getAngle() const;
   
   // Setters
   void set(const Angle& angle, double magnitude);
   void setDX(double dx) { this->dx = dx; }
   void setDY(double dy) { this->dy = dy; }
   void set(double dx, double dy) { this->dx = dx; this->dy = dy; }
   
   // Mutators
   void addDX(double dx) { this->dx += dx; }
   void addDY(double dy) { this->dy += dy; }
   void add(const Acceleration& acceleration, double time);
   void add(const Velocity& rhs);
   void reverse();
   void scale(double factor) { dx *= factor; dy *= factor; }  // NEW: scale velocity
   void reset() { dx = 0.0; dy = 0.0; }  // NEW: reset to zero

   // Operators
   Velocity operator+(const Velocity& rhs) const;
   Velocity operator-(const Velocity& rhs) const;
   Velocity operator*(double scalar) const;
   Velocity& operator+=(const Velocity& rhs);
   Velocity& operator-=(const Velocity& rhs);
   Velocity& operator*=(double scalar);
   bool operator==(const Velocity& rhs) const;
   bool operator!=(const Velocity& rhs) const;

   // Utility
   bool isZero() const;  // NEW: check if velocity is zero
   double getKineticEnergy(double mass) const;  // NEW: calculate kinetic energy

private:
   double dx;           // horizontal velocity (m/s)
   double dy;           // vertical velocity (m/s)
};
