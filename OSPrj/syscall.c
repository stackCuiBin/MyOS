/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-14 00:09:07
 * @LastEditTime: 2021-12-14 00:47:24
 * @LastEditors: Cuibb
 */

#include "syscall.h"

void Exit()
{
    // 当可选参数被省略，寄存器前使用单个%作为参数
    asm volatile(
        "movl  $0,  %eax \n"
        "int   $0x80    \n"
    );
}

uint CreateMutex()
{
    volatile uint ret = 0;

    // 当可选参数不被省略，寄存器前使用两个%%作为参数
    asm volatile(
        "movl  $1,  %%eax \n"  // type
        "movl  $0,  %%ebx \n"  // cmd
        "movl  %0,  %%ecx \n"  // param1
        "int   $0x80      \n"
        :
        : "r"(&ret)
        : "eax", "ebx", "ecx", "edx"
    );

    return ret;
}

void EnterCritical(uint mutex)
{
    asm volatile(
        "movl  $1,  %%eax \n"  // type
        "movl  $1,  %%ebx \n"  // cmd
        "movl  %0,  %%ecx \n"  // param1
        "int   $0x80      \n"
        :
        : "r"(mutex)
        : "eax", "ebx", "ecx", "edx"
    );
}

void ExitCritical(uint mutex)
{
    asm volatile(
        "movl  $1,  %%eax \n"  // type
        "movl  $2,  %%ebx \n"  // cmd
        "movl  %0,  %%ecx \n"  // param1
        "int   $0x80      \n"
        :
        : "r"(mutex)
        : "eax", "ebx", "ecx", "edx"
    );
}

void DestroyMutex(uint mutex)
{
    asm volatile(
        "movl  $1,  %%eax \n"  // type
        "movl  $3,  %%ebx \n"  // cmd
        "movl  %0,  %%ecx \n"  // param1
        "int   $0x80      \n"
        :
        : "r"(mutex)
        : "eax", "ebx", "ecx", "edx"
    );
}
