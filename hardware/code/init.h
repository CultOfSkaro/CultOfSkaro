/******************************************************
 * init.h 
 * Spencer Fowers, 2010
 * Modified from Barrett Edward's SAA code
 */
#ifndef INIT_H
#define INIT_H

/*  Initialize the computing platform 
 *	1: Initialize CPU Caches
 *	2: Initialize CPU interrupts
 *  3: Initialize software systems
 *  4: Initialize Camera
 *  5: Enable user cores
 *  6: Enable CPU interrupts
 */
void Init();
void Init_Interrupts();
void Init_Camera();


#endif 
