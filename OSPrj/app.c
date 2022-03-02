/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-15 23:00:32
 * @LastEditTime: 2022-03-02 23:56:50
 * @LastEditors: Cuibb
 */

#include "shell.h"
#include "syscall.h"

void AppMain()
{
    RegApp("Shell", Shell, 255);
}

