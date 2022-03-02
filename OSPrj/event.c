/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2022-03-02 22:31:37
 * @LastEditTime: 2022-03-02 22:36:50
 * @LastEditors: Cuibb
 */

#include "event.h"
#include "memory.h"

Event* CreateEvent(uint type, uint id, uint param1, uint param2)
{
    Event* ret = Malloc(sizeof(Event));

    if (ret) {
        ret->type = type;
        ret->id = id;
        ret->param1 = param1;
        ret->param2 = param2;
    }

    return ret;
}

void DestroyEvent(Event* event)
{
    Free(event);
}
