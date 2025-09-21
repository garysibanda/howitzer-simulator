/**********************************************************************
 * Source File:
 *    Lab 12: M777 Howitzer
 * Author:
 *    Br. Helfrich and Gary Sibanda
 * Summary:
 *    Simulate firing the M777 howitzer 155mm artillery piece
 ************************************************************************/

#include <cassert>      // for ASSERT
#include <iomanip>      // for formatting
#include "uiInteract.h" // for INTERFACE
#include "uiDraw.h"     // for RANDOM and DRAW*
#include "simulation.h" // for SIMULATION
#include "position.h"   // for POSITION
#include "test.h"       // for the unit tests

using namespace std;

/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/
void callBack(const Interface* pUI, void* p)
{
   // Cast the void pointer into our Simulator object.
   Simulator* pSim = (Simulator*)p;
   
   assert(pSim != nullptr);
   assert(pUI != nullptr);
   
   // Handle user input
   pSim->handleInput(pUI);
   
   // Update simulation physics
   pSim->update(0.5);  // 0.5 second time step
   
   // Set up graphics output stream
   ogstream gout(Position(10.0, pSim->getPosUpperRight().getPixelsY() - 20.0));
   
   // Render the simulation
   pSim->draw(gout);
}

// Initialize the static member for pixel-to-meter conversion
double Position::metersFromPixels = 40.0;

/*********************************
 * Initialize the simulation and set it in motion
 *********************************/
#ifdef _WIN32
#include <windows.h>
int WINAPI wWinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ PWSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   // Run unit tests first
   testRunner();

   // Initialize OpenGL window
   Position posUpperRight;
   posUpperRight.setZoom(40.0);  // 40 meters equals 1 pixel
   posUpperRight.setPixelsX(700.0);
   posUpperRight.setPixelsY(500.0);
   
   // Create the user interface
   Interface ui("M777 Howitzer Simulation - Enhanced Edition", posUpperRight);

   // Initialize the simulation
   Simulator sim(posUpperRight);

   // Set everything into action
   ui.run(callBack, (void*)&sim);

   return 0;
}
