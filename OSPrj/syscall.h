/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-14 00:09:00
 * @LastEditTime: 2021-12-14 00:11:52
 * @LastEditors: Cuibb
 */

#ifndef SYSCALL_H
#define SYSCALL_H

#include "type.h"

void Exit();

uint CreateMutex();
void EnterCritical(uint mutex);
void ExitCritical(uint mutex);
void DestroyMutex(uint mutex);

#endif // SYSCALL_H