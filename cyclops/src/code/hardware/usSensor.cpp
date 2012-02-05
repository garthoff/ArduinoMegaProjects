//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 4th, 2012
//----------------------------------------------------------------------------------------------------------------------
// Ultrasound interface
#include <avr/io.h>
#include <avr/interrupt.h>

#include "clock.h"
#include "usSensor.h"
#include "usb.h"

// ISR(TIMER4_CAPT_vect)
// {
// 	hardware::USSensor::onCaptureEvent();
// }
// 
// ISR(TIMER5_CAPT_vect)
// {
// 	hardware::USSensor::onCaptureEvent();
// }

namespace hardware
{
	unsigned USSensor::measure(ESensor _sensor)
	{
		if(_sensor == eSensor0)
		{
			// -- Launch pulse
			DDRL |= 1; // Pin as output
			PORTL|= 1; // Pin high
			clock::delayUS(10);
			//usb::send(1);
			PORTL&=~1; // Pin low
			DDRL &=~1; // Pin as input
			// -- Wait for echo
			while(!(PINL & 1)) {} // Wait for pulse
			//usb::send(2);
			// -- Measure echo duration
			u32 us = clock::micros(); // micro seconds time stamp
			while((PINL & 1)) {}	// wait for completion
			//usb::send(3);
			u32 delta = clock::micros() - us; // Measure difference
			return (unsigned)(float(delta) * 0.17f);
		}
		return 0;
	}
	/*//------------------------------------------------------------------------------------------------------------------
	// Static data
	//------------------------------------------------------------------------------------------------------------------
	USSensor * USSensor::sActiveSensor = 0;
	USSensor * USSensor::sNextSensor = 0;
	
	//------------------------------------------------------------------------------------------------------------------
	// Implementation
	//------------------------------------------------------------------------------------------------------------------
	USSensor::USSensor(ESensor _slot)
		:mSlot(_slot)
		,mState(eReady)
		,mDistance(-1)
		,mDestroy(false)
		,mEchoArrived(false)
		,mUS(0)
		,mCallback(0)
	{
		// Configure registers
		switch(_slot)
		{
			case eSensor0:	// Sensor 0 uses timer 4 hardware, digital pin 49 in arduino mega
			{
				TCCR4B = (1 << 7); // Activate noise canceller
				TCCR4B |= (1 << 1); // Start counting time (0.5 uS per tick)
				// Note: The clock never stops counting to save the initialization and stop instructions.
				// It just resets when required
				break;
			}
			case eSensor1:
			default:		// Sensor 1 uses timer 5 hardware, digital pin 48 in arduino mega
			{
				break;	
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void USSensor::destroy()
	{
		cli();	// Disable interrupts to prevent this sensor to become active in the middle of its destruction
		if(this == sActiveSensor)
		{	// If there is an ongoing measure, we delay deletion till measure completion
			mDestroy = true;
		}
		else
		{
			if(this == sNextSensor)
				sNextSensor = 0;
			delete this;
		}
		sei();	// Restore interrupts
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void USSensor::measure()
	{
		if(mState != eReady)
			return;	// Already trying to measure
		
		// If there is an active sensor already, enqueue myself
		if(0 != sActiveSensor)
		{
			sNextSensor = this;
			mState = eWaiting;
			return;
		}
		
		// If we get here, it means we can start our measure
		initMeasure();
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void USSensor::initMeasure()
	{
		mState = eMeasuring;
		sActiveSensor = this;
		switch(mSlot)
		{
			case eSensor0:
			{
				DDRL |= 1;	// Set sensor pin as output
				PORTL|= 1;	// Start activation pulse
				// -- Delay 10 microseconds (~160 instructions)
// 				unsigned long us = clock::micros();
// 				while((clock::micros() - us) < 10)
// 				{}
				TCNT4 = 0;	// Clear counter
				while(TCNT4L < 20)	// 20 ticks mean 10 microseconds
				{}
				// -- End of delay --
				PORTL&= ~1;	// Terminate activation pulse
				DDRL &= ~1;
				break;
			}
			case eSensor1:
			default:
			{
				DDRL |= 1<<1;	// Set sensor pin as output
				PORTL|= 1<<1;	// Start activation pulse
				// -- Delay 10 microseconds (~160 instructions)
				TCNT5 = 0;	// Clear counter
				while(TCNT5L < 20)	// 20 ticks mean 10 microseconds
				{}
				// -- End of delay --
				PORTL&= ~(1<<1);	// Terminate activation pulse
				DDRL &= ~(1<<1);
				break;
			}
		}
		// Wait for the echo
		listenForEchoSignal();
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void USSensor::listenForEchoSignal()
	{
		mEchoArrived = false;
		switch(mSlot)
		{
			case eSensor0:
			{
				TIFR4 = 1 << 7;		// Clear input capture events
				TIMSK4 |= 1 << 5;	// Enable input capture interrupts
				TCCR4B |= (1 << 6); // Listen to raising events in the input pin
				break;
			}
			case eSensor1:
			{
				TIFR5 = 1 << 7;		// Clear input capture events
				TIMSK5 |= 1 << 5;	// Enable input capture interrupts
				TCCR5B |= (1 << 6); // Listen to raising events in the input pin
				break;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void USSensor::onEchoSignal()
	{
		sActiveSensor->listenForEchoEnd();
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void USSensor::listenForEchoEnd()
	{
		mEchoArrived = true;
		switch(mSlot)
		{
			case eSensor0:
			{
				TCNT4 = 0;			// Clear clock counter
				TCCR4B &= ~(1 << 6); // Listen to lowering events in the input pin
				break;
			}
			case eSensor1:
			{
				TCNT5 = 0;			// Clear clock counter
				TCCR5B &= ~(1 << 6); // Listen to lowering events in the input pin
				break;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void USSensor::onEchoEnd()
	{
		sActiveSensor->finishMeasure();
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void USSensor::onCaptureEvent()
	{
		if(sActiveSensor)
		{
			if(sActiveSensor->mEchoArrived)
			{
				sActiveSensor->onEchoEnd();
			}				
			else
			{
				sActiveSensor->onEchoSignal();
			}				
		}
		// clear the flag		
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void USSensor::finishMeasure()
	{
		unsigned timeStamp;
		switch(mSlot)
		{
			case eSensor0:
			{
				TIMSK4 &= ~(1 << 5);	// Disable input capture interrupts
				timeStamp = ICR4 >> 9; // clock::micros() - mUS;
				usb::send(timeStamp);
			}
			case eSensor1:
			{
				timeStamp = ICR5 >> 1;
			}
		}
		if(timeStamp > 25000)	// Measure out of range
		{
			mDistance = -1;
		}
		else
		{
			mDistance = int (float(timeStamp) * 0.17f);
		}
		if(0 != mCallback)
		{
			mCallback(this);
		}			
		mState = eReady;
		if(mDestroy)
		{
			delete this;
		}			
		sActiveSensor = sNextSensor;
		if(sNextSensor)
		{
			sNextSensor->initMeasure();
		}
	}*/
}	// namespace hardware
