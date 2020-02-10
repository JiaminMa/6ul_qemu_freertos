#include <app.h>
#include <fsl_uart.h>
#include <tiny_console.h>
#include <cmd.h>
#include <ff.h>

static tiny_console_t g_console;

static void uart1_irq_handler(void)
{
    /* If new data arrived. */
    if ((UART_GetStatusFlag(UART1, kUART_RxDataReadyFlag)) 
        || (UART_GetStatusFlag(UART1, kUART_RxOverrunFlag))) {

        g_console.data_from_uart = UART_ReadByte(UART1);
        xSemaphoreGiveFromISR(g_console.data_semaphore, NULL);
    }
}

char buf[256];
static void tiny_console_handle_cmd(char *cmd)
{
    char *argvs[10];
    uint32_t argc = 0;
    argvs[argc++] = strtok(cmd, " ");
    while (argvs[argc - 1] != NULL) {
        argvs[argc++] = strtok(NULL , " ");
    }

    trace("\n");
    if (true == strcmp(argvs[0], "ls")) {
        // vTaskResume(g_print_task_handle); 
        do_ls(argvs, argc);
    } else if (true == strcmp(argvs[0], "taskinfo")) {
        trace("===================================================");
        trace("\nTask Name\t\tStat\tPrio\tRStack\tTID\n");
        vTaskList(buf);
        trace("%s", buf);
    } else if (true == strcmp(argvs[0], "top")) {
        trace("===================================================");
        trace("\nTask Name\t\tRuntime\t\tCPU Rate\n");
        vTaskGetRunTimeStats(buf);
        trace("%s", buf);
    } else if (true == strcmp(argvs[0], "clear")) {
         /* do nothing */
    } else if (true == strcmp(argvs[0], "")) {
        /* do nothing */
    } else if (true == strcmp(argvs[0], "hello")) {
        do_hello();
    } else if (true == strcmp(argvs[0], "cat")) {
        do_cat(argvs, argc);
    } else if (true == strcmp(argvs[0], "mkdir")) {
        if (f_mkdir(argvs[1]) != FR_OK)  {
            trace("create folder failure\n");
        }
    } else {
        trace("invalid command\n");
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
    
    GIC_SetPriority(UART1_IRQn, 25);
    SystemInstallIrqHandler(UART1_IRQn, 
        (system_irq_handler_t)uart1_irq_handler, NULL);
    UART_EnableInterrupts(UART1, kUART_RxDataReadyEnable | kUART_RxOverrunEnable);
    EnableIRQ(UART1_IRQn);
}

static void tiny_console_task(void *pvParameters)
{
    trace("%s entry\n", __func__);
    uint32_t i = 0;
    uint8_t cmd_buf[ECHO_BUF_SIZE];
    uint8_t data;

    trace(TINY_CONSEL_PROMOPT);

    while(1) {

        xSemaphoreTake(g_console.data_semaphore, portMAX_DELAY);
        data = g_console.data_from_uart;

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

        while ((UART_GetStatusFlag(UART1, kUART_TxReadyFlag)) 
            && (g_console.wr_idx != g_console.rd_idx) 
            && !g_console.is_delete
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
            trace(TINY_CONSEL_PROMOPT);
            g_console.is_delete = false;
            for (i = 0; i < g_console.rd_idx; i++) {
                UART_WriteByte(UART1, g_console.echo_buf[i]);
            }
        }

        if (g_console.is_carriage == true) {
            g_console.echo_buf[g_console.wr_idx] = 0;
            memcpy(cmd_buf, g_console.echo_buf, ECHO_BUF_SIZE);
            tiny_console_handle_cmd((char *)cmd_buf);
            g_console.wr_idx = 0;
            g_console.rd_idx = 0;
            g_console.is_carriage = false;
            trace(TINY_CONSEL_PROMOPT);
        }
    }
}

void tiny_console_init()
{
    uint32_t i = 0;
    uart1_init();

    g_console.is_delete = false;
    g_console.is_carriage = false;
    g_console.rd_idx = 0;
    g_console.wr_idx = 0;

    for (i = 0; i < ECHO_BUF_SIZE; i++)
        g_console.echo_buf[i] = 0;

    g_console.data_semaphore = xSemaphoreCreateCounting(1, 0);
    if (g_console.data_semaphore == NULL) {
        while(1);
    }

    if (xTaskCreate(tiny_console_task, "init_console", 
        1024, NULL, configMAX_PRIORITIES, g_console.console_task_handle) 
                != pdPASS) {
        while(1);
    }
}