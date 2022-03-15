#include <stdint.h>
#include "nrfx_timer.h"
#include "app_error.h"
#include "nrf_gpio.h"

#define LED_PIN             17
#define TIMER_INTERVAL_MS   500

const nrfx_timer_t timer0 = NRFX_TIMER_INSTANCE(0);

static void timer0_callback(nrf_timer_event_t event_type, void* p_context)
{
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            nrf_gpio_pin_toggle(LED_PIN);
            break;
        default:
            break;
    }
}

int main(void)
{
    uint32_t time_ticks;

    nrf_gpio_cfg_output(LED_PIN);
    nrf_gpio_pin_set(LED_PIN);

    nrfx_timer_config_t timer0_config = NRFX_TIMER_DEFAULT_CONFIG;
    APP_ERROR_CHECK(nrfx_timer_init(&timer0, &timer0_config, timer0_callback));

    time_ticks = nrfx_timer_ms_to_ticks(&timer0, TIMER_INTERVAL_MS);

    nrfx_timer_extended_compare(
         &timer0, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrfx_timer_enable(&timer0);

    while (1)
    {
        __WFI();
    }
}