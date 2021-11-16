/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-11-14 21:32:18
 * @LastEditTime: 2021-11-15 21:31:59
 * @LastEditors: Cuibb
 */

#ifndef IHANDLER_H
#define IHANDLER_H

#define DeclHandler(name)    void name##Entry(); \
                             void name()

DeclHandler(SegmentFaultHandler);   
DeclHandler(PageFaultHandler);                             
DeclHandler(TimerHandler);
DeclHandler(SysCallHandler);

#endif // IHANDLER_H