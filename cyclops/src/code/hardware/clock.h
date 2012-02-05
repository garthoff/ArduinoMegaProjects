//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 2nd, 2012
//----------------------------------------------------------------------------------------------------------------------
// System clock

#ifndef _CYCLOPS_HARDWARE_CLOCK_H_
#define _CYCLOPS_HARDWARE_CLOCK_H_

#include "core/types.h"

namespace hardware { namespace clock
{
	// Clock life management
	void init	();	///< Initialize system clock
	// void reset	();	///< Reset system clock to zero
	
	u32	micros	();	///< Microseconds since power-up or last reset
	u32	millis	();	///< Milliseconds since power-up or last reset
	u32	seconds	();	///< seconds since power-up or last reset
	
	void	delayMS	(u16 _ms);	///< Delay milliseconds
	void	delayUS	(u16 _us);	///< Delay microseconds
}	// namespace clock
}	// namespace hardware

#endif // _CYCLOPS_HARDWARE_CLOCK_H_
