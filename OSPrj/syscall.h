/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-14 00:09:00
 * @LastEditTime: 2021-12-14 19:07:07
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

uint CreateMutex(uint type);
void EnterCritical(uint mutex);
void ExitCritical(uint mutex);
uint DestroyMutex(uint mutex);

#endif // SYSCALL_H