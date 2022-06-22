/*
 * Lab 4 Display characters on the LED pendulum
 * @date: 07.06.2022
 * @author: T.Warnakulnasooriya
 * @date: 23.05.2022
 */
#include <stdio.h>
#include <stdint.h>
#include "int_handler.h"
#include "inc/tm4c1294ncpdt.h" //this is the configuration file of  the circuit  Tiva C Series TM4C1294

void IntPortKHandler(void)
{
    static int state = 0;

    GPIO_PORTK_ICR_R |= 0x01; // clearing interrFupt flag

    if (toggleInvert == 0 && state == 0)
    {
        toggleInvert = true; // set toggle true 1st time
        state = 1;           // set state to 1
    }
    else if (toggleInvert == 1 && state == 1)
    {
        toggleInvert = false; // revert thr invert ,clear toggle bit
        state = 0;        // set state back to 0
    }
}

void IntUART0Handler(void)
{
    static int i = 0;
    if (UART0_MIS_R & (1 << 4))
    {
        UART0_ICR_R |= (1 << 4);
        if (i < 10)
        {
            buffer[i] = UART0_DR_R;
            i++;
        }
        else if (i > 9)
        {
            i = 0;
            buffer[i] = UART0_DR_R;
            i++;
        }

    }
}

