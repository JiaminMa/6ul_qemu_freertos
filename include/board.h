#ifndef __BOARD_H
#define __BOARD_H

void board_epit1_init(unsigned int frac, unsigned int value);
void board_epit1_irqhandler(unsigned int gicciar, void *param);

#endif