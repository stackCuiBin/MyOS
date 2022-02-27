/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:22:03
 * @LastEditTime: 2021-12-08 22:47:23
 * @LastEditors: Cuibb
 */

#ifndef UTILITY_H
#define UTILITY_H

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

void Delay(int n);
char* StrnCpy(char* dst, const char* src, int len);

#endif // UTILITY_H