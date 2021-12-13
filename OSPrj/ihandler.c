/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:32:32
 * @LastEditTime: 2021-12-14 02:33:00
 * @LastEditors: Cuibb
 */

#include "interrupt.h"
#include "task.h"
#include "mutex.h"

#define TYPE_KILL_TASK   0
#define TYPE_MUTEX_OPT   1

extern volatile Task* gCTaskAddr;

void TimerHandler()
{
    static uint i = 0;
    
    i = (i + 1) % 5;
    
    if( i == 0 )
    {
        Schedule();
    }
    
    SendEOI(MASTER_EOI_PORT);
}

void SysCallHandler(uint type, uint cmd, uint param1, uint param2)   // __cdecl__
{
    switch(type)
    {
        case TYPE_KILL_TASK:
            KillTask();
            break;

        case TYPE_MUTEX_OPT:
            MutexCallHandler(cmd, param1);
            break;

        default:
            break;
    }
}

void PageFaultHandler()
{
    SetPrintPos(0, 6);
    
    PrintString("Page Fault: kill ");
    PrintString(gCTaskAddr->name);
    
    KillTask();
}

void SegmentFaultHandler()
{
    SetPrintPos(0, 6);
    
    PrintString("Segment Fault: kill ");
    PrintString(gCTaskAddr->name);
    
    KillTask();
}
