#include <stdint.h>
#include <stdio.h>

void __assert_fail(unsigned int i)
{
    
}

void *memcpy (void *__restrict __dest, const void *__restrict __src,
             size_t __n)
{
    uint8_t *dst = (uint8_t *)__dest;
    uint8_t *src = (uint8_t *)__src;
    while(__n--) {
        *dst++ = *src++;
    }
    return dst;
}

void *memset (void *__s, int __c, size_t __n)
{
    uint8_t *dst = (uint8_t *)__s;
    while(__n--)
        *dst++ = __c;
    return dst;
}

uint32_t strlen(char *s)
{
    uint32_t len = 0;
    while(*s != 0) {
        len++;
        s++;
    }

    return len;
}