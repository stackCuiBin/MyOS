/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-14 00:09:00
 * @LastEditTime: 2022-02-27 22:29:58
 * @LastEditors: Cuibb
 */

#ifndef SYSCALL_H
#define SYSCALL_H

#include "type.h"

enum
{
    Normal,
    Strict
};

void Exit();
void Wait(const char* name);

uint CreateMutex(uint type);
void EnterCritical(uint mutex);
void ExitCritical(uint mutex);
uint DestroyMutex(uint mutex);

#endif // SYSCALL_H