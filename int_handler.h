/*
 * Project-Displaying of 10 characters in a LED pendulum and which takes the input from the
 * Tera-term software which communicates with the microcontroller via UART and displays it on the fly
 * author: T.Warnakulnasooriya
 * date : 15.06.2022
 * This header file is used to handle link all the target files also for the global variables 
 */
#ifndef INT_HANDLER_H_
#define INT_HANDLER_H_
#include <stdbool.h>
//check the datsheet tmc1429
void IntPortLHandler(void);
void IntPortKHandler(void);
void IntUART0Handler(void);

extern bool toggleLeft;
extern bool toggleRight;
extern bool toggleInvert;
extern unsigned char buffer[10];
#endif /* INT_HANDLER_H_ */
