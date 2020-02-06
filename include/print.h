#ifndef __PRINT_H__
#define __PRINT_H__

#include <stdarg.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <stdbool.h>
#include <semphr.h>

extern void trace(const char *fmt, ...);
extern bool print_init(void);

#endif