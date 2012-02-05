//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 2nd, 2012
//----------------------------------------------------------------------------------------------------------------------
// main

#include "core/terminal.h"
#include "hardware/clock.h"
#include "hardware/hardware.h"
#include "hardware/usSensor.h"

using namespace hardware;

int main ()
{
	init();	// Initialize hardware

	unsigned long lastMillis = 0;
	while( 1 ) // Main loop
	{
		lastMillis = clock::millis();
		terminal::write(USSensor::measure(USSensor::eSensor0));
		while((clock::millis() - lastMillis) < 250)
		{}
	}
}