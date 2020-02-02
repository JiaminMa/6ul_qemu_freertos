#include <stdint.h>
#include "bsp_int.h"
#include "bsp_epit.h"
#include <print.h>

void __assert_fail(uint32_t assert)
{

}


int main(void)
{
    print_init();
    
    trace("hello 6ul\n");
    int_init();                         /* 初始化中断 */
    epit1_init(0, 66000000/2);       /* 初始化epit1，周期为500ms */

    while(1);
    return 0;
}