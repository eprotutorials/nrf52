#include <stdint.h>
#include "nrf_drv_twis.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define TWIS_ADDR		0x11
#define TWIS_SDA_PIN	26
#define TWIS_SCL_PIN	27
#define TX_BUF_LEN		5

static const nrf_drv_twis_t twis1 = NRF_DRV_TWIS_INSTANCE(1);
static uint8_t tx_buf[TX_BUF_LEN] = { 1, 2, 3, 4, 5 };

static void twis_event_handler(nrf_drv_twis_evt_t const * const p_event)
{
	switch (p_event->type)
	{
		case TWIS_EVT_READ_REQ:
			if (p_event->data.buf_req)
			{
				nrf_drv_twis_tx_prepare(&twis1, tx_buf, TX_BUF_LEN);
			}
			break;
		default:
			break;
	}
}

int main(void)
{
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
	NRF_LOG_DEFAULT_BACKENDS_INIT();

	NRF_LOG_INFO("TWIS Example Started");

	const nrf_drv_twis_config_t config =
    {
        .addr               = {TWIS_ADDR, 0},
        .scl                = TWIS_SCL_PIN,
        .scl_pull           = NRF_GPIO_PIN_PULLUP,
        .sda                = TWIS_SDA_PIN,
        .sda_pull           = NRF_GPIO_PIN_PULLUP,
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH
    };

	APP_ERROR_CHECK(nrf_drv_twis_init(&twis1, &config, twis_event_handler));
	nrf_drv_twis_enable(&twis1);

	while (1);
}