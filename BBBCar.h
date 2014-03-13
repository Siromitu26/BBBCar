#ifndef _BBBCAR_H_
#include <BBB.h>
#include <BBB_GPIO.h>

void BBB_gpioArray_init(BBB_gpio **, char **, int);
void BBB_gpioArray_close(BBB_gpio **, int);
void motorControl(BBB_gpio **, int);

#define _BBBCAR_H_ 1
#endif
