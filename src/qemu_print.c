#include <print.h>

static SemaphoreHandle_t sPrintMutex;

typedef struct {
    volatile uint32_t uart_rx_reg;
    volatile uint32_t unused_rx_regs[15];
    volatile uint32_t uart_tx_reg;
    volatile uint32_t unused_tx_regs[15];
    volatile uint32_t uart_control_reg1;
    volatile uint32_t uart_control_reg2;
    volatile uint32_t uart_control_reg3;
    volatile uint32_t uart_control_reg4;
    volatile uint32_t uart_fifo_control_reg;
    volatile uint32_t uart_status_reg_1;
    volatile uint32_t uart_status_reg_2;
    volatile uint32_t uart_esc_char_reg;
    volatile uint32_t uart_esc_tim_reg;
    volatile uint32_t uart_brm_inc_block;
    volatile uint32_t uart_brm_mod_block;
    volatile uint32_t uart_baud_rate_count_reg;
    volatile uint32_t uart_one_milli_reg;
    volatile uint32_t uart_test_reg;
} imx_uart_t;

static imx_uart_t *imx_uart0;

char uart_putc(unsigned char ch)
{
    while((imx_uart0->uart_status_reg_2 & 0x00000008) == 0);
    imx_uart0->uart_tx_reg = ch;
    return ch;
}

bool print_init()
{
    bool ret = true;
    imx_uart0 = (imx_uart_t *)0x02020000;

    /* shutdown CSI*/
    imx_uart0->uart_control_reg2 &= ~0x04;
    imx_uart0->uart_control_reg2 &= ~0x02;

    /* reset controller */
    imx_uart0->uart_control_reg2 &= ~0x01;
    while((imx_uart0->uart_test_reg & 0x1) == 1);
    imx_uart0->uart_control_reg2 |= 0x1;
    /* Enable the UART module */
    imx_uart0->uart_control_reg1 |= 0x00000001;
    /* Set RXDMUXSEL of UCR3 */
    imx_uart0->uart_control_reg3 |= 0x00000004;

    imx_uart0->uart_fifo_control_reg |= (0x4 << 7);

    /* Set the baud rate */
    imx_uart0->uart_brm_inc_block |= 11;
    imx_uart0->uart_brm_mod_block |= 11;
    imx_uart0->uart_control_reg2 |= 0x04;
    imx_uart0->uart_control_reg2 |= 0x02;
    imx_uart0->uart_control_reg2 |= 0x00004000;
    /* Turn SCI port back on at new BAUD rate */
    imx_uart0->uart_control_reg2 &= ~0x00000100;
    imx_uart0->uart_control_reg2 &= ~0x00000040;
    imx_uart0->uart_control_reg2 &= ~0x00000020;
    
    sPrintMutex = xSemaphoreCreateMutex();
    if (sPrintMutex == NULL) {
        ret = false;
    } else {
        ret = true;
    }

    return ret;
}

char send_char(uint8_t *ch)
{
    char ret = 0;
    ret =  uart_putc(*ch);
    return ret;
}

const char hex_asc_table[16] =
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
const char upper_hex_asc_table[16] =
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

int is_dec_asc(char ch)
{
    uint32_t i;
    for (i = 0; i < 10; i++) {
        if (ch == hex_asc_table[i])
            return 1;
    }

    return 0;
}

