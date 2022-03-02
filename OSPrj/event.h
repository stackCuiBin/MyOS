/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2022-03-02 22:31:25
 * @LastEditTime: 2022-03-02 22:34:37
 * @LastEditors: Cuibb
 */

#ifndef EVENT_H
#define EVENT_H

#include "type.h"

enum {
    NoneEvent,
    MutexEvent,
    KeyEvent,
    TaskEvent
};

typedef struct 
{
    uint type;
    uint id;
    uint param1;
    uint param2;
} Event;

Event* CreateEvent(uint type, uint id, uint param1, uint param2);
void DestroyEvent(Event* event);


#endif // EVENT_H