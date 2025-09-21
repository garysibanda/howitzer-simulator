/***********************************************************************
 * Source File:
 *    ANGLE
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about a direction
 ************************************************************************/

#include "angle.h"
#include <cmath>
#include <cassert>
using namespace std;

/************************************
 * ANGLE : NORMALIZE
 * Normalize angle to [0, 2π) range
 ************************************/
double Angle::normalize(double radians) const
{
   // Use fmod to get remainder, then ensure positive
   radians = fmod(radians, 2.0 * M_PI);
   if (radians < 0.0)
      radians += 2.0 * M_PI;
   return radians;
}

/************************************
 * ANGLE : ASSIGNMENT OPERATOR (Angle)
 ************************************/
Angle& Angle::operator=(const Angle& rhs)
{
   if (this != &rhs)
   {
      radians = rhs.radians;
   }
   return *this;
}

/************************************
 * ANGLE : ASSIGNMENT OPERATOR (double)
 ************************************/
Angle& Angle::operator=(double degrees)
{
   setDegrees(degrees);
   return *this;
}

/************************************
 * ANGLE : OPERATOR + (degrees)
 * FIXED: Now properly returns computed angle
 ************************************/
Angle Angle::operator+(double degrees) const
{
   Angle result;
   result.radians = normalize(this->radians + convertToRadians(degrees));
   return result;
}

/************************************
 * ANGLE : OPERATOR - (degrees)
 ************************************/
Angle Angle::operator-(double degrees) const
{
   Angle result;
   result.radians = normalize(this->radians - convertToRadians(degrees));
   return result;
}

/************************************
 * ANGLE : OPERATOR + (Angle)
 ************************************/
Angle Angle::operator+(const Angle& rhs) const
{
   Angle result;
   result.radians = normalize(this->radians + rhs.radians);
   return result;
}

/************************************
 * ANGLE : OPERATOR - (Angle)
 ************************************/
Angle Angle::operator-(const Angle& rhs) const
{
   Angle result;
   result.radians = normalize(this->radians - rhs.radians);
   return result;
}

/************************************
 * ANGLE : OPERATOR += (degrees)
 ************************************/
Angle& Angle::operator+=(double degrees)
{
   radians = normalize(radians + convertToRadians(degrees));
   return *this;
}

/************************************
 * ANGLE : OPERATOR -= (degrees)
 ************************************/
Angle& Angle::operator-=(double degrees)
{
   radians = normalize(radians - convertToRadians(degrees));
   return *this;
}

/************************************
 * ANGLE : OPERATOR += (Angle)
 ************************************/
Angle& Angle::operator+=(const Angle& rhs)
{
   radians = normalize(radians + rhs.radians);
   return *this;
}

/************************************
 * ANGLE : OPERATOR -= (Angle)
 ************************************/
Angle& Angle::operator-=(const Angle& rhs)
{
   radians = normalize(radians - rhs.radians);
   return *this;
}

/************************************
 * ANGLE : OPERATOR ==
 ************************************/
bool Angle::operator==(const Angle& rhs) const
{
   const double epsilon = 1e-6;
   return abs(radians - rhs.radians) < epsilon;
}

/************************************
 * ANGLE : OPERATOR !=
 ************************************/
bool Angle::operator!=(const Angle& rhs) const
{
   return !(*this == rhs);
}

/************************************
 * ANGLE : GET SHORTEST ROTATION TO
 * Find the shortest rotation to reach target angle
 ************************************/
double Angle::getShortestRotationTo(const Angle& target) const
{
   double diff = target.radians - this->radians;
   
   // Normalize to [-π, π]
   while (diff > M_PI)
      diff -= 2.0 * M_PI;
   while (diff < -M_PI)
      diff += 2.0 * M_PI;
   
   return diff;
}

/************************************
 * ANGLE : IS CLOCKWISE TO
 * Check if clockwise rotation is shorter to target
 ************************************/
bool Angle::isClockwiseTo(const Angle& target) const
{
   return getShortestRotationTo(target) > 0;
}