int is_asc(char ch)
{
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

int is_hex_asc(char ch)
{
    return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f');
}

int printf_char(char ch)
{
    uint8_t c1 = (uint8_t)'\r';
    uint8_t c2 = 0;

    c2 = (uint8_t)ch;

    send_char(&c2);

    if (((uint8_t)'\n') == c2) {
        send_char(&c1);
    }

    return 0;
}

int printf_str(char *str)
{
    while (str && (*str != (char)'\0')) {
        printf_char(*str);
        str++;
    }

    return 0;
}

int printf_hex(uint32_t val, uint32_t width)
{
    int i = 0;
    char hex_val = 0, asc = 0;

    if ((width > 8) || (width == 0))
        width = 8;

    for (i = width - 1; i >= 0; i--) {
        hex_val = (val & (0x0F << (i << 2))) >> (i << 2);
        asc = hex_asc_table[(int)hex_val];
        printf_char(asc);
    }

    return 0;
}

int printf_hex_upper(uint32_t val, uint32_t width)
{
    int i = 0;
    char hex_val = 0, asc = 0;

    if ((width > 8) || (width == 0))
        width = 8;

    for (i = width - 1; i >= 0; i--) {
        hex_val = (val & (0x0F << (i << 2))) >> (i << 2);
        asc = upper_hex_asc_table[(int)hex_val];
        printf_char(asc);
    }

    return 0;
}

#if defined(MULTIPLY_DIVIDE_CAN_BE_USED)
int printf_dec(uint32_t val)
{
    uint8_t buf[16];
    char asc = 0;
    int i = 0;
    while (1) {
        buf[i] = val % 10;
        val = val / 10;
        i++;
        if (val == 0) {
            break;
        }
    }

    for (; i > 0; i--) {
        asc = hex_asc_table[buf[i - 1]];
        printf_char(asc);
    }

    return 0;
}
#else
const uint32_t hex_weight_value_table[] =
    { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };

int printf_dec(uint32_t val)
{
    uint32_t i = 0;
    uint32_t tmp = 1, tmp_w = 1;
    char asc = 0;

    /* Figure out the digitals */
    while (1) {
        tmp = (tmp << 3) + (tmp << 1);  // tmp *= 10;
        i++;
        if (tmp > val) {
            break;
        }
    }

    if (i > 8) {
        return -1;
    }

    while (i > 0) {
        if (val >= ((hex_weight_value_table[i - 1] << 3) + hex_weight_value_table[i - 1])) {    //<=9xxx
            tmp = 9;
            tmp_w = (hex_weight_value_table[i - 1] << 3) + hex_weight_value_table[i - 1];
        } else if (val >= (hex_weight_value_table[i - 1] << 3)) {   //8xxx
            tmp = 8;
            tmp_w = hex_weight_value_table[i - 1] << 3;
        } else if (val >=
                   ((hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1) +
                    hex_weight_value_table[i - 1])) {
            tmp = 7;
            tmp_w =
                (hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1) +
                hex_weight_value_table[i - 1];
        } else if (val >=
                   ((hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1))) {
            tmp = 6;
            tmp_w = (hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1);
        } else if (val >= ((hex_weight_value_table[i - 1] << 2) + hex_weight_value_table[i - 1])) {
            tmp = 5;
            tmp_w = (hex_weight_value_table[i - 1] << 2) + hex_weight_value_table[i - 1];
        } else if (val >= (hex_weight_value_table[i - 1] << 2)) {
            tmp = 4;
            tmp_w = hex_weight_value_table[i - 1] << 2;
        } else if (val >= ((hex_weight_value_table[i - 1] << 1) + hex_weight_value_table[i - 1])) {
            tmp = 3;
            tmp_w = (hex_weight_value_table[i - 1] << 1) + hex_weight_value_table[i - 1];
        } else if (val >= (hex_weight_value_table[i - 1] << 1)) {
            tmp = 2;
            tmp_w = hex_weight_value_table[i - 1] << 1;
        } else if (val >= (hex_weight_value_table[i - 1])) {
            tmp = 1;
            tmp_w = hex_weight_value_table[i - 1];
        } else {
            tmp = 0;
            tmp_w = 0;
        }

        asc = hex_asc_table[tmp];
        printf_char(asc);
        i--;

        val -= tmp_w;
    }

    return 0;
}
#endif

void trace(const char *fmt, ...)
{
    char c;
    uint32_t width = 0;
    va_list argptr;
    
    xSemaphoreTake(sPrintMutex, portMAX_DELAY);
    va_start(argptr, fmt);
    do {
        c = *fmt;
        if (c != '%') {
            printf_char(c);
        } else {
            while (1) {
                c = *++fmt;
                if ((c == 'd') || (c == 'x') || (c == 'X') || (c == 's') || (c == 'c')) {
                    if ((c == 'x') || (c == 'X')) {
                        if (*(fmt - 1) == '%')
                            width = 8;
                        else
                            width = *(fmt - 1) - '0';
                    }
                    break;
                }
            }

            switch (c) {
            case 'd':
                printf_dec(va_arg(argptr, int));
                break;
            case 'x':
                printf_hex((va_arg(argptr, int)), width);
                break;
            case 'X':
                printf_hex_upper((va_arg(argptr, int)), width);
                break;
            case 's':
                printf_str(va_arg(argptr, char *));
                break;
            case 'c':
                printf_char(va_arg(argptr, int));
                break;
            default:
                break;
            }
        }
        ++fmt;
    }
    while (*fmt != '\0');

    va_end(argptr);
    xSemaphoreGive(sPrintMutex);
}
