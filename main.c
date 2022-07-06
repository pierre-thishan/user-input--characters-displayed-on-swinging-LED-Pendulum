/*
 * Project-Displaying of 10 characters in a LED pendulum and which takes the input from the
 * Tera-term software which communicates with the microcontroller via UART and displays it on the fly
 * author: T.Warnakulnasooriya
 * date : 15.06.2022
 */

#include "inc/tm4c1294ncpdt.h"
#include <stdio.h>
#include <stdint.h>
#include "5x5_font.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "int_handler.h"

bool toggleLeft = false;
bool toggleInvert = false;
bool toggleRight = false;
bool DEBUG = false;

void configurePorts();
void configureTimer();
int calculateLoadValue();
void configUart0(void);
void wait();
void borderspacing(void);

unsigned char buffer[10] = { 0x52, 0x4C, 0x43, 0x53, 0x57, 0x45, 0x32, 0x51,
                             0x44, 0x4A };

int main(void)

{
    configurePorts();
    configureTimer();
    configUart0();

    int j, i, k;

    while (1)
    {   //printing left -> rigth with invert

        if ((toggleLeft) && (toggleInvert))
        { // detect left turning point and invert button 

            borderspacing();
            for (j = 0; j < 10; j++)
            {
                for (i = 0; i < 36; i++)
                {
                    if (buffer[j] == font[i][0])
                    {
                        for (k = 1; k < 7; k++)
                        {
                            GPIO_PORTM_DATA_R = ~font[i][k];
                            TIMER2_CTL_R |= 0x01;
                            while (!(TIMER2_RIS_R & 0x01))
                                ;                 // wait for timer event
                            TIMER2_ICR_R |= 0x01; // clear Timer2A capture event flag
                        }
                    }
                }
            }
            toggleLeft = false;

            borderspacing();
        }
        //printing left -> right without invert
        else if (toggleLeft && !(toggleInvert))
        {// detect left turning point only

            borderspacing();

            for (j = 0; j < 10; j++)
            {
                for (i = 0; i < 36; i++)
                {
                    if (buffer[j] == font[i][0])
                    {
                        for (k = 1; k < 7; k++)
                        {
                            GPIO_PORTM_DATA_R = font[i][k];
                            TIMER2_CTL_R |= 0x01;
                            while (!(TIMER2_RIS_R & 0x01))
                                ;                 // wait for timer event
                            TIMER2_ICR_R |= 0x01; // clear Timer2A capture event flag
                        }
                    }
                }
            }
            toggleLeft = false;
            borderspacing();
        }
/*
The code below is to make the pendulum print for  the right -> left side as well
but becuase the pendulum is not accurate and it has slightly different time periods
for the left to right edge and the right to left edge movement, the letters get scrambled 
and unclear,
In case you want to  print it in both directions in line 20 set DEBUG= true and also change
the code in line 178 to trigger  the interupt in both falling and rising edges.
( GPIO_PORTL_IBE_R |= 0x01;)
*/
       else if ((toggleRight) && (toggleInvert) && DEBUG)
       { // right -> left mit invert
           borderspacing();
           for (j = 9; j >= 0; j--)
           {
               for (i = 0; i < 36; i++)
               {
                   if (buffer[j] == font[i][0])
                   {
                       for (k = 6; k > 0; k--)
                       {
                           GPIO_PORTM_DATA_R = ~font[i][k];
                           TIMER2_CTL_R |= 0x01;
                           while (!(TIMER2_RIS_R & 0x01))
                               ;                 // wait for timer event
                           TIMER2_ICR_R |= 0x01; // clear Timer2A capture event flag
                       }
                   }
               }
           }
           toggleRight = false;
           borderspacing();

       }

       else if ((toggleRight) && !(toggleInvert) && DEBUG)
       { // right -> left ohne invert
           borderspacing();

           for (j = 9; j >= 0; j--)
           {
               for (i = 0; i < 36; i++)
               {

                   if (buffer[j] == font[i][0])
                   {
                       for (k = 6; k > 0; k--)
                       {
                           GPIO_PORTM_DATA_R = font[i][k];
                           TIMER2_CTL_R |= 0x01;
                           while (!(TIMER2_RIS_R & 0x01))
                               ;                 // wait for timer event
                           TIMER2_ICR_R |= 0x01; // clear Timer2A capture event flag
                       }
                   }
               }
           }
           toggleRight = false;
           borderspacing();
       }
    }
}

void configurePorts()
{
    // port M config for swinging LED
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;
    while ((SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R11) == 0)
        ; // wait for stable clock
    GPIO_PORTM_DEN_R = 0xFF; // enables 8 bits of port M
    GPIO_PORTM_DIR_R = 0xFF; // sets all 8 bits of M as output

    // port L
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10;
    while ((SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R10) == 0)
        ; // wait for stable clock
    GPIO_PORTL_DEN_R = 0x03;
    GPIO_PORTL_DIR_R = 0x00;

    // potLInterrupt

    GPIO_PORTL_IS_R &= ~(0x01); // edge sensitive
    GPIO_PORTL_IM_R &= ~0x01; // masking everything before configuring
    GPIO_PORTL_IBE_R |= 0x00; // when IBE_R is set to a specific pin, doesn't matter on what it is being set to that pin in IEV_R
    // when IBE_R set to 0 the event is handled by the IEV Port
    GPIO_PORTL_IEV_R |= 0x01; // sensitive to rising to pl(0)
    GPIO_PORTL_ICR_R |= 0x01;     // clearing at the beginning
    GPIO_PORTL_IM_R |= 0x01;      // unmask interrupt of this pin
    NVIC_EN1_R |= 1 << (53 - 32); // send to nvic interrupt number 53

    // port K
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R9;
    while ((SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R9) == 0)
        ; // wait for stable clock
    GPIO_PORTK_DEN_R = 0x01; // enable PK(0)
    GPIO_PORTK_DIR_R = 0x00; // input by default

    // port K interrupt
    GPIO_PORTK_IS_R &= ~(0x01);   // edge sensitive
    GPIO_PORTK_IM_R &= ~0x01;     // masking
    GPIO_PORTK_IBE_R |= 0x00; // when IBE_R is set toa specific pin, doesn't matter on whati sbeing set to that pin in IEV_R
    GPIO_PORTK_IEV_R |= 0x01;     // sensitive to rising to PK(0)
    GPIO_PORTK_ICR_R |= 0x01;     // clearing at the beginning
    GPIO_PORTK_IM_R |= 0x01;      // unmask interrupt of this pin
    NVIC_EN1_R |= 1 << (52 - 32); // send to nvic interrupt number 52

    // initialize Port A
    SYSCTL_RCGCGPIO_R |= (1 << 0); // switch on clock for Port A
    while ((SYSCTL_PRGPIO_R & (1 << 0)) == 0)
        ; // wait for stable clock
    GPIO_PORTA_AHB_DEN_R |= 0x03; // digital I/O enable for pin PA(1:0)
    GPIO_PORTA_AHB_AFSEL_R |= 0x03; // PA(1:0) set to alternate func
    GPIO_PORTA_AHB_PCTL_R |= 0x00000011; // alternate func is U0Rx+Tx

}

int calculateLoadValue()
{
    int load = 0;
    int mc_freq = 16e6;
    load = mc_freq * 1e-3; // load value for 1ms
    return load;
}

void configureTimer()
{
    int loadValue = 16e3;

    SYSCTL_RCGCTIMER_R |= (1 << 2); // timer2A
    while (!(SYSCTL_PRTIMER_R & (1 << 2)))
        ;
    TIMER2_CTL_R &= ~0x01; // disbale timer
    // 16 bit
    TIMER2_CFG_R = 0x04;
    // Match disabled,downwards direction and one-shot
    TIMER2_TAMR_R = 0x01;

    // TIMER2_TAPR_R = 0x00;
    // Load value 16000 to get time-out after 1ms (->1kHz duty cycle)
    TIMER2_TAILR_R = 8000 - 1;
}

void wait(int value)
{
    int counter;
    for (counter = 0; counter < value; counter++)
        ;
}
void configUart0(void)
{
// initialize UART0
    SYSCTL_RCGCUART_R |= (1 << 0); // switch on clock for UART0
    while ((SYSCTL_PRUART_R & (1 << 0)) == 0)
        ; // wait for stable clock
    UART0_CTL_R &= ~(1 << 0); // disable UART0 for config
// initialize bitrate of 9600 bit per second
    UART0_IBRD_R = 104; // set DIVINT of BRD floor(16MHz/16/9600Hz)
    UART0_FBRD_R = 11; // set DIVFRAC of BRD
    UART0_LCRH_R = 0x00000040; // serial format 7N1
    UART0_CTL_R |= 0x0301; // re-enable UART0

    //uart= interrupts
    UART0_ICR_R = 0x31FFF;
    UART0_IM_R = 1 << 4;
    NVIC_EN0_R = 1 << 5;

}
void borderspacing(void)
{// this fucntion is used to give border spacing in the right and left 
// corners of the LED pendulum. Because the speed is the lowest at the right and left 
// of a pendulum, the characters will look different in size (streched or compressed)
//In order to avoid this we try to print the chracters in the middle part of it
    int a = 0;
    for (a = 0; a < 30; a++)
    {
        if (toggleInvert)
        {
            GPIO_PORTM_DATA_R = ~0x00;
        }
        else
        {
            GPIO_PORTM_DATA_R = 0x00;
        }
        TIMER2_CTL_R |= 0x01;
        while (!(TIMER2_RIS_R & 0x01))
            ;                 // wait for timer event
        TIMER2_ICR_R |= 0x01; // clear Timer2A capture event flag
    }
}

