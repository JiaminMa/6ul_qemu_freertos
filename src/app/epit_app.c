#include <stdarg.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <stdbool.h>
#include <semphr.h>
#include <system.h>
#include <core_ca7.h>
#include <task.h>
#include <fsl_debug_console.h>

static bool s_is_epit_flag;
static uint32_t s_epit1_counter;

static void epit1_irqhandler(uint32_t gicciar, void *param)
{
    if(EPIT1->SR & (1 << 0)) {
        s_is_epit_flag = true;
        s_epit1_counter++;
    }
    EPIT1->SR |= (1 << 0);
}

static void epit1_init(uint32_t frac, unsigned int value)
{
    s_is_epit_flag = false;
    s_epit1_counter = 0;

    if( frac > 4095 )
        frac = 4095;

    EPIT1->CR = 0;

    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (frac << 4) | (1 << 24);

    EPIT1->LR = value;
    EPIT1->CMPR = 0;

    GIC_EnableIRQ(EPIT1_IRQn);
    SystemInstallIrqHandler(EPIT1_IRQn, epit1_irqhandler, NULL);

    EPIT1->CR |= 1 << 0;
}

void epit1_task(void *pvParamters)
{
    
    epit1_init(0, 66000000/2);
    PRINTF("%s is_epit_flag:%d\n", __func__, s_is_epit_flag);
    while(1) {
        while(s_is_epit_flag == false) {
            taskYIELD();
        }
        PRINTF("%s: %d\n", __func__, s_epit1_counter);
        s_is_epit_flag = false;
    }
}
