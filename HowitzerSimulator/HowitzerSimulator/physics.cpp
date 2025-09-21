/***********************************************************************
 * Source File:
 *    PHYSICS
 * Author:
 *    Gary Sibanda
 * Summary:
 *    Laws of motion, effects of gravity, wind resistance, etc.
 ************************************************************************/

#include "physics.h"
#include <algorithm>

/*********************************************************
 * LINEAR INTERPOLATION WITH MAPPING
 * From a list of domains and ranges, linear interpolate
 *********************************************************/
double linearInterpolation(const Mapping mapping[], int numMapping, double domain)
{
   assert(mapping != nullptr);
   assert(numMapping > 0);
   
   // Edge cases: domain is outside the given mapping range
   if (domain <= mapping[0].domain)
      return mapping[0].range;
   if (domain >= mapping[numMapping - 1].domain)
      return mapping[numMapping - 1].range;
   
   // Binary search for the interval containing 'domain'
   int left = 0;
   int right = numMapping - 1;
   
   while (left < right - 1)
   {
      int mid = left + (right - left) / 2;
      if (mapping[mid].domain <= domain)
         left = mid;
      else
         right = mid;
   }
   
   // Linear interpolation between the two found points
   return linearInterpolation(mapping[left].domain,
                              mapping[left].range,
                              mapping[right].domain,
                              mapping[right].range,
                              domain);
}

/*********************************************************
 * GRAVITY FROM ALTITUDE
 * Determine gravity coefficient based on the altitude
 * Uses realistic atmospheric model data
 *********************************************************/
double gravityFromAltitude(double altitude)
{
   // Standard gravity values at different altitudes
   static const Mapping gravityMapping[] =
   {
      {0.0,     9.807},     // Sea level
      {1000.0,  9.804},     // 1,000 meters
      {2000.0,  9.801},     // 2,000 meters
      {3000.0,  9.797},     // 3,000 meters
      {4000.0,  9.794},     // 4,000 meters
      {5000.0,  9.791},     // 5,000 meters
      {6000.0,  9.788},     // 6,000 meters
      {7000.0,  9.785},     // 7,000 meters
      {8000.0,  9.782},     // 8,000 meters
      {9000.0,  9.779},     // 9,000 meters
      {10000.0, 9.776},     // 10,000 meters
      {15000.0, 9.761},     // 15,000 meters
      {20000.0, 9.745},     // 20,000 meters
      {25000.0, 9.730},     // 25,000 meters
      {30000.0, 9.715},     // 30,000 meters
      {40000.0, 9.684},     // 40,000 meters
      {50000.0, 9.654},     // 50,000 meters
      {60000.0, 9.624},     // 60,000 meters
      {70000.0, 9.594},     // 70,000 meters
      {80000.0, 9.564}      // 80,000 meters
   };
   
   constexpr int numMapping = sizeof(gravityMapping) / sizeof(gravityMapping[0]);
   return linearInterpolation(gravityMapping, numMapping, altitude);
}

/*********************************************************
 * DENSITY FROM ALTITUDE
 * Determine the density of air based on the altitude
 * Uses standard atmospheric density model
 *********************************************************/
double densityFromAltitude(double altitude)
{
   // Standard air density values at different altitudes (kg/m³)
   static const Mapping densityMapping[] =
   {
      {0.0,     1.225},      // Sea level
      {1000.0,  1.112},      // 1,000 meters
      {2000.0,  1.007},      // 2,000 meters
      {3000.0,  0.9093},     // 3,000 meters
      {4000.0,  0.8194},     // 4,000 meters
      {5000.0,  0.7364},     // 5,000 meters
      {6000.0,  0.6601},     // 6,000 meters
      {7000.0,  0.5900},     // 7,000 meters
      {8000.0,  0.5258},     // 8,000 meters
      {9000.0,  0.4671},     // 9,000 meters
      {10000.0, 0.4135},     // 10,000 meters
      {15000.0, 0.1948},     // 15,000 meters
      {20000.0, 0.08891},    // 20,000 meters
      {25000.0, 0.04008},    // 25,000 meters
      {30000.0, 0.01841},    // 30,000 meters
      {40000.0, 0.003996},   // 40,000 meters
      {50000.0, 0.001027},   // 50,000 meters
      {60000.0, 0.0003097},  // 60,000 meters
      {70000.0, 0.0000828},  // 70,000 meters
      {80000.0, 0.0000185}   // 80,000 meters
   };
   
   constexpr int numMapping = sizeof(densityMapping) / sizeof(densityMapping[0]);
   return linearInterpolation(densityMapping, numMapping, altitude);
}

/*********************************************************
 * SPEED OF SOUND FROM ALTITUDE
 * Determine the speed of sound for a given altitude
 * Accounts for temperature variation with altitude
 *********************************************************/
double speedSoundFromAltitude(double altitude)
{
   // Speed of sound values at different altitudes (m/s)
   static const Mapping speedSoundMapping[] =
   {
      {0.0,     340.0},     // Sea level
      {1000.0,  336.0},     // 1,000 meters
      {2000.0,  332.0},     // 2,000 meters
      {3000.0,  328.0},     // 3,000 meters
      {4000.0,  324.0},     // 4,000 meters
      {5000.0,  320.0},     // 5,000 meters
      {6000.0,  316.0},     // 6,000 meters
      {7000.0,  312.0},     // 7,000 meters
      {8000.0,  308.0},     // 8,000 meters
      {9000.0,  303.0},     // 9,000 meters
      {10000.0, 299.0},     // 10,000 meters
      {15000.0, 295.0},     // 15,000 meters
      {20000.0, 295.0},     // 20,000 meters
      {25000.0, 295.0},     // 25,000 meters
      {30000.0, 305.0},     // 30,000 meters
      {40000.0, 324.0},     // 40,000 meters
      {50000.0, 337.0},     // 50,000 meters
      {60000.0, 319.0},     // 60,000 meters
      {70000.0, 289.0},     // 70,000 meters
      {80000.0, 269.0}      // 80,000 meters
   };
   
   constexpr int numMapping = sizeof(speedSoundMapping) / sizeof(speedSoundMapping[0]);
   return linearInterpolation(speedSoundMapping, numMapping, altitude);
}

/*********************************************************
 * DRAG FROM MACH
 * Determine the drag coefficient for a M795 shell given speed in Mach
 * Based on empirical data for artillery projectiles
 *********************************************************/
double dragFromMach(double speedMach)
{
   // Drag coefficient values for M795 projectile at different Mach numbers
   static const Mapping dragMapping[] =
   {
      {0.0,   0.0},         // Mach 0.0
      {0.1,   0.0543},      // Mach 0.1
      {0.3,   0.1629},      // Mach 0.3
      {0.5,   0.1659},      // Mach 0.5
      {0.7,   0.2031},      // Mach 0.7
      {0.89,  0.2597},      // Mach 0.89
      {0.92,  0.3010},      // Mach 0.92
      {0.96,  0.3287},      // Mach 0.96
      {0.98,  0.4002},      // Mach 0.98
      {1.00,  0.4258},      // Mach 1.0 (sonic)
      {1.02,  0.4335},      // Mach 1.02
      {1.06,  0.4483},      // Mach 1.06
      {1.24,  0.4064},      // Mach 1.24
      {1.53,  0.3663},      // Mach 1.53
      {1.99,  0.2897},      // Mach 1.99
      {2.87,  0.2297},      // Mach 2.87
      {2.89,  0.2306},      // Mach 2.89
      {5.00,  0.2656}       // Mach 5.0
   };
   
   constexpr int numMapping = sizeof(dragMapping) / sizeof(dragMapping[0]);
   return linearInterpolation(dragMapping, numMapping, speedMach);
}
