/***********************************************************************
 * Source File:
 *    POSITION
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about a location on the screen.
 ************************************************************************/

#include "position.h"
#include "velocity.h"
#include "acceleration.h"
#include "angle.h"
#include <cassert>
#include <cmath>

// Note: metersFromPixels is initialized in main.cpp

/*******************************************
 * POSITION : NON-DEFAULT CONSTRUCTOR
 *****************************************/
Position::Position(double x, double y) : x(x), y(y)
{
   // All initialization done in member initializer list
}

/******************************************
 * POSITION : ASSIGNMENT OPERATOR
 * Assign a position. Check for self-assignment
 * and copy data members.
 *****************************************/
Position& Position::operator=(const Position& posRHS)
{
   // Check for self-assignment
   if (this == &posRHS)
      return *this;
   
   // Copy data members from posRHS to this object
   this->x = posRHS.x;
   this->y = posRHS.y;
   
   // Return *this to allow chaining
   return *this;
}

/******************************************
 * POSITION : GET DISTANCE TO
 * Calculate Euclidean distance to another position
 *****************************************/
double Position::getDistanceTo(const Position& other) const
{
   double dx = other.x - this->x;
   double dy = other.y - this->y;
   return sqrt(dx * dx + dy * dy);
}

/******************************************
 * POSITION : IS ORIGIN
 * Check if position is at origin (within epsilon)
 *****************************************/
bool Position::isOrigin() const
{
   const double epsilon = 1e-10;
   return (abs(x) < epsilon && abs(y) < epsilon);
}

/************************************************************************
 * POSITION : ADD
 * Update the current position based on velocity and acceleration
 * using kinematic equation:
 *
 *   s = s_0 + vt + (1/2)at²
 *
 *   INPUT  ACCELERATION The acceleration of the item
 *          VELOCITY     The velocity of the item
 *          TIME         How much time are we talking about?
 *   OUTPUT POSITION     The location, updated
 *
 *  x = x + dx*t + (1/2)*ddx*t²
 *  y = y + dy*t + (1/2)*ddy*t²
 *************************************************************************/
void Position::add(const Acceleration& a, const Velocity& v, double t)
{
   assert(t >= 0.0);  // Time should not be negative
   
   x += v.getDX() * t + 0.5 * a.getDDX() * t * t;
   y += v.getDY() * t + 0.5 * a.getDDY() * t * t;
}

/******************************************
 * POSITION : OPERATOR +
 * Add two positions together
 *****************************************/
Position Position::operator+(const Position& rhs) const
{
   return Position(x + rhs.x, y + rhs.y);
}

/******************************************
 * POSITION : OPERATOR -
 * Subtract one position from another
 *****************************************/
Position Position::operator-(const Position& rhs) const
{
   return Position(x - rhs.x, y - rhs.y);
}

/******************************************
 * POSITION : OPERATOR *
 * Multiply position by a scalar
 *****************************************/
Position Position::operator*(double scalar) const
{
   return Position(x * scalar, y * scalar);
}

/******************************************
 * POSITION : OPERATOR +=
 * Add another position to this one
 *****************************************/
Position& Position::operator+=(const Position& rhs)
{
   x += rhs.x;
   y += rhs.y;
   return *this;
}

/******************************************
 * POSITION : OPERATOR -=
 * Subtract another position from this one
 *****************************************/
Position& Position::operator-=(const Position& rhs)
{
   x -= rhs.x;
   y -= rhs.y;
   return *this;
}

/******************************************
 * POSITION : OPERATOR *=
 * Multiply this position by a scalar
 *****************************************/
Position& Position::operator*=(double scalar)
{
   x *= scalar;
   y *= scalar;
   return *this;
}

/******************************************
 * POSITION : OPERATOR ==
 * Check if two positions are equal (within epsilon)
 *****************************************/
bool Position::operator==(const Position& rhs) const
{
   const double epsilon = 1e-10;
   return (abs(x - rhs.x) < epsilon && abs(y - rhs.y) < epsilon);
}

/******************************************
 * POSITION : OPERATOR !=
 * Check if two positions are not equal
 *****************************************/
bool Position::operator!=(const Position& rhs) const
{
   return !(*this == rhs);
}

/******************************************
 * POSITION insertion
 * Display coordinates on the screen
 *****************************************/
std::ostream& operator<<(std::ostream& out, const Position& pt)
{
   out << "(" << pt.getMetersX() << "m, " << pt.getMetersY() << "m)";
   return out;
}
   
/*******************************************
 * POSITION extraction
 * Prompt for coordinates
 ******************************************/
std::istream& operator>>(std::istream& in, Position& pt)
{
   double x, y;
   in >> x >> y;

   pt.setMetersX(x);
   pt.setMetersY(y);

   return in;
}
