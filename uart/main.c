#include <stdint.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_uart.h"

void uart_error_handle(app_uart_evt_t * p_event)
{
    // Empty error handler
}

int main(void)
{
    uint32_t err_code;
	uint8_t byte;

    const app_uart_comm_params_t comm_params = { 8, 6, 7, 5, false, false, NRF_UART_BAUDRATE_115200 };
    APP_UART_FIFO_INIT(&comm_params, 256, 256, uart_error_handle, APP_IRQ_PRIORITY_LOWEST, err_code);
    APP_ERROR_CHECK(err_code);

    while (1)
    {
        while (app_uart_get(&byte) != NRF_SUCCESS);
        while (app_uart_put(byte) != NRF_SUCCESS);
    }
}