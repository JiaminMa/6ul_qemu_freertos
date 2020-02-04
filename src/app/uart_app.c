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

void uart1_irq_handler(uint32_t gicciar, void *param)
{

}

void uart1_task(void *param)
{
    
    while(1) {
        vTaskDelay(1000);
    }
}

void uart1_init()
{
    uart_config_t config;
    char *test_str = "uart1_init\n";
    SystemInstallIrqHandler(UART1_IRQn, 
        (system_irq_handler_t)uart1_irq_handler, NULL);
    config.baudRate_Bps = 115200;
    config.enableRx = true;
    config.enableTx = true;
    UART_Init(UART1, &config, QEMU_CLK);
    UART_WriteBlocking(UART1, test_str, strlen(test_str));
    UART_EnableInterrupts(UART1, kUART_RxDataReadyEnable | kUART_RxOverrunEnable);
    EnableIRQ(UART1_IRQn);   
}