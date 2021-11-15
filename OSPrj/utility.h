/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:22:03
 * @LastEditTime: 2021-11-14 21:46:24
 * @LastEditors: Cuibb
 */

#ifndef UTILITY_H
#define UTILITY_H

#define AddrOff(a, i)    ((void*)((uint)a + i * sizeof(*a)))

void Delay(int n);

#endif // UTILITY_H