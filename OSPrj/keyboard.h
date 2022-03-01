/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2022-03-01 23:50:13
 * @LastEditTime: 2022-03-02 00:52:34
 * @LastEditors: Cuibb
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "type.h"

void KeyboardModInit();
void PutScanCode(byte sc);
uint FetchKeyCode();

#endif // KEYBOARD_H