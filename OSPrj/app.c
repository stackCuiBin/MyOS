/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-15 23:00:32
 * @LastEditTime: 2021-12-09 02:23:03
 * @LastEditors: Cuibb
 */

#include "app.h"
#include "utility.h"
#include "memory.h"

#define MAX_APP_NUM       16

static AppInfo  gAppToRun[MAX_APP_NUM] = {0};
static uint gAppNum = 0;

void TaskA();
void TaskB();
void TaskC();
void TaskD();

static void RegApp(const char* name, void (*tmain)(), byte pri)
{
    if ( gAppNum < MAX_APP_NUM ) {
        AppInfo* app = AddrOff(gAppToRun, gAppNum);

        /* 需要深拷贝 */
        app->name = name;
        app->tmain = tmain;
        app->priority = pri;

        gAppNum++;
    }
}

void AppMain()
{
    RegApp("Task A", TaskA, 255);
    // RegApp("Task B", TaskB, 230);
    // RegApp("Task C", TaskC, 230);
    // RegApp("Task D", TaskD, 255);
}

AppInfo* GetAppToRun(uint index)
{
    AppInfo* ret = NULL;

    if ( index < MAX_APP_NUM ) {
        ret = AddrOff(gAppToRun, index);
    }
    
    return ret;
}

uint GetAppNum()
{
    return gAppNum;
}


void TaskA()
{
    int i = 0;
    
    SetPrintPos(0, 12);
    
    PrintString(__FUNCTION__);
    PrintChar('\n');
    
    uint* p = (uint*)Malloc(sizeof(uint) * 10);

    PrintIntHex(p);
    PrintChar('\n');
    *p = 666;
    PrintIntDec(*p);
    PrintChar('\n');

    Free(p);
}

void TaskB()
{
    int i = 0;
    
    SetPrintPos(0, 13);
    
    PrintString(__FUNCTION__);
    
    while(1)
    {
        SetPrintPos(8, 13);
        PrintChar('0' + i);
        i = (i + 1) % 10;
        Delay(1);
    }
}

void TaskC()
{
    int i = 0;
    
    SetPrintPos(0, 14);
    
    PrintString(__FUNCTION__);
    
    while(1)
    {
        SetPrintPos(8, 14);
        PrintChar('a' + i);
        i = (i + 1) % 26;
        Delay(1);
    }
}

void TaskD()
{
    int i = 0;
    
    SetPrintPos(0, 15);
    
    PrintString(__FUNCTION__);
    
    while(1)
    {
        SetPrintPos(8, 15);
        PrintChar('!' + i);
        i = (i + 1) % 10;
        Delay(1);
    }
}