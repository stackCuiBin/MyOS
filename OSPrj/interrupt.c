/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:21:30
 * @LastEditTime: 2021-11-15 21:33:54
 * @LastEditors: Cuibb
 */

#include "utility.h"
#include "interrupt.h"
#include "ihandler.h"

void (* const InitInterrupt)() = NULL;
void (* const EnableTimer)() = NULL;
void (* const SendEOI)(uint port) = NULL;

void IntModInit()
{
    SetIntHandler(AddrOff(gIdtInfo.entry, 0x20), (uint)TimerHandlerEntry);
    SetIntHandler(AddrOff(gIdtInfo.entry, 0x80), (uint)SysCallHandlerEntry);
    
    InitInterrupt();
}

int SetIntHandler(Gate* pGate, uint ifunc)
{
    int ret = 0;
    
    if( ret = (pGate != NULL) )
    {
        pGate->offset1  = ifunc & 0xFFFF;
        pGate->selector = GDT_CODE32_FLAT_SELECTOR;
        pGate->dcount   = 0;
        pGate->attr     = DA_386IGate + DA_DPL3;
        pGate->offset2  = (ifunc >> 16) & 0xFFFF;
    }
    
    return ret;
}

int GetIntHandler(Gate* pGate, uint* pIFunc)
{
    int ret = 0;
    
    if( ret = (pGate && pIFunc) )
    {
        *pIFunc = (pGate->offset2 << 16) | pGate->offset1;
    }
    
    return ret;
}
