/*!
 * @author  MJm
 * @date    2020/1/22
 */
#include <imx_uart.h>
#include <sd_card.h>
#include <imx_usdhc.h>
#include <ff.h>

static bool boot_freertos(void);
extern void jump_os(uint32_t);

void *memcpy (uint8_t *__dest, uint8_t *__src, uint32_t __n)
{
    uint8_t *dst = (uint8_t *)__dest;
    uint8_t *src = (uint8_t *)__src;
    while(__n--) {
        *dst++ = *src++;
    }
    return dst;
}

void c_entry()
{
    uart_init();

    printf("hello imx6ul bootloader\n");

    if (boot_freertos() == false) {
        printf("boot failure\n");
    }

    while(1)
        ;
}

static bool boot_freertos()
{
    bool boot_status = false;
    FATFS fs;
    FIL file;
    uint8_t buf[1024];
    uint8_t *dst = (uint8_t *)0x80000000;
    uint32_t len = 0;
    FRESULT res = 0;

    printf("%s entry\n", __func__);

    res = f_mount(&fs,"0:",1); 
    if (res != FR_OK) {
        printf("%s f_mount res:%d\n", __func__, res);
        boot_status = false;
        goto cleanup;
    }

    res = f_open(&file, "os.bin", FA_READ);
    if (res != FR_OK) {
        printf("%s f_open res:%d\n", __func__, res);
        boot_status = false;
        goto cleanup;
    }

    printf("%s start to read os.bin\n", __func__);
    do {
        res = f_read(&file, buf, 1024, (UINT *)&len);
        if (res != FR_OK) {
            printf("%s read failure\n", __func__);
            boot_status = false;
            goto cleanup;
        }
        // printf("%s dst:%x, len:%d\n", __func__, dst, len);
        memcpy(dst, buf, len);
        dst += 1024;
        
    } while (len != 0);
    printf("%s read os.bin done\n", __func__);
    f_close(&file);
    f_unmount("0:/");

    boot_status = true;
    jump_os(0x8000004c);

cleanup:
    return boot_status;
}