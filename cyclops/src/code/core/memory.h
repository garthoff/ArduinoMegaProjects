//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 4th, 2012
//----------------------------------------------------------------------------------------------------------------------
// Memory

#ifndef _CYCLOPS_CORE_MEMORY_H_
#define _CYCLOPS_CORE_MEMORY_H_

#include "types.h"

void * operator new (size_t);
void operator delete(void *);

#endif // _CYCLOPS_CORE_MEMORY_H_
