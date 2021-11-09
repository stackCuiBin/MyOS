/*
 * @Descripttion: 
 * @version: V0.01
 * @Author: Cuibb
 * @Date: 2021-11-07 21:40:45
 * @LastEditors: Cuibb
 * @LastEditTime: 2021-11-09 11:36:34
 */
#include "kernel.h"
#include "screen.h"

void KMain()
{
    int n = PrintString("HELLO D.T.OS\n");
    
    n = PrintString("Nice\n");
    
    PrintIntDec(n);

}
