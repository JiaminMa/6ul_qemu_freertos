#ifndef __TINY__CONSOLE_H__
#define __TINY__CONSOLE_H__

#include <app.h>

typedef struct tiny_console_tag{
    uint8_t echo_buf[ECHO_BUF_SIZE];
    uint32_t wr_idx;
    uint32_t rd_idx;
    bool is_delete;
    bool is_carriage;
    SemaphoreHandle_t data_semaphore;
} tiny_console_t;

extern void tiny_console_init(void);


#endif