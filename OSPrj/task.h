/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:20:33
 * @LastEditTime: 2022-03-03 01:44:39
 * @LastEditors: Cuibb
 */

#ifndef TASK_H
#define TASK_H

#include "kernel.h"
#include "queue.h"
#include "event.h"

typedef struct {
    uint gs;
    uint fs;
    uint es;
    uint ds;
    uint edi;
    uint esi;
    uint ebp;
    uint kesp;
    uint ebx;
    uint edx;
    uint ecx;
    uint eax;
    uint error_code;
    uint eip;
    uint cs;
    uint eflags;
    uint esp;
    uint ss;
} RegValue;

typedef struct
{
    uint   previous;
    uint   esp0;
    uint   ss0;
    uint   unused[22];
    ushort reserved;
    ushort iomb;
} TSS;

typedef struct
{
    RegValue   rv;
    Descriptor ldt[3];
    ushort     ldtSelector;
    ushort     tssSelector;
    void       (*tmain)();
    uint       id;
    ushort     current; /* 已执行时间数 */
    ushort     total;   /* 执行的总时间数 */
    char       name[16];
    Queue      wait; 
    byte*      stack;
    Event*     event;
} Task;

typedef struct 
{
    QueueNode head;
    Task      task;
} TaskNode;

typedef struct
{
    const char* name;
    void (*tmain)();
    byte priority;
} AppInfo;

typedef struct
{
    QueueNode head;
    AppInfo app;
}AppNode;

enum {
    WAIT,
    NOTIFY
};

enum {
    TASK_CMD_KILL,
    TASK_CMD_WAIT,
    TASK_CMD_REG_APP
};

extern void (* const RunTask)(volatile Task* pt);
extern void (* const LoadTask)(volatile Task* pt);

void TaskModInit();
void LaunchTask();
void Schedule();
void TaskCallHandler(uint cmd, uint param1, uint param2);
void EventSchedule(uint action, Event* event);

const char* CurrentTaskName();
uint CurrentTaskId();

#endif
