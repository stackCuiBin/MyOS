/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:32:32
 * @LastEditTime: 2022-03-02 00:54:46
 * @LastEditors: Cuibb
 */

#include "interrupt.h"
#include "task.h"
#include "mutex.h"

#define TYPE_TASK_OPT    0
#define TYPE_MUTEX_OPT   1

extern byte ReadPort(ushort port);

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

void KeyboardHandler()
{
    byte sc = ReadPort(0x60);
    
    PutScanCode(sc);

    SendEOI(MASTER_EOI_PORT);
}

void SysCallHandler(uint type, uint cmd, uint param1, uint param2)   // __cdecl__
{
    switch(type)
    {
        case TYPE_TASK_OPT:
            TaskCallHandler(cmd, param1, param2);
            break;

        case TYPE_MUTEX_OPT:
            MutexCallHandler(cmd, param1, param2);
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
    
    TaskCallHandler(TASK_CMD_KILL, 0, 0);
}

void SegmentFaultHandler()
{
    SetPrintPos(0, 6);
    
    PrintString("Segment Fault: kill ");
    PrintString(gCTaskAddr->name);
    
    TaskCallHandler(TASK_CMD_KILL, 0, 0);
}
