/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-13 23:59:06
 * @LastEditTime: 2021-12-14 01:20:22
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
void MutexCallHandler(uint cmd, uint param);

uint SysCreateMutex();
void SysEnterCritical(Mutex* mutex);
void SysExitCritical(Mutex* mutex);
void SysDestroyMutex(Mutex* mutex);

#endif // MUTEX_H