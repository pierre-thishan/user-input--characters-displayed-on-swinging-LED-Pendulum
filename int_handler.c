/*
 * Project-Displaying of 10 characters in a LED pendulum and which takes the input from the
 * Tera-term software which communicates with the microcontroller via UART and displays it on the fly
 * author: T.Warnakulnasooriya
 * date : 15.06.2022
 * This file is used to handle the left and right edge detection interrupts
 */
#include <stdio.h>
#include <stdint.h>
#include "int_handler.h"
#include "inc/tm4c1294ncpdt.h" //this is the configuration file of  the circuit  Tiva C Series TM4C1294
#define STEPS 1e2

void IntPortLHandler(void) // IRQ number 53, vector number 69
{

    GPIO_PORTL_ICR_R |= 0x01;

    static int dirState=0;    // dirState=1; when right -> left
                             // dirState 00; when left -> right
    if (dirState == 0 )
    {
        toggleLeft = true; // set printing dir to left -> right
        dirState = 1;   // set dirState 1 for the next direction for the next interrupt
    }
    else if (dirState == 1)
    {
        toggleRight = true; // set printing dir to right -> left
        dirState = 0;    // set dirstate back to 0, for the next interrupt
    }
}
