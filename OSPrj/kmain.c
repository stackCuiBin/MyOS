/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-10 16:15:55
 * @LastEditTime: 2021-11-16 00:10:39
 * @LastEditors: Cuibb
 */

#include "task.h"
#include "interrupt.h"
#include "screen.h"
#include "app.h"

void KMain()
{
    int n = PrintString("D.T.OS\n");
    
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
    
    AppModInit();

    TaskModInit();
    
    /* 中断初始化延后，在任务初始化完成后再使能中断 */
    IntModInit();
    
    LaunchTask();

}
