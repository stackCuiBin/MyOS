/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-15 22:58:16
 * @LastEditTime: 2021-11-16 00:39:32
 * @LastEditors: Cuibb
 */

#ifndef APP_H
#define APP_H

#include "type.h"

typedef struct
{
    const char* name;
    void (*tmain)();
    byte priority;
} AppInfo;


#endif // APP_H