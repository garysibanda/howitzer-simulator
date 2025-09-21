/***********************************************************************
 * Header File:
 *    ACCELERATION
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about acceleration
 ************************************************************************/

#pragma once

// Forward declarations
class TestAcceleration;
class TestVelocity;
class TestPosition;
class Angle;

/*********************************************
 * Acceleration
 * Represents acceleration in 2D space with
 * horizontal (ddx) and vertical (ddy) components
 *********************************************/
class Acceleration
{
   friend TestPosition;
   friend TestVelocity;
   friend TestAcceleration;

public:
   // Constructors
   Acceleration() : ddx(0.0), ddy(0.0) {}
   Acceleration(double ddx, double ddy) : ddx(ddx), ddy(ddy) {}  // FIXED: proper initialization
   Acceleration(const Acceleration& other) : ddx(other.ddx), ddy(other.ddy) {}

   // Assignment operator
   Acceleration& operator=(const Acceleration& rhs);

   // Getters
   double getDDX() const { return ddx; }
   double getDDY() const { return ddy; }
   double getMagnitude() const;  // NEW: get magnitude of acceleration

   // Setters
   void setDDX(double ddx) { this->ddx = ddx; }
   void setDDY(double ddy) { this->ddy = ddy; }
   void set(double ddx, double ddy) { this->ddx = ddx; this->ddy = ddy; }
   void set(const Angle& angle, double magnitude);
   
   // Mutators
   void addDDX(double ddx) { this->ddx += ddx; }
   void addDDY(double ddy) { this->ddy += ddy; }
   void add(const Acceleration& rhs);
   void scale(double factor) { ddx *= factor; ddy *= factor; }  // NEW: scale acceleration
   void reset() { ddx = 0.0; ddy = 0.0; }  // NEW: reset to zero

   // Operators
   Acceleration operator+(const Acceleration& rhs) const;
   Acceleration operator-(const Acceleration& rhs) const;
   Acceleration operator*(double scalar) const;
   Acceleration& operator+=(const Acceleration& rhs);
   Acceleration& operator-=(const Acceleration& rhs);
   Acceleration& operator*=(double scalar);

   // Utility
   bool isZero() const;  // NEW: check if acceleration is zero

private:
   double ddx;     // horizontal acceleration (m/s²)
   double ddy;     // vertical acceleration (m/s²)
};
