/***********************************************************************
 * Header File:
 *    ANGLE
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Everything we need to know about a direction
 ************************************************************************/

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>   // for M_PI which is 3.14159
#include <iostream>

// Forward declarations for unit tests
class TestAngle;
class TestPosition;
class TestVelocity;
class TestAcceleration;
class TestHowitzer;
class TestProjectile;

/************************************
 * ANGLE
 * Represents an angle in radians with
 * automatic normalization to [0, 2π)
 ************************************/
class Angle
{
public:
   // for the unit tests
   friend TestAcceleration;
   friend TestVelocity;
   friend TestAngle;
   friend TestHowitzer;
   friend TestProjectile;

   // Constructors
   Angle() : radians(0.0) {}
   Angle(const Angle& rhs) : radians(rhs.radians) {}
   Angle(double degrees) : radians(convertToRadians(degrees)) {}

   // Assignment operator
   Angle& operator=(const Angle& rhs);
   Angle& operator=(double degrees);

   // Getters
   double getRadians() const { return radians; }
   double getDegrees() const { return convertToDegrees(radians); }

   //         dx
   //    +-------/
   //    |      /
   // dy |     /
   //    |    / 1.0
   //    | a /
   //    |  /
   //    | /
   // dy = cos a
   // dx = sin a
   double getDx() const { return sin(radians); }
   double getDy() const { return cos(radians); }
   bool   isRight() const { return (radians > 0 && radians < M_PI); }
   bool   isLeft() const { return (radians > M_PI && radians < 2.0 * M_PI); }  // FIXED: proper range

   // Setters
   void setDegrees(double degrees) { radians = convertToRadians(degrees); }
   void setRadians(double radians) { this->radians = normalize(radians); }
   void setUp() { radians = 0.0; }
   void setDown() { radians = M_PI; }
   void setRight() { radians = M_PI_2; }
   void setLeft() { radians = M_PI + M_PI_2; }
   void reverse() { radians = normalize(radians + M_PI); }
   
   // Modifiers
   Angle& add(double delta)
   {
      radians = normalize(radians + delta);
      return *this;
   }

   // Set based on the components
   //         dx
   //     +-------/
   //     |      /
   //  dy |     /
   //     |    /
   //     | a /
   //     |  /
   //     | /
   void setDxDy(double dx, double dy) { radians = normalize(atan2(dx, dy)); }
   
   // Operators
   Angle operator+(double degrees) const;  // FIXED: proper implementation
   Angle operator-(double degrees) const;
   Angle operator+(const Angle& rhs) const;
   Angle operator-(const Angle& rhs) const;
   Angle& operator+=(double degrees);
   Angle& operator-=(double degrees);
   Angle& operator+=(const Angle& rhs);
   Angle& operator-=(const Angle& rhs);
   
   // Comparison operators
   bool operator==(const Angle& rhs) const;
   bool operator!=(const Angle& rhs) const;

   // Utility functions
   double getShortestRotationTo(const Angle& target) const;  // NEW: shortest rotation
   bool isClockwiseTo(const Angle& target) const;           // NEW: direction check
   Angle getOpposite() const { return Angle(getDegrees() + 180.0); }  // NEW: opposite angle

private:
   double normalize(double radians) const;
   double convertToDegrees(double radian) const
   {
      return normalize(radian) * (180.0 / M_PI);
   }
   double convertToRadians(double degrees) const
   {
      return normalize(degrees * (M_PI / 180.0));
   }

   double radians;   // 360 degrees equals 2 PI radians
};

/*******************************************************
 * OUTPUT ANGLE
 * place output on the screen in degrees
 *******************************************************/
inline std::ostream& operator << (std::ostream& out, const Angle& rhs)
{
   out << rhs.getDegrees() << "°";
   return out;
}
