/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-13 23:59:21
 * @LastEditTime: 2021-12-14 02:28:06
 * @LastEditors: Cuibb
 */

#include "mutex.h"
#include "screen.h"
#include "memory.h"
#include "task.h"

#define CMD_MUTEX_CERATE     0
#define CMD_MUTEX_ENTER      1
#define CMD_MUTEX_EXIT       2
#define CMD_MUTEX_DESTROY    3

static List gMList = {0};

void MutexModInit()
{
    List_Init(&gMList);
}

void MutexCallHandler(uint cmd, uint param)
{
    switch(cmd)
    {
        case CMD_MUTEX_CERATE: 
            *(uint*)param = SysCreateMutex();
            break;

        case CMD_MUTEX_ENTER:
            SysEnterCritical((Mutex*)param);
            break;

        case CMD_MUTEX_EXIT:
            SysExitCritical((Mutex*)param);
            break;

        case CMD_MUTEX_DESTROY:
            SysDestroyMutex((Mutex*)param);
            break;

        default:
            break;
    }
}

static bool IsMutexValid(Mutex* mutex)
{
    bool ret = False;

    if ( mutex ) {
        ListNode* pn = NULL;

        List_ForEach(&gMList, pn) {
            if ( IsEqual(mutex, pn) ) {
                ret = True;
                break;
            }
        }
    }

    return ret;
}

uint SysCreateMutex()
{
    Mutex* ret = (Mutex*)Malloc(sizeof(Mutex));

    if ( ret ) {
        ret->lock = 0;

        List_Add(&gMList, (ListNode*)ret);
    }

    return (uint)ret;

}

void SysDestroyMutex(Mutex* mutex)
{
    if ( mutex && IsMutexValid(mutex) ) {
        List_DelNode((ListNode*)mutex);

        Free(mutex);
    }
}

void SysEnterCritical(Mutex* mutex)
{
    if ( mutex && IsMutexValid(mutex) ) {
        if ( mutex->lock ) {
            PrintString("Move current task to wait\n");
            MtxSchedule(WAIT);
        } else {
            mutex->lock = 1;
            PrintString("Enter critical section\n");
        }
    }
}

void SysExitCritical(Mutex* mutex)
{
    if ( mutex && IsMutexValid(mutex) ) {
        mutex->lock = 0;
        PrintString("Notify all task to run again\n");
        MtxSchedule(NOTIFY);
    }
}