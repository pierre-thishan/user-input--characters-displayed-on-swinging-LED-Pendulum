/*
 * HAW Hamburg IE
 * MCL_3
 * Task 2_int_handler.h
 * @author: T.Warnakulnasooriya, S.Park, S.Wijayaratna
 * @date: 23.05.2022
 */

#ifndef INT_HANDLER_H_
#define INT_HANDLER_H_
#include <stdbool.h>

void IntPortLHandler(void);
void IntPortKHandler(void);
void IntUART0Handler(void);

//void IntHandlerTimer0A(void);
extern bool toggleLeft;
extern bool toggleRight;
extern bool toggleInvert;
extern unsigned char buffer[10];
#endif /* INT_HANDLER_H_ */
