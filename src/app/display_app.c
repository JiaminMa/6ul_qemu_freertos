#include <app.h>
#include <fsl_common.h>

TaskHandle_t g_display_task_handle;

typedef struct {
    uint32_t fb_addr;
    uint32_t xres;
    uint32_t yres;
    uint32_t bpp;
} simple_lcdif_t;

simple_lcdif_t *s_lcdif;

#define FRAME_BUFFER_ALIGN 64

static uint16_t s_framebuffer[500][300] __attribute__((aligned(64)));

static void fill_frame_buffer_single_color(uint16_t color)
{
    uint32_t i, j = 0;

    for (j = 0; j < 300; j++) {
        for (i = 0; i < 500; i++) {
            s_framebuffer[i][j] = color;
        }
    }
}

void display_task(void *pvParameter)
{
    uint32_t i = 0;
    s_lcdif = (simple_lcdif_t *)0x21c8000;
    uint16_t colors[16] = {
        0xFE41, 0x051d, 0x0000, 0x3a39,
        0xff80, 0xa254, 0x78ef, 0xc618,
        0x8802, 0xbbca, 0xe8c4, 0xfd79,
        0xfbe4, 0xb723, 0x2589, 0x9edd
    };
    trace("%s entry\n",__func__);

    while (1) {
        fill_frame_buffer_single_color(colors[i++]);
        if (i > 16) {
            i = 0;
        }
        s_lcdif->fb_addr = (uint32_t)s_framebuffer;
        s_lcdif->xres = 500;
        s_lcdif->yres = 300;
        s_lcdif->bpp = 16;
        vTaskDelay(200);
    }
}