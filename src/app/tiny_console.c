#include <app.h>
#include <fsl_uart.h>
#include <tiny_console.h>

TaskHandle_t g_print_task_handle = NULL;
TaskHandle_t g_control_task_handle = NULL;
TaskHandle_t g_console_task_handle = NULL;

static uint32_t flag = 0;

void print_task(void *pvParameters)
{
    while (1) {
        flag = 0x55;
        vTaskSuspend(g_print_task_handle);
        trace("\nprint stask resumed\n");
    }
}

tiny_console_t g_console; 

void console_task(void *pvParameters)
{
    trace("%s entry\n", __func__);
    uint32_t i = 0;
    uint8_t cmd_buf[ECHO_BUF_SIZE];

    trace("console:$");

    while(1) {

        while ((UART_GetStatusFlag(UART1, kUART_TxReadyFlag)) 
            && (g_console.wr_idx != g_console.rd_idx) && !g_console.is_delete
            && !g_console.is_carriage){
            UART_WriteByte(UART1, g_console.echo_buf[g_console.rd_idx]);
            g_console.rd_idx++;
            g_console.rd_idx %= ECHO_BUF_SIZE;
        }

        if (g_console.is_delete == true) {

            UART_WriteByte(UART1, 0xD);
            for (i = 0; i < ECHO_BUF_SIZE; i++) {
                UART_WriteByte(UART1, ' ');
            }
            UART_WriteByte(UART1, 0xD);
            trace("console:$");
            g_console.is_delete = false;
            for (i = 0; i < g_console.rd_idx; i++) {
                UART_WriteByte(UART1, g_console.echo_buf[i]);
            }
        }

        if (g_console.is_carriage == true) {
            memcpy(cmd_buf, g_console.echo_buf, ECHO_BUF_SIZE);
            if (cmd_buf[0] == 'l' && cmd_buf[1] == 's') {
                vTaskResume(g_print_task_handle);
            } else {
                trace("\ninvalid command\n");
            }
            g_console.wr_idx = 0;
            g_console.rd_idx = 0;
            g_console.is_carriage = false;
            trace("\nconsole:$");
        }
    }
}

static void UART1_IRQHandler(void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((UART_GetStatusFlag(UART1, kUART_RxDataReadyFlag)) 
        || (UART_GetStatusFlag(UART1, kUART_RxOverrunFlag))) {

        data = UART_ReadByte(UART1);

        if (((g_console.wr_idx + 1) % ECHO_BUF_SIZE) != g_console.wr_idx) {
            if (data == 0x7f) {
                g_console.echo_buf[g_console.wr_idx] = 0;
                if (g_console.rd_idx > 0)
                    g_console.rd_idx--;
                if (g_console.wr_idx > 0)
                    g_console.wr_idx--;
                g_console.is_delete = true;
            } else if (data == 0xd) {
                g_console.is_carriage = true;
            }else {
                g_console.echo_buf[g_console.wr_idx] = data;
                g_console.wr_idx++;
                g_console.wr_idx %= ECHO_BUF_SIZE;
            }
        }
    }
}

static void uart1_init()
{
    uart_config_t config;

    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200;
    config.enableTx = true;
    config.enableRx = true;
    UART_Init(UART1, &config, QEMU_CLK);
    
    SystemInstallIrqHandler(UART1_IRQn, 
        (system_irq_handler_t)UART1_IRQHandler, NULL);
    UART_EnableInterrupts(UART1, kUART_RxDataReadyEnable | kUART_RxOverrunEnable);
    EnableIRQ(UART1_IRQn);
   
}

void tiny_console_init()
{
    uint32_t i = 0;

    g_console.is_delete = false;
    g_console.is_carriage = false;
    g_console.rd_idx = 0;
    g_console.wr_idx = 0;

    for (i = 0; i < ECHO_BUF_SIZE; i++)
        g_console.echo_buf[i] = 0;

    uart1_init();
}