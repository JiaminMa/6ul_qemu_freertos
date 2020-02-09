#include <app.h>
#include <cmd.h>

static TaskHandle_t s_hello_main_handle;

static void hello_main()
{
    trace("hello cmd\n");
    vTaskDelete(s_hello_main_handle);
    while(1);
}

void do_hello()
{
    if (xTaskCreate(hello_main, "hello cmd", 128,
        NULL, configMAX_PRIORITIES - 1, &s_hello_main_handle) != pdPASS) {
        trace("%s: create hello main fail\n", __func__);
    }
}