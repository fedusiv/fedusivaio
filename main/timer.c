#include "driver/gptimer.h"

#include "timer.h"

#define SCT_GROUP TIMER_GROUP_0
#define SCT_TIMER TIMER_0

static gptimer_handle_t gptimer;

void timers_init()
{
    gptimer_config_t timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT, // APB
    .direction = GPTIMER_COUNT_UP,
    .resolution_hz = 1 * 1000 * 1000, // 1MHz, 1 tick = 1us
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    // this is to limit timer to count until u32 max.
    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0, // counter will reload with 0 on alarm event
        .alarm_count = 0xFFFFFFFF, // 
        .flags.auto_reload_on_alarm = true, // enable auto-reload
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

// return timer tick in 10us precise
u32 get_sys_tick()
{
    u64 timer_val;
    gptimer_get_raw_count(gptimer, &timer_val);
    return (u32)timer_val;
}
