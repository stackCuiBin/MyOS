/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:22:18
 * @LastEditTime: 2022-02-28 00:51:07
 * @LastEditors: Cuibb
 */

#include "utility.h"

void Delay(int n)
{
    while( n > 0 )
    {
        int i = 0;
        int j = 0;
        
        for(i=0; i<1000; i++)
        {
            for(j=0; j<1000; j++)
            {
                asm volatile ("nop\n");
            }
        }
        
        n--;
    }
}

byte* MemCpy(byte* dst, const byte* src, uint n)
{
    byte* ret = dst;
    uint dAddr = (uint)dst;
    uint sAddr = (uint)src;
    int i = 0;

    if( dAddr < sAddr )
    {
        for(i=0; i<n; i++)
        {
            dst[i] = src[i];
        }
    }

    if( dAddr > sAddr )
    {
        for(i=n-1; i>=0; i--)
        {
            dst[i] = src[i];
        }
    }

    return ret;
}

byte* MemSet(byte* dst, uint n, byte val)
{
    byte* ret = dst;
    int i = 0;

    while( i < n )
    {
        dst[i++] = val;
    }

    return ret;
}

char* StrnCpy(char* dst, const char* src, uint len)
{
    char* ret = dst;
    int i = 0;

    for (i = 0; src[i] && (i < len); i++) {
        dst[i] = src[i];
    }

    dst[i] = 0;

    return ret;
}

uint StrLen(const char* s)
{
    uint ret = 0;

    while (s && s[ret]) ret++;

    return ret;
}

int StrCmp(const char* left, const char* right, uint n)
{
    int ret = 1;

    if (!IsEqual(left, right)) {
        uint lLen = StrLen(left);
        uint rLen = StrLen(right);
        uint m = Min(lLen, rLen);
        int i = 0;

        ret = IsEqual(lLen, rLen);
        n = Min(m, n);

        for (i = 0; (i < n) && ret; i++) {
            ret = IsEqual(left[i], right[i]);

            if (!ret) break;
        }
    }

    return ret;
}
