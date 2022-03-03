/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:32:32
 * @LastEditTime: 2022-03-02 00:54:46
 * @LastEditors: Cuibb
 */

#include "interrupt.h"
#include "keyboard.h"
#include "task.h"
#include "mutex.h"
#include "screen.h"
#include "sysinfo.h"

#define TYPE_TASK_OPT    0
#define TYPE_MUTEX_OPT   1
#define TYPE_KEY_OPT     2
#define TYPE_SYSINFO_OPT 3

extern byte ReadPort(ushort port);

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
    
    NotifyKeyCode();
    
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
        case TYPE_KEY_OPT:
            KeyCallHandler(cmd, param1, param2);
			
        case TYPE_SYSINFO_OPT:
            SysInfoCallHandler(cmd, param1, param2);
            break;
        default:
            break;
    }
}

void PageFaultHandler()
{
    SetPrintPos(ERR_START_W, ERR_START_H);
    
    PrintString("Page Fault: kill ");
    PrintString(CurrentTaskName());
    
    TaskCallHandler(TASK_CMD_KILL, 0, 0);
}

void SegmentFaultHandler()
{
    SetPrintPos(ERR_START_W, ERR_START_H);
    
    PrintString("Segment Fault: kill ");
    PrintString(CurrentTaskName());
    
    TaskCallHandler(TASK_CMD_KILL, 0, 0);
}
