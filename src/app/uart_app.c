#include <app.h>
#include <fsl_uart.h>
#include <system_MCIMX6Y2.h>
#include <fsl_uart_freertos.h>

uint32_t test_flag;

void uart_poll_demo()
{
    // char *test_str = "uart_poll_demo\n";
    // char c;
    // uart_config_t uart0_config;
    // UART_GetDefaultConfig(&uart0_config);
    // uart0_config.baudRate_Bps = 115200;
    // uart0_config.enableTx = true;
    // uart0_config.enableRx = true;
    // UART_Init(UART1, &uart0_config, QEMU_CLK);

    // UART_WriteBlocking(UART1, test_str, strlen(test_str));
    // while(1) {
    //     UART_ReadBlocking(UART1, &c, 1);
    //     UART_WriteBlocking(UART1, &c, 1);
    // }
}


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

const char *to_send = "FreeRTOS UART driver example!\r\n";
const char *send_ring_overrun = "\r\nRing buffer overrun!\r\n";
const char *send_hardware_overrun = "\r\nHardware buffer overrun!\r\n";
uint8_t background_buffer[32];
uint8_t recv_buffer[4];

uart_rtos_handle_t handle;
struct _uart_handle t_handle;

uart_rtos_config_t uart_config = {
    .baudrate = 115200,
    .parity = kUART_ParityDisabled,
    .stopbits = kUART_OneStopBit,
    .buffer = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

void uart_task(void *pvParameters)
{
    int error;
    size_t n;

    uart_config.srcclk = DEMO_UART_CLK_FREQ;
    uart_config.base = DEMO_UART;

    if (0 > UART_RTOS_Init(&handle, &t_handle, &uart_config))
    {
        vTaskSuspend(NULL);
    }

    /* Send introduction message. */
    if (0 > UART_RTOS_Send(&handle, (uint8_t *)to_send, strlen(to_send)))
    {
        vTaskSuspend(NULL);
    }

    /* Receive user input and send it back to terminal. */
    do
    {
        error = UART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
        if (error == kStatus_UART_RxHardwareOverrun)
        {
            /* Notify about hardware buffer overrun */
            if (kStatus_Success !=
                UART_RTOS_Send(&handle, (uint8_t *)send_hardware_overrun, strlen(send_hardware_overrun)))
            {
                vTaskSuspend(NULL);
            }
        }
        if (error == kStatus_UART_RxRingBufferOverrun)
        {
            /* Notify about ring buffer overrun */
            if (kStatus_Success != UART_RTOS_Send(&handle, (uint8_t *)send_ring_overrun, strlen(send_ring_overrun)))
            {
                vTaskSuspend(NULL);
            }
        }
        if (n > 0)
        {
            /* send back the received data */
            UART_RTOS_Send(&handle, (uint8_t *)recv_buffer, n);
        }
    } while (kStatus_Success == error);

    UART_RTOS_Deinit(&handle);
    vTaskSuspend(NULL);
}
