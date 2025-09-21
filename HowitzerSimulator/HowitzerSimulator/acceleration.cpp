/***********************************************************************
 * Source File:
 *    ACCELERATION
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about changing speed
 ************************************************************************/

#include "acceleration.h"
#include "angle.h"
#include <cmath>
#include <cassert>

/*********************************************
 * ACCELERATION : ASSIGNMENT OPERATOR
 * Assign one acceleration to another
 *********************************************/
Acceleration& Acceleration::operator=(const Acceleration& rhs)
{
   if (this != &rhs)  // Check for self-assignment
   {
      ddx = rhs.ddx;
      ddy = rhs.ddy;
   }
   return *this;
}

/*********************************************
 * ACCELERATION : GET MAGNITUDE
 * Calculate the magnitude of acceleration
 *********************************************/
double Acceleration::getMagnitude() const
{
   return sqrt(ddx * ddx + ddy * ddy);
}

/*********************************************
 * ACCELERATION : ADD
 * Add another acceleration to this one
 *********************************************/
void Acceleration::add(const Acceleration& acceleration)
{
   ddx += acceleration.getDDX();
   ddy += acceleration.getDDY();
}

/*********************************************
 * ACCELERATION : SET
 * Set acceleration from angle and magnitude
 *********************************************/
void Acceleration::set(const Angle& angle, double magnitude)
{
   assert(magnitude >= 0.0);  // Magnitude should not be negative
   ddx = magnitude * sin(angle.getRadians());
   ddy = magnitude * cos(angle.getRadians());
}

/*********************************************
 * ACCELERATION : OPERATOR +
 * Add two accelerations together
 *********************************************/
Acceleration Acceleration::operator+(const Acceleration& rhs) const
{
   return Acceleration(ddx + rhs.ddx, ddy + rhs.ddy);
}

/*********************************************
 * ACCELERATION : OPERATOR -
 * Subtract one acceleration from another
 *********************************************/
Acceleration Acceleration::operator-(const Acceleration& rhs) const
{
   return Acceleration(ddx - rhs.ddx, ddy - rhs.ddy);
}

/*********************************************
 * ACCELERATION : OPERATOR *
 * Multiply acceleration by a scalar
 *********************************************/
Acceleration Acceleration::operator*(double scalar) const
{
   return Acceleration(ddx * scalar, ddy * scalar);
}

/*********************************************
 * ACCELERATION : OPERATOR +=
 * Add another acceleration to this one
 *********************************************/
Acceleration& Acceleration::operator+=(const Acceleration& rhs)
{
   ddx += rhs.ddx;
   ddy += rhs.ddy;
   return *this;
}

/*********************************************
 * ACCELERATION : OPERATOR -=
 * Subtract another acceleration from this one
 *********************************************/
Acceleration& Acceleration::operator-=(const Acceleration& rhs)
{
   ddx -= rhs.ddx;
   ddy -= rhs.ddy;
   return *this;
}

/*********************************************
 * ACCELERATION : OPERATOR *=
 * Multiply this acceleration by a scalar
 *********************************************/
Acceleration& Acceleration::operator*=(double scalar)
{
   ddx *= scalar;
   ddy *= scalar;
   return *this;
}

/*********************************************
 * ACCELERATION : IS ZERO
 * Check if acceleration is effectively zero
 *********************************************/
bool Acceleration::isZero() const
{
   const double epsilon = 1e-10;
   return (abs(ddx) < epsilon && abs(ddy) < epsilon);
}
