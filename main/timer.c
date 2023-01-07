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
    .resolution_hz = SYS_TIMER_RES, // 1 MHZ, 1 tick = 1us
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

// return timer tick in 10us precise
u32 get_sys_tick()
{
    u64 timer_val;
    u32 converted_val;
    gptimer_get_raw_count(gptimer, &timer_val);
    converted_val = (u32)timer_val ;
    return converted_val;
}
