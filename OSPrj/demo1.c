/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2022-02-27 12:53:22
 * @LastEditTime: 2022-02-28 01:32:46
 * @LastEditors: Cuibb
 */

#include "demo1.h"
#include "memory.h"
#include "syscall.h"
#include "list.h"
#include "screen.h"

typedef struct 
{
    ListNode head;
    char product;
}Product;

enum { NUM = 60 };

static List g_store = {0};
static uint g_mutex = 0;
static int g_num = 0;
static int g_get = NUM;

static void Store(char c)
{
    Product* p = Malloc(sizeof(Product));

    p->product = c;
    List_AddTail(&g_store, (ListNode*)p);
}

static int Fetch(char type, char* c)
{
    int ret = 0;
    ListNode* pos = NULL;
    
    List_ForEach(&g_store, pos) {
        Product* p = (Product*)pos;

        if (type == 'A') {
            ret = ('A' <= p->product) && ('Z' >= p->product);
        } else if (type == 'B') {
            ret = ('a' <= p->product) && ('z' >= p->product);
        }

        if (ret) {
            *c = p->product;
            List_DelNode(pos);
            Free(pos);
            break;
        }
    }

    return ret;
}

static void ProducerA()
{
    int next = 0;
    int run = 1;
    
    SetPrintPos(TASK_START_W, TASK_START_H);
    
    PrintString(__FUNCTION__);

    while (1) {
        EnterCritical(g_mutex);
        if (run = (g_num < NUM)) {
            char c = 'A' + next % 26;

            Store(c);

            g_num++;
            
            SetPrintPos(TASK_START_W + 12 + next, TASK_START_H);
            PrintChar(c);
            
            next++;
        }
        ExitCritical(g_mutex);

        if (run) 
            Delay(1);
        else
            break;
    }
}

static void ProducerB()
{
    int next = 0;
    int run = 1;
    
    SetPrintPos(TASK_START_W, TASK_START_H + 2);
    
    PrintString(__FUNCTION__);

    while (1) {
        EnterCritical(g_mutex);
        if (run = (g_num < NUM)) {
            char c = 'a' + next % 26;

            Store(c);

            g_num++;
            
            SetPrintPos(TASK_START_W + 12 + next, TASK_START_H + 2);
            PrintChar(c);
            
            next++;
        }
        ExitCritical(g_mutex);

        if (run) 
            Delay(next % 2 + 1);
        else
            break;
    }
}

static void ConsumerA()
{
    int next = 0;
    int run = 1;
    
    SetPrintPos(TASK_START_W, TASK_START_H + 4);
    
    PrintString(__FUNCTION__);

    while (1) {
        char c = 0;

        EnterCritical(g_mutex);
        if ((run = (g_get > 0)) && Fetch('A', &c)) {
            SetPrintPos(TASK_START_W + 12 + next++, TASK_START_H + 4);
            PrintChar(c);

            g_get--;
        }
        ExitCritical(g_mutex);

        if (run)
            Delay(next % 2 + 1);
        else
            break;
    }
}

static void ConsumerB()
{
    int next = 0;
    int run = 1;
    
    SetPrintPos(TASK_START_W, TASK_START_H + 6);
    
    PrintString(__FUNCTION__);

    while (1) {
        char c = 0;

        EnterCritical(g_mutex);
        if ((run = (g_get > 0)) && Fetch('B', &c)) {
            SetPrintPos(TASK_START_W + 12 + next++, TASK_START_H + 6);
            PrintChar(c);

            g_get--;
        }
        ExitCritical(g_mutex);

        if (run)
            Delay(2);
        else
            break;
    }
}

static void Init()
{
    g_mutex = CreateMutex(Strict);
    g_num = 0;
    g_get = NUM;
    
    List_Init(&g_store);
}

static void DeInit()
{
    Wait("PA");
    Wait("PB");
    Wait("CA");
    Wait("CB");
    
    SetPrintPos(TASK_START_W, TASK_START_H + 8);
    PrintString(__FUNCTION__);
    
    DestroyMutex(g_mutex);
}

void RunDemo1()
{
    Init();

    RegApp("PA", ProducerA, 255);
    RegApp("PB", ProducerB, 255);
    RegApp("CA", ConsumerA, 255);
    RegApp("CB", ConsumerB, 255);
    RegApp("DeInit", DeInit, 255);
}
