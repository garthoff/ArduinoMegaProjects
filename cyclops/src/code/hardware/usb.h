//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 4th, 2012
//----------------------------------------------------------------------------------------------------------------------
// Serial communications over usb

#ifndef _CYCLOPS_HARDWARE_USB_H_
#define _CYCLOPS_HARDWARE_USB_H_

namespace hardware
{
	class usb // Serial communications through usb Link
	{
	public:
		static void init ();
		static void send (unsigned char _x);
	};
}	// namespace hardware

#endif // _CYCLOPS_HARDWARE_USB_H_