#include <print.h>
#include <system.h>
#include <core_ca7.h>
#include <board.h>

// void board_epit1_init(unsigned int frac, unsigned int value)
// {
//     if( frac > 4095 )
//         frac = 4095;

//     EPIT1->CR = 0;

//     EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (frac << 4) | (1 << 24);

//     EPIT1->LR = value;
//     EPIT1->CMPR = 0;

//     GIC_EnableIRQ(EPIT1_IRQn);
//     system_register_irqhandler(EPIT1_IRQn, board_epit1_irqhandler, NULL);

//     EPIT1->CR |= 1 << 0;
// }

// void board_epit1_irqhandler(unsigned int gicciar, void *param)
// {
//     static unsigned char state = 0;

//     state = !state;
//     if(EPIT1->SR & (1 << 0)) {
//         trace("%s: state:%d\n", __func__, state);
//     }

//     EPIT1->SR |= (1 << 0);
// }