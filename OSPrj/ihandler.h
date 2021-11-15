/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:32:18
 * @LastEditTime: 2021-11-15 17:44:16
 * @LastEditors: Cuibb
 */

#ifndef IHANDLER_H
#define IHANDLER_H

#define DeclHandler(name)    void name##Entry(); \
                             void name();
                             
DeclHandler(TimerHandler);

#endif // IHANDLER_H