/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-14 00:09:00
 * @LastEditTime: 2022-02-28 00:38:56
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
void RegApp(const char* name, void(*tmain)(), byte pri);

uint CreateMutex(uint type);
void EnterCritical(uint mutex);
void ExitCritical(uint mutex);
uint DestroyMutex(uint mutex);

#endif // SYSCALL_H