//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 5th, 2012
//----------------------------------------------------------------------------------------------------------------------
// Communications terminal

#include "terminal.h"

#include "hardware/usb.h"

using namespace hardware;

//----------------------------------------------------------------------------------------------------------------------
void terminal::print(const char * _str)
{
	const unsigned char * str = reinterpret_cast<const unsigned char*>(_str);
	// Measure the string
	size_t size = 0;
	for(const char * i = _str; *i != '\0'; ++i)
		++size;
	write(str, size);
}

//----------------------------------------------------------------------------------------------------------------------
void terminal::write(s16 _x)
{
	write(&_x, sizeof(s16));
}

//----------------------------------------------------------------------------------------------------------------------
void terminal::write(u16 _x)
{
	write(&_x, sizeof(u16));
}

//----------------------------------------------------------------------------------------------------------------------
void terminal::write(s32 _x)
{
	write(&_x, sizeof(s32));
}

//----------------------------------------------------------------------------------------------------------------------
void terminal::write(u32 _x)
{
	write(&_x, sizeof(u32));
}

//----------------------------------------------------------------------------------------------------------------------
void terminal::write(const void * _buffer, size_t _size)
{
	const unsigned char * buffer = reinterpret_cast<const unsigned char*>(_buffer);
	// Send packet header, two 255's
	usb::send(255);
	usb::send(255);
	unsigned char checksum = 0;
	// Send message size
	usb::send(_size>>8);
	usb::send(_size&0xff);
	checksum+=(_size>>8) + (_size&0xff);
	// Send the actual message
	for(size_t i = 0; i < _size; ++i)
	{
		checksum+= buffer[i];
		usb::send(buffer[i]);
	}
	// Send the checksum
	usb::send(checksum);
}