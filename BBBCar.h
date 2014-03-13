#ifndef _BBBCAR_H_
#include <BBB.h>
#include <BBB_GPIO.h>


typedef struct CarControlData_t{
	
	int steering;
	BBB_gpio **steeringGpio;
	int spwm;
	
	int drive;
	BBB_gpio **driveGpio;
	int dpwm;
	
} CCData;

void BBB_gpioArray_init(BBB_gpio **, char **, int);
void BBB_gpioArray_close(BBB_gpio **, int);
CCData *CCData_create(int, BBB_gpio **, BBB_gpio **);
void CCData_set(CCData *, int);
void CCData_close(CCData *);
void carControl(CCData *);
void motorDrive(BBB_gpio **, int);


#define _BBBCAR_H_ 1
#endif
