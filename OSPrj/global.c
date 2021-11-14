/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-10 16:15:55
 * @LastEditTime: 2021-11-10 22:13:59
 * @LastEditors: Cuibb
 */

#include "global.h"

GdtInfo gGdtInfo = {0};
IdtInfo gIdtInfo = {0};
void (* const RunTask)(Task* pt) = NULL;
