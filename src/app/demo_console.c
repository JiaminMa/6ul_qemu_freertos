#include <app.h>
#include <fsl_uart.h>

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

void control_task(void *pvParameters)
{
    uint32_t i = 0;

    PRINTF("\r\nDebug console Demo-scanf task.\r\n"); 
    while(1) {
        PRINTF("\r\n Please Input a interger together with press enter: \r\n");
        SCANF("%d", &i);
        if (i == 1) {
            PRINTF("%s resume print task\n", __func__);
            vTaskResume(g_print_task_handle);
        } else {
            PRINTF("%s not valid task index:%d\n", __func__, i);
        }

        PRINTF("flag:%d\n", flag);
        if (flag == 0x55) {
            flag = 0;
        }
    }
}

uint8_t g_echo_buf[ECHO_BUF_SIZE];
uint32_t g_echo_buf_widx = 0;
uint32_t g_echo_buf_ridx = 0;
bool is_clear_show = false;
bool is_cr = false;

void console_task(void *pvParameters)
{
    trace("%s entry\n", __func__);
    uint32_t i = 0;
    uint8_t cmd_buf[ECHO_BUF_SIZE];

    trace("console:$");

    while(1) {
        while ((UART_GetStatusFlag(UART1, kUART_TxReadyFlag)) 
            && (g_echo_buf_widx != g_echo_buf_ridx) && !is_clear_show
            && !is_cr){
            UART_WriteByte(UART1, g_echo_buf[g_echo_buf_ridx]);
            g_echo_buf_ridx++;
            g_echo_buf_ridx %= ECHO_BUF_SIZE;
        }

        if (is_clear_show == true) {

            UART_WriteByte(UART1, 0xD);
            for (i = 0; i < ECHO_BUF_SIZE; i++) {
                UART_WriteByte(UART1, ' ');
            }
            UART_WriteByte(UART1, 0xD);
            trace("console:$");
            is_clear_show = false;
            for (i = 0; i < g_echo_buf_ridx; i++) {
                UART_WriteByte(UART1, g_echo_buf[i]);
            }
        }

        if (is_cr == true) {
            memcpy(cmd_buf, g_echo_buf, ECHO_BUF_SIZE);
            if (cmd_buf[0] == 'l' && cmd_buf[1] == 's') {
                vTaskResume(g_print_task_handle);
            } else {
                trace("\ninvalid command\n");
            }
            g_echo_buf_ridx = 0;
            g_echo_buf_widx = 0;
            is_cr = false;
            trace("\nconsole:$");
        }
    }
}