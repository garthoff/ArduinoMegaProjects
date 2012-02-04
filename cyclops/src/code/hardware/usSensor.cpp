//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 4th, 2012
//----------------------------------------------------------------------------------------------------------------------
// Ultrasound interface
#include <avr/io.h>
#include <avr/interrupt.h>

#include "usSensor.h"
#include "usb.h"

ISR(TIMER4_CAPT_vect)
{
	hardware::CUSSensor::onCaptureEvent();
	hardware::CUSB::send('b');
}

ISR(TIMER5_CAPT_vect)
{
	hardware::CUSSensor::onCaptureEvent();
	hardware::CUSB::send('c');
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
		,mEchoArrived(false)
		,mCallback(0)
	{
		// Configure registers
		switch(_slot)
		{
			case eSensor0:	// Sensor 0 uses timer 4 hardware, digital pin 49 in arduino mega
			{
				CUSB::send('d');
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
		CUSB::send('e');
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
		CUSB::send('f');
		if(mState != eReady)
			return;	// Already trying to measure
		
		// If there is an active sensor already, enqueue myself
		CUSB::send('g');
		if(0 == sActiveSensor)
		{
			CUSB::send('h');
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
		CUSB::send('i');
		mState = eMeasuring;
		sActiveSensor = this;
		switch(mSlot)
		{
			case eSensor0:
			{
				CUSB::send('j');
				DDRL |= 1;	// Set sensor pin as output
				PORTL|= 1;	// Start activation pulse
				// -- Delay 10 microseconds (~160 instructions)
				TCNT4 = 0;	// Clear counter
				while(TCNT4L < 20)	// 20 ticks mean 10 microseconds
				{}
					CUSB::send('k');
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
	void CUSSensor::listenForEchoSignal()
	{
		mEchoArrived = false;
		switch(mSlot)
		{
			case eSensor0:
			{
				CUSB::send('l');
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
		CUSB::send('m');
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
	void CUSSensor::onEchoEnd()
	{
		sActiveSensor->finishMeasure();
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void CUSSensor::onCaptureEvent()
	{
		CUSB::send('n');
		if(sActiveSensor)
		{
			if(sActiveSensor->mEchoArrived)
			{
				CUSB::send('o');
				sActiveSensor->onEchoEnd();
			}				
			else
			{
				CUSB::send('p');
				sActiveSensor->onEchoSignal();
			}				
		}
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void CUSSensor::finishMeasure()
	{
		CUSB::send('q');
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
			CUSB::send('r');
			mDistance = -1;
		}
		else
		{
			CUSB::send('s');
			mDistance = int (float(timeStamp) * 0.17f);
		}
		if(0 != mCallback)
		{
			CUSB::send('t');
			mCallback(this);
		}			
		mState = eReady;
		if(mDestroy)
		{
			CUSB::send('u');
			delete this;
		}			
		sActiveSensor = sNextSensor;
		if(sNextSensor)
		{
			CUSB::send('v');
			sNextSensor->initMeasure();
		}
	}
}	// namespace hardware
