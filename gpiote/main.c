#include "nrfx_gpiote.h"
#include "app_error.h"
#define LED_PIN     17
#define BUTTON_PIN  13
static void in_pin_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    nrfx_gpiote_out_task_trigger(LED_PIN);
}
int main(void)
{
    APP_ERROR_CHECK(nrfx_gpiote_init());
    // Configure output pin
    nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);
    APP_ERROR_CHECK(nrfx_gpiote_out_init(LED_PIN, &out_config));
    nrfx_gpiote_out_task_enable(LED_PIN);
    // Configure input pin
    nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;
    APP_ERROR_CHECK(nrfx_gpiote_in_init(BUTTON_PIN, &in_config, in_pin_handler));
    nrfx_gpiote_in_event_enable(BUTTON_PIN, true);
   
    while (1)
    {
        __WFI();
    }
}