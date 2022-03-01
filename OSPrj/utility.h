/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:22:03
 * @LastEditTime: 2022-03-02 00:22:57
 * @LastEditors: Cuibb
 */

#ifndef UTILITY_H
#define UTILITY_H

#include "type.h"

#define Min(a, b)        ((a) < (b) ? (a) : (b))
#define Max(a, b)        ((a) > (b) ? (a) : (b))

#define AddrOff(a, i)    ((void*)((uint)(a) + (i) * sizeof(*(a))))

#define AddrIndex(a, b)  (((uint)(a) - (uint)(b)) / sizeof(*(a)))

#define IsEqual(a, b)               \
({                                  \
    unsigned ta = (unsigned)(a);    \
    unsigned tb = (unsigned)(b);    \
    !(ta - tb);                     \
})

#define OffsetOf(type, member)    ((unsigned)&(((type*)0)->member))

#define ContainerOf(ptr, type, member)                     \
({                                                       \
    const typeof(((type*)0)->member)* __mptr = (ptr);    \
    (type*)((char*)__mptr - OffsetOf(type, member));     \
})

#define CHECK_PTR(ptr)                                    

#define Dim(a)  (sizeof(a)/sizeof(*(a)))

void Delay(int n);
char* StrnCpy(char* dst, const char* src, uint len);
uint StrLen(const char* s);
int StrCmp(const char* left, const char* right, uint n);

#endif // UTILITY_H