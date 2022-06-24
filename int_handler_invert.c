/*
 * Project-Displaying of 10 characters in a LED pendulum and which takes the input from the
 * Tera-term software which communicates with the microcontroller via UART and displays it on the fly
 * author: T.Warnakulnasooriya
 * date : 15.06.2022
 * This file is used to handle the UART interrupt and the inversion of the font interrupt,
 */
#include <stdio.h>
#include <stdint.h>
#include "int_handler.h"
#include "inc/tm4c1294ncpdt.h" //this is the configuration file of  the circuit  Tiva C Series TM4C1294

void IntPortKHandler(void)
{
    static int state = 0;//  local satic variable saves the value of it and keeps it 
    // until you come back to the same function.
    // here we use it to give meaningful states

    GPIO_PORTK_ICR_R |= 0x01; // clearing interrFupt flag

    if (toggleInvert == 0 && state == 0)
    {
        toggleInvert = true; // set toggle true 1st time
        state = 1;           // set state to 1
    }
    else if (toggleInvert == 1 && state == 1)
    {
        toggleInvert = false; // revert the invert ,clear toggle bit
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

