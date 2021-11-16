/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:32:32
 * @LastEditTime: 2021-11-15 21:38:53
 * @LastEditors: Cuibb
 */

#include "interrupt.h"
#include "task.h"

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

void SysCallHandler(ushort ax)   // __cdecl__
{
    if ( ax == 0 ) {
        KillTask();
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
