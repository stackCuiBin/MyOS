/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:22:18
 * @LastEditTime: 2021-11-15 23:34:52
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

char* StrnCpy(char* dist, const char* src, int len)
{
    char* ret = dist;
    int i = 0;

    for (i = 0; src[i] && (i < len); i++) {
        dist[i] = src[i];
    }

    dist[i] = 0;

    return ret;
}
