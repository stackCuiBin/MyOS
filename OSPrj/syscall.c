/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-14 00:09:07
 * @LastEditTime: 2022-02-27 22:29:15
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

void Wait(const char* name)
{
    if (name) {
        SysCall(0, 1, name, 0);
    }
}

uint CreateMutex(uint type)
{
    volatile uint ret = 0;
    
    SysCall(1, 0, &ret, type); 
    
    return ret;
}

void EnterCritical(uint mutex)
{
    volatile uint wait = 0;
    
    do
    {
        SysCall(1, 1, mutex, &wait);
    } while( wait );
}

void ExitCritical(uint mutex)
{
    SysCall(1, 2, mutex, 0);
}

uint DestroyMutex(uint mutex)
{
    uint ret = 0;
    
    SysCall(1, 3, mutex, &ret);
    
    return ret;
}
