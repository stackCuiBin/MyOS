/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-10 16:15:55
 * @LastEditTime: 2021-11-10 22:14:07
 * @LastEditors: Cuibb
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include "kernel.h"
#include "const.h"

extern GdtInfo gGdtInfo;
extern IdtInfo gIdtInfo;
extern void (* const RunTask)(Task* pt);

#endif
