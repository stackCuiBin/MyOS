/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-08 21:39:13
 * @LastEditTime: 2021-12-09 01:40:35
 * @LastEditors: Cuibb
 */

#ifndef MEMORY_H
#define MEMORY_H

#include "type.h"

// #define MEM_DEBUG    1

void MemModInit(byte* mem, uint size);
void* Malloc(uint size);
void Free(void* ptr);

#ifdef MEM_DEBUG
void FMemTest();
void VMemTest();
#endif

#endif // MEMORY_H