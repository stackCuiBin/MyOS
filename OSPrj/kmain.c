/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-10 16:15:55
 * @LastEditTime: 2022-03-04 00:48:04
 * @LastEditors: Cuibb
 */

#include "task.h"
#include "interrupt.h"
#include "screen.h"
#include "memory.h"
#include "mutex.h"
#include "keyboard.h"
#include "hdraw.h"

void KMain()
{
    void (*AppModInit)() = (void*)BaseOfApp;
    /* BIOS启动时扫描硬件信息，硬盘数量存在此地址处 */
    byte* pn = (byte*)0x475;
    
    PrintString("D.T.OS\n");
    
    PrintString("GDT Entry: ");
    PrintIntHex((uint)gGdtInfo.entry);
    PrintChar('\n');
    
    PrintString("GDT Size: ");
    PrintIntDec((uint)gGdtInfo.size);
    PrintChar('\n');
    
    PrintString("IDT Entry: ");
    PrintIntHex((uint)gIdtInfo.entry);
    PrintChar('\n');
    
    PrintString("IDT Size: ");
    PrintIntDec((uint)gIdtInfo.size);
    PrintChar('\n');
    
    PrintString("Number of Hard Disk: ");
    PrintIntDec(*pn);
    PrintChar('\n');
    
    MemModInit((byte*)KernelHeapBase, HeapSize);

    KeyboardModInit();

    MutexModInit();

    // AppModInit();

    TaskModInit();
    
    /* 中断初始化延后，在任务初始化完成后再使能中断 */
    IntModInit();
    
    ConfigPageTable();
    
    while(1);
    
    LaunchTask();

}
