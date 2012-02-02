//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fern�ndez-Ag�era Tortosa (a.k.a. Technik)
// on February 2nd, 2012
//----------------------------------------------------------------------------------------------------------------------
// System clock

#include "clock.h"

#include <avr/interrupt.h>
#include <avr/io.h>

//----------------------------------------------------------------------------------------------------------------------
// Global data
//----------------------------------------------------------------------------------------------------------------------
volatile u32 gTicksH = 0; // High word of tick count (low word is timer 1 register)

//----------------------------------------------------------------------------------------------------------------------
// Interrupt handler
//----------------------------------------------------------------------------------------------------------------------
ISR(TIMER1_OVF_vect)
{
	++gTicksH;	// Increase on buffer overflow
}

namespace hardware { namespace clock
{
	//------------------------------------------------------------------------------------------------------------------
	void init ()
	{
		TCCR1B |= 1 << 1; // Clock source equals system clock / 8, 0.5 uS per tick
	}

	//------------------------------------------------------------------------------------------------------------------
	void reset ()
	{
		gTicksH = 0;// Reset stored time
		TCNT1 = 0;	// Reset timer register
	}

	//------------------------------------------------------------------------------------------------------------------
	u32 micros ()
	{
		return (gTicksH << 15) | (TCNT1 >> 1); // Return nTicks / 2
	}

	//------------------------------------------------------------------------------------------------------------------
	u32 millis ()
	{
		return micros() / 1000;
	}

	//------------------------------------------------------------------------------------------------------------------
	u32 seconds ()
	{
		return micros() / 1000000;
	}
}	// namespace clock
}	// namespace hardware