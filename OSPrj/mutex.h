/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-13 23:59:06
 * @LastEditTime: 2021-12-14 19:05:43
 * @LastEditors: Cuibb
 */

#ifndef MUTEX_H
#define MUTEX_H

#include "type.h"
#include "queue.h"

enum
{
    Normal,
    Strict
};

typedef struct 
{
    ListNode head;
    Queue wait;
    uint type;
    uint lock;
} Mutex;

void MutexModInit();
void MutexCallHandler(uint cmd, uint param1, uint param2);


#endif // MUTEX_H