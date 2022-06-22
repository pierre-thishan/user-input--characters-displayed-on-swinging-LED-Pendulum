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
#define STEPS 1e2

//void wait(int value)
//{
//    int counter;
//    for (counter = 0; counter < value; counter++)
//        ;
//}
void IntPortLHandler(void) // IRQ number 53, vector number 69
{

    GPIO_PORTL_ICR_R |= 0x01;

    static int dirState=0;    // dirState=1; when right -> left
                             // dirState 00; when left -> right
//
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

//    GPIO_PORTD_AHB_DATA_R = 0xFF;
//    wait(100);
//    GPIO_PORTD_AHB_DATA_R = 0x00;

//    toggleLeft = true;
//    toggleRight = true;
}
