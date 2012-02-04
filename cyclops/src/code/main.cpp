//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 2nd, 2012
//----------------------------------------------------------------------------------------------------------------------
// main

#include "hardware/clock.h"
#include "hardware/hardware.h"
#include "hardware/usb.h"
#include "hardware/usSensor.h"

using namespace hardware;

int main ()
{
	init();	// Initialize hardware
	
	CUSSensor * sensor = new CUSSensor(CUSSensor::eSensor0);
	
	sensor->measure();

	unsigned long lastMillis = 0;
	while( 1 ) // Main loop
	{
		lastMillis = clock::millis();
		while((clock::millis() - lastMillis) < 250)
		{}
		toggleLed();
	}
}