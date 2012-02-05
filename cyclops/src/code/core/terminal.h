//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 5th, 2012
//----------------------------------------------------------------------------------------------------------------------
// Communications terminal

#ifndef _CYCLOPS_CORE_TERMINAL_H_
#define _CYCLOPS_CORE_TERMINAL_H_

#include "types.h"

class terminal
{
public:
	static void print(const char * _str);
	static void write(s16 _x);
	static void write(u16 _x);
	static void write(s32 _x);
	static void write(u32 _x);
	static void write(const void * _buffer, size_t _size);
};

#endif // _CYCLOPS_CORE_TERMINAL_H_
