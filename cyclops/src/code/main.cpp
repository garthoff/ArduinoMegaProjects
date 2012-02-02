//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 2nd, 2012
//----------------------------------------------------------------------------------------------------------------------
// main

#include "hardware/clock.h"
#include "hardware/hardware.h"

using namespace hardware;

int main ()
{
	init();	// Initialize hardware

	while( 1 ) // Main loop
	{
		if(clock::seconds())
		{
			clock::reset();
			toggleLed();
		}
	}
}