#ifndef __TIMER_H__
#define __TIMER_H__
#include "types.h"

#define SYS_TIMER_RES 1000000 // 1Mhz, 1 tick = 1microsend
#define SYS_TIMER_TICKS_IN_MS 1000 // ticks of timer in one millisecond
#define SYS_TIMER_TICKS_IN_S 1000000 // ticks of timer in one second

void timers_init();
u32 get_sys_tick();


#endif // __TIMER_H__
