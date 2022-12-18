#include "driver/timer.h"

#include "timer.h"

#define SCT_GROUP TIMER_GROUP_0
#define SCT_TIMER TIMER_0

void timers_init()
{
    timer_config_t config = {
        .divider = 800,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = TIMER_AUTORELOAD_EN,
    };
    timer_init(SCT_GROUP, SCT_TIMER, &config);
    timer_set_counter_value(SCT_GROUP, SCT_TIMER, 0);
    timer_set_alarm_value(SCT_GROUP, SCT_TIMER, 0xFFFF);
    timer_start(SCT_GROUP, SCT_TIMER);
}

// return timer tick in 10us precise
u32 get_sys_tick()
{
    u64 timer_val;
    timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &timer_val);

    return (u32)timer_val;
}
