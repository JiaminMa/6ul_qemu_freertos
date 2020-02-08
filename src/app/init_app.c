#include <ff.h>
#include <app.h>
#include <print.h>

static FATFS sdcard_fs;
TaskHandle_t g_init_task_handle;

static void init_file_system()
{
    FRESULT res = 0;
    res = f_mount(&sdcard_fs,"0:",1);
    trace("%s f_mount res:%d\n", __func__, res);
    if (res != FR_OK) {
        trace("init_file_system sdcard FATFS failure\n");
        while(1);
    }
}

void init_task(void *pvParameters)
{
    trace("%s entry\n", __func__);
    init_file_system();
    vTaskSuspend(g_init_task_handle);
    while(1);
}