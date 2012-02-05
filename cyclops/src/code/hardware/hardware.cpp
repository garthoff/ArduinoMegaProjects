//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 2nd, 2012
//----------------------------------------------------------------------------------------------------------------------
// hardware abstraction

#include "hardware.h"

// Avr header files
#include <avr/interrupt.h>
#include <avr/io.h>

// Project header files
#include "clock.h"
#include "usb.h"

namespace hardware
{
	//------------------------------------------------------------------------------------------------------------------
	void init ()
	{
		sei();			// Enable global interrupts
		clock::init();	// Initialize system clock
		
		CUSB::init();	// Initialize usb communications

		DDRB |= 1 << 7;	// on board led pin must be set as output
		PORTB |= 1 << 7;// Turn the led on
	}

	//------------------------------------------------------------------------------------------------------------------
	void toggleLed ()
	{
		PINB = 1 << 7;	// Toggle led
	}
}	// namespace hardware
