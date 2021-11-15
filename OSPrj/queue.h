/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-15 16:11:38
 * @LastEditTime: 2021-11-15 16:33:29
 * @LastEditors: Cuibb
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef ListNode QueueNode;

typedef struct
{
    ListNode head;
    int length;
}Queue;

void Queue_Init(Queue* queue);
int Queue_IsEmpty(Queue* queue);
int Queue_IsContained(Queue* queue, QueueNode* node);
void Queue_Add(Queue* queue, QueueNode* node);
QueueNode* Queue_Front(Queue* queue);
QueueNode* Queue_Remove(Queue* queue);
int Queue_Length(Queue* queue);
/* 将队首元素移到队尾 */
void Queue_Rotate(Queue* queue);

#endif // QUEUE_H