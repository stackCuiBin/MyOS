/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2022-03-03 23:42:50
 * @LastEditTime: 2022-03-03 23:42:50
 * @LastEditors: Cuibb
 */

#ifndef HDRAW_H
#define HDRAW_H

#include "type.h"

#define SECT_SIZE    512

void HDRawModInit();
uint HDRawSectors();
uint HDRawWrite(uint si, byte* buf);
uint HDRawRead(uint si, byte* buf);

#endif // HDRAW_H