/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:22:18
 * @LastEditTime: 2021-11-14 21:22:19
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
