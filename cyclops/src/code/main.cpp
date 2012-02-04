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
	
	CUSB::init();
	CUSB::send('a');
	
	CUSSensor * sensor = new CUSSensor(CUSSensor::eSensor0);
	
	sensor->measure();
	while(sensor->state() != CUSSensor::eReady)
	{}
	int distance = sensor->lastDistance();
	if(distance > 0)
	{
		CUSB::send((unsigned char)distance);
	}
	else
	{
		CUSB::send(255);
	}

	while( 1 ) // Main loop
	{
		if(clock::millis() > 500)
		{
			clock::reset();
			toggleLed();
		}
	}
}