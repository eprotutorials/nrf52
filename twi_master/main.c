#include <stdint.h>
#include <stdbool.h>
#include "nrf_drv_twi.h"
#include "app_error.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define TWI_ADDR		0x11
#define TWI_SDA_PIN		26
#define TWI_SCL_PIN		27
#define BUF_LEN			5

static const nrf_drv_twi_t twi0 = NRF_DRV_TWI_INSTANCE(0);
static volatile bool m_xfer_done = false;
static uint8_t m_buf[BUF_LEN];

void twi0_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            m_xfer_done = true;
            break;
        default:
            break;
    }
}


int main(void)
{
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
	NRF_LOG_DEFAULT_BACKENDS_INIT();

	NRF_LOG_INFO("TWI Master Example Started");

	const nrf_drv_twi_config_t twi0_config = {
       .scl                = TWI_SCL_PIN,
       .sda                = TWI_SDA_PIN,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    APP_ERROR_CHECK(nrf_drv_twi_init(&twi0, &twi0_config, twi0_handler, NULL));
    nrf_drv_twi_enable(&twi0);

	while (1)
    {
        m_xfer_done = false;

		do
        {
            nrf_drv_twi_rx(&twi0, TWI_ADDR, m_buf, BUF_LEN);
        } while (m_xfer_done == false);

		NRF_LOG_INFO("Data from TWI Slave");
		NRF_LOG_HEXDUMP_INFO(m_buf, BUF_LEN);

		nrf_delay_ms(1000);
    }
}