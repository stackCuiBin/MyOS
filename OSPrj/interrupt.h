/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:21:13
 * @LastEditTime: 2021-11-14 21:43:10
 * @LastEditors: Cuibb
 */

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "kernel.h"

extern void (* const EnableTimer)();
extern void (* const SendEOI)(uint port);

void IntModInit();
int SetIntHandler(Gate* pGate, uint ifunc);
int GetIntHandler(Gate* pGate, uint* pIFunc);

#endif // INTERRUPT_H