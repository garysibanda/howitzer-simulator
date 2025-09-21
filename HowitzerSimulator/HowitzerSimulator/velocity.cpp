/***********************************************************************
 * Source File:
 *    VELOCITY
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about speed
 ************************************************************************/

#include "velocity.h"
#include "acceleration.h"
#include "angle.h"
#include <cmath>
#include <cassert>

/*********************************************
 * VELOCITY : ASSIGNMENT OPERATOR
 * Assign one velocity to another
 *********************************************/
Velocity& Velocity::operator=(const Velocity& rhs)
{
   if (this != &rhs)  // Check for self-assignment
   {
      dx = rhs.dx;
      dy = rhs.dy;
   }
   return *this;
}

/*********************************************
 * VELOCITY : ADD
 * Update velocity using acceleration over time
 *  v = v_0 + a*t
 *********************************************/
void Velocity::add(const Acceleration& acceleration, double time)
{
   assert(time >= 0.0);  // Time should not be negative
   dx += acceleration.getDDX() * time;
   dy += acceleration.getDDY() * time;
}

/*********************************************
 * VELOCITY : GET SPEED
 * Find the magnitude of velocity
 *        dx
 *     +-------/
 *     |      /
 *  dy |     /
 *     |    /speed or magnitude
 *     | a /
 *     |  /
 *     | /
 *            _____________
 *  speed = √(dx² + dy²)
 *********************************************/
double Velocity::getSpeed() const
{
   return sqrt((dx * dx) + (dy * dy));
}

/*********************************************
 * VELOCITY : SET
 * Set velocity from angle and magnitude
 *        dx
 *     +-------/
 *     |      /
 *  dy |     /
 *     |    /speed or magnitude
 *     | a /
 *     |  /
 *     | /
 * dy = speed * cos(a)
 * dx = speed * sin(a)
 *********************************************/
void Velocity::set(const Angle& angle, double magnitude)
{
   assert(magnitude >= 0.0);  // Magnitude should not be negative
   dx = magnitude * sin(angle.getRadians());
   dy = magnitude * cos(angle.getRadians());
}

/************************************************
 * VELOCITY : GET ANGLE
 * Determine the direction of velocity
 * where 0 is up. This returns angle in radians
 *        dx
 *     +-------/
 *     |      /
 *  dy |     /
 *     |    / speed
 *     | a /
 *     |  /
 *     | /
 *
 *  a = atan2(dx, dy)
 ************************************************/
Angle Velocity::getAngle() const
{
   Angle angle;
   angle.setDxDy(dx, dy);
   return angle;
}

/*********************************************
 * VELOCITY : ADD (Velocity)
 * Add the velocity components of rhs to current velocity
 *********************************************/
void Velocity::add(const Velocity& rhs)
{
   this->dx += rhs.dx;
   this->dy += rhs.dy;
}

/*********************************************
 * VELOCITY : REVERSE
 * Reverse the direction of the velocity
 *********************************************/
void Velocity::reverse()
{
   this->dx = -this->dx;
   this->dy = -this->dy;
}

/*********************************************
 * VELOCITY : IS ZERO
 * Check if velocity is effectively zero
 *********************************************/
bool Velocity::isZero() const
{
   const double epsilon = 1e-10;
   return (abs(dx) < epsilon && abs(dy) < epsilon);
}

/*********************************************
 * VELOCITY : GET KINETIC ENERGY
 * Calculate kinetic energy: KE = (1/2)mv²
 *********************************************/
double Velocity::getKineticEnergy(double mass) const
{
   assert(mass > 0.0);  // Mass should be positive
   return 0.5 * mass * getSpeed() * getSpeed();
}

/*********************************************
 * VELOCITY : OPERATOR +
 * Add two velocities together
 *********************************************/
Velocity Velocity::operator+(const Velocity& rhs) const
{
   return Velocity(dx + rhs.dx, dy + rhs.dy);
}

/*********************************************
 * VELOCITY : OPERATOR -
 * Subtract one velocity from another
 *********************************************/
Velocity Velocity::operator-(const Velocity& rhs) const
{
   return Velocity(dx - rhs.dx, dy - rhs.dy);
}

/*********************************************
 * VELOCITY : OPERATOR *
 * Multiply velocity by a scalar
 *********************************************/
Velocity Velocity::operator*(double scalar) const
{
   return Velocity(dx * scalar, dy * scalar);
}

/*********************************************
 * VELOCITY : OPERATOR +=
 * Add another velocity to this one
 *********************************************/
Velocity& Velocity::operator+=(const Velocity& rhs)
{
   dx += rhs.dx;
   dy += rhs.dy;
   return *this;
}

/*********************************************
 * VELOCITY : OPERATOR -=
 * Subtract another velocity from this one
 *********************************************/
Velocity& Velocity::operator-=(const Velocity& rhs)
{
   dx -= rhs.dx;
   dy -= rhs.dy;
   return *this;
}

/*********************************************
 * VELOCITY : OPERATOR *=
 * Multiply this velocity by a scalar
 *********************************************/
Velocity& Velocity::operator*=(double scalar)
{
   dx *= scalar;
   dy *= scalar;
   return *this;
}

/*********************************************
 * VELOCITY : OPERATOR ==
 * Check if two velocities are equal (within epsilon)
 *********************************************/
bool Velocity::operator==(const Velocity& rhs) const
{
   const double epsilon = 1e-10;
   return (abs(dx - rhs.dx) < epsilon && abs(dy - rhs.dy) < epsilon);
}

/*********************************************
 * VELOCITY : OPERATOR !=
 * Check if two velocities are not equal
 *********************************************/
bool Velocity::operator!=(const Velocity& rhs) const
{
   return !(*this == rhs);
}
