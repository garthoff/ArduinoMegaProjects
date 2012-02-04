//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 4th, 2012
//----------------------------------------------------------------------------------------------------------------------
// Serial communications over usb
#include <avr/io.h>

#include "usb.h"

namespace hardware
{
	//------------------------------------------------------------------------------------------------------------------
	void CUSB::init()
	{ // USB Link actually uses USART0 to communicate with the serial to usb bridge
		UCSR0A |= 1 << 1; // Enable 2x speed
		UCSR0B |= (1<<4)|(1<<3); // Enable transmitter and transmitter reciever
		// UCSR0B |= (1<<7); // Recieve complete interrupt enable
		UBRR0 = 16; // Set baud rate to 115200 bauds
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void CUSB::send(unsigned char _x)
	{
		while ((UCSR0A & (1 << UDRE0)) == 0)
		{}
		UDR0 = _x;
// 		while(!(UCSR0A & (1<<6))); // Wait for transmision to complete
// 		UCSR0A |= 1<<6;
	}
}	// namespace hardware
