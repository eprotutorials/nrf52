#include <stdint.h>
#include "nrfx_ppi.h"
#include "nrfx_timer.h"
#include "nrfx_gpiote.h"
#include "app_error.h"

#define LED_PIN					18
#define TIMER_INTERVAL_MS		1000

static nrfx_timer_t timer1 = NRFX_TIMER_INSTANCE(1);
static nrf_ppi_channel_t ppi_channel;

int main(void)
{
	nrfx_gpiote_out_config_t gpiote_cfg = NRFX_GPIOTE_CONFIG_OUT_TASK_TOGGLE(false);

    APP_ERROR_CHECK(nrfx_gpiote_init());
    APP_ERROR_CHECK(nrfx_gpiote_out_init(LED_PIN, &gpiote_cfg));
	nrfx_gpiote_out_task_enable(LED_PIN);

    nrfx_timer_config_t timer_cfg = NRFX_TIMER_DEFAULT_CONFIG;
	timer_cfg.frequency = NRF_TIMER_FREQ_1MHz;
	timer_cfg.mode = NRF_TIMER_MODE_TIMER;
	timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
	timer_cfg.interrupt_priority = 6;
    APP_ERROR_CHECK(nrfx_timer_init(&timer1, &timer_cfg, NULL));
	nrfx_timer_extended_compare(&timer1, 
								NRF_TIMER_CC_CHANNEL0,
								nrfx_timer_ms_to_ticks(&timer1, TIMER_INTERVAL_MS),
								NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);

    APP_ERROR_CHECK(nrfx_ppi_channel_alloc(&ppi_channel));
    APP_ERROR_CHECK(nrfx_ppi_channel_assign(ppi_channel, 
									        nrfx_timer_event_address_get(&timer1, NRF_TIMER_EVENT_COMPARE0),
									        nrfx_gpiote_out_task_addr_get(LED_PIN)));
    APP_ERROR_CHECK(nrfx_ppi_channel_enable(ppi_channel));

    nrfx_timer_enable(&timer1);

    while (1);	// Do nothing
}

