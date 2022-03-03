/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2022-03-03 22:49:22
 * @LastEditTime: 2022-03-03 22:49:22
 * @LastEditors: Cuibb
 */

#include "sysinfo.h"

uint gMemSize = 0;



void SysInfoCallHandler(uint cmd, uint param1, uint param2)
{
    if( cmd == 0 )
    {
        uint* pRet = (uint*)param1;
        
        *pRet = gMemSize;
    }
}
