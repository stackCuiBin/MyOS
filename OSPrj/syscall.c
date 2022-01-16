/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-14 00:09:07
 * @LastEditTime: 2021-12-14 19:17:56
 * @LastEditors: Cuibb
 */

#include "syscall.h"

// 当可选参数被省略，寄存器前使用单个%作为参数
// 当可选参数不被省略，寄存器前使用两个%%作为参数
#define SysCall(type, cmd, param1, param2)              \
                asm volatile(                           \
                    "movl  $" #type ",  %%eax \n"       \ 
                    "movl  $" #cmd ",   %%ebx \n"       \
                    "movl  %0,          %%ecx \n"       \
                    "movl  %1,          %%edx \n"       \
                    "int   $0x80      \n"               \
                    :                                   \
                    : "r"(param1), "r"(param2)          \                   
                    : "eax", "ebx", "ecx", "edx"        \
                );

void Exit()
{
    SysCall(0, 0, 0, 0);
}

uint CreateMutex()
{
    volatile uint ret = 0;

    SysCall(1, 0, &ret, 0);

    return ret;
}

void EnterCritical(uint mutex)
{
    volatile uint wait = 0;

    do {
        SysCall(1, 1, mutex, &wait);
    } while( wait != 1 );
}

void ExitCritical(uint mutex)
{
    SysCall(1, 2, mutex, 0);
}

void DestroyMutex(uint mutex)
{
    SysCall(1, 3, mutex, 0);
}
