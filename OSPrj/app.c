/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-15 23:00:32
 * @LastEditTime: 2022-02-28 01:32:17
 * @LastEditors: Cuibb
 */

#include "utility.h"
#include "memory.h"
#include "syscall.h"

#include "demo1.h"
#include "demo2.h"

void TaskA();
void TaskB();
void TaskC();
void TaskD();

void CookRice();
void CookDish();
void HaveDinner();

void AppMain()
{   
    // 验证运行在用户态,访问非用户空间产生段错误
    // int* p = (int*)0x80000;
    // *p = 0;
    
    // SetPrintPos(0, 10);
    // PrintString("AppMain() : Hello D.T.OS!\n");

    // RegApp("CookRice", CookRice, 255);
    // RegApp("CookDish", CookDish, 255);
    // RegApp("HaveDinner", HaveDinner, 255);

    // RunDemo1();
    RunDemo2();
}

void CookRice()
{
    int i = 0;
    
    SetPrintPos(0, 12);
    
    PrintString(__FUNCTION__);
    PrintChar('\n');
    
    for(i=0; i<50; i++)
    {
        SetPrintPos(10, 12);
        PrintChar('A' + i % 26);
        Delay(1);
    }
}

void CookDish()
{
    int i = 0;
    
    SetPrintPos(0, 14);
    
    PrintString(__FUNCTION__);
    PrintChar('\n');
    
    for(i=0; i<30; i++)
    {
        SetPrintPos(10, 14);
        PrintChar('0' + i % 10);
        Delay(1);
    }
}

void HaveDinner()
{
    Wait("CookDish");
    Wait("CookRice");
    
    SetPrintPos(10, 16);
    PrintString("Having dinner...\n");
}

static uint g_mutex = 0;
static int i = 0;

void TaskA()
{
    SetPrintPos(0, 12);
    
    PrintString(__FUNCTION__);
    PrintChar('\n');
    
    g_mutex = CreateMutex(Normal);
    
    EnterCritical(g_mutex);
    
    for(i=0; i<50; i++)
    {
        SetPrintPos(8, 12);
        PrintChar('A' + i % 26);
        Delay(1);
    }
    
    ExitCritical(g_mutex);
}

void TaskB()
{
    SetPrintPos(0, 16);
    
    PrintString(__FUNCTION__);
    
    // ExitCritical(g_mutex);
    
    EnterCritical(g_mutex);
    
    i = 0;
    
    while(0)
    {
        SetPrintPos(8, 16);
        PrintChar('0' + i);
        i = (i + 1) % 10;
        Delay(1);
    }
    
    SetPrintPos(8, 16);
    
    i = DestroyMutex(g_mutex);
    
    PrintIntDec(i);
    PrintChar('\n');
    
    ExitCritical(g_mutex);
    
    i = DestroyMutex(g_mutex);
    
    PrintIntDec(i);
    PrintChar('\n');
    
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