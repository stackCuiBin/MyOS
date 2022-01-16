/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-13 23:59:06
 * @LastEditTime: 2021-12-14 19:05:43
 * @LastEditors: Cuibb
 */

#ifndef MUTEX_H
#define MUTEX_H

#include "type.h"
#include "list.h"

typedef struct
{
    ListNode head;
    uint lock;
}Mutex;

void MutexModInit();
void MutexCallHandler(uint cmd, uint param1, uint param2);

uint SysCreateMutex();
void SysEnterCritical(Mutex* mutex, uint* wait);
void SysExitCritical(Mutex* mutex);
void SysDestroyMutex(Mutex* mutex);

#endif // MUTEX_H