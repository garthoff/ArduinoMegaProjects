//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 4th, 2012
//----------------------------------------------------------------------------------------------------------------------
// Ultrasound interface
#include <avr/io.h>
#include <avr/interrupt.h>

#include "usSensor.h"

ISR(TIMER4_CAPT_vect)
{
	CUSSensor::onCaptureEvent();
}

ISR(TIMER5_CAPT_vect)
{
	CUSSensor::onCaptureEvent();
}

namespace hardware
{
	//------------------------------------------------------------------------------------------------------------------
	// Static data
	//------------------------------------------------------------------------------------------------------------------
	CUSSensor * CUSSensor::sActiveSensor = 0;
	CUSSensor * CUSSensor::sNextSensor = 0;
	
	//------------------------------------------------------------------------------------------------------------------
	// Implementation
	//------------------------------------------------------------------------------------------------------------------
	CUSSensor::CUSSensor(ESensor _slot)
		:mSlot(_slot)
		,mState(eReady)
		,mDistance(-1)
		,mDestroy(false)
		,mCallback(0)
	{
		// Configure registers
		switch(_slot)
		{
			case eSensor0:	// Sensor 0 uses timer 4 hardware, digital pin 49 in arduino mega
			{
				TCCR4B = (1 << 7); // Activate noise canceller
				TCCR4B |= (1 << 8); // Start counting time (0.5 uS per tick)
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
	void CUSSensor::destroy()
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
	void CUSSensor::measure()
	{
		if(mState != eReady)
			return;	// Already trying to measure
		
		// If there is an active sensor already, enqueue myself
		if(0 == sActiveSensor)
		{
			sNextSensor = this;
			mState = eWaiting;
			return;
		}
		
		// If we get here, it means we can start our measure
		initMeasure();
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void CUSSensor::initMeasure()
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
				TCNT4 = 0;	// Clear counter
				while(TCNT4L < 20)	// 20 ticks mean 10 microseconds
				{}
				// -- End of delay --
				PORTL&= ~1;	// Terminate activation pulse
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
				break;
			}
		}
		// Wait for the echo
		listenForEchoSignal();
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void CUSSensor::listenForEchoSignal()
	{
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
	void CUSSensor::onEchoSignal()
	{
		sActiveSensor->listenForEchoEnd();
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void CUSSensor::listenForEchoEnd()
	{
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
	void CUSSensor::onEchoEnd()
	{
		sActiveSensor->finishMeasure();
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void CUSSensor::onCaptureEvent()
	{
		//
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void CUSSensor::finishMeasure()
	{
		unsigned timeStamp;
		switch(mSlot)
		{
			case eSensor0:
			{
				timeStamp = ICR4 >> 1;
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
			mCallback(this);
		mState = eReady;
		if(mDestroy)
			delete this;
		sActiveSensor = sNextSensor;
		if(sNextSensor)
		{
			sNextSensor->initMeasure();
		}
	}
}	// namespace hardware
