#include <stdint.h>
#include "nrfx_timer.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#define LED_1	            17
#define LED_2				18
#define LED_3				19
#define LED_4				20
#define BUTTON_1			13
#define TIMER_TICKS			6

#define ON					0
#define OFF					1

const nrfx_timer_t timer0 = NRFX_TIMER_INSTANCE(0);

static void led_config(uint8_t led, uint8_t state)
{
	if (state == ON)
	   nrf_gpio_pin_clear(led);
	
	if (state == OFF)
	   nrf_gpio_pin_set(led);
}

static void led_init(void)
{
	nrf_gpio_cfg_output(LED_1);
	nrf_gpio_cfg_output(LED_2);
	nrf_gpio_cfg_output(LED_3);
	nrf_gpio_cfg_output(LED_4);
	led_config(LED_1, OFF);
	led_config(LED_2, OFF);
	led_config(LED_3, OFF);
	led_config(LED_4, OFF);
}

static void button_init(void)
{
	nrf_gpio_cfg_input(BUTTON_1, NRF_GPIO_PIN_PULLUP);
}

static void timer0_init(void)
{
	nrfx_timer_config_t timer0_config = NRFX_TIMER_DEFAULT_CONFIG;
	timer0_config.mode = NRF_TIMER_MODE_COUNTER;
    APP_ERROR_CHECK(nrfx_timer_init(&timer0, &timer0_config, NULL));
	nrfx_timer_extended_compare(
         &timer0, NRF_TIMER_CC_CHANNEL0, TIMER_TICKS, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);
}

static void display_led(uint32_t timer_value)
{
	switch (timer_value)
	{
		case 1:
			led_config(LED_1, ON);
			led_config(LED_2, OFF);
			led_config(LED_3, OFF);
			led_config(LED_4, OFF);
			break;
		case 2:
			led_config(LED_1, OFF);
			led_config(LED_2, ON);
			led_config(LED_3, OFF);
			led_config(LED_4, OFF);
			break;
		case 3:
			led_config(LED_1, ON);
			led_config(LED_2, ON);
			led_config(LED_3, OFF);
			led_config(LED_4, OFF);
			break;
		case 4:
			led_config(LED_1, OFF);
			led_config(LED_2, OFF);
			led_config(LED_3, ON);
			led_config(LED_4, OFF);
			break;
		case 5:
			led_config(LED_1, ON);
			led_config(LED_2, OFF);
			led_config(LED_3, ON);
			led_config(LED_4, OFF);
			break;
		default:
			led_config(LED_1, OFF);
			led_config(LED_2, OFF);
			led_config(LED_3, OFF);
			led_config(LED_4, OFF);
			break;
	}
}

int main(void)
{
	uint32_t timer_value = 0;

    led_init();
	button_init();

	timer0_init();
    
	nrfx_timer_enable(&timer0);

    while (1)
    {
        if (nrf_gpio_pin_read(BUTTON_1) == 0) // Button 1 is pressed
		{
			// Trigger COUNT task
			nrfx_timer_increment(&timer0);

			timer_value = nrfx_timer_capture(&timer0, NRF_TIMER_CC_CHANNEL1);
			display_led(timer_value);

			nrf_delay_ms(500);
		}
    }

	return 0;
}