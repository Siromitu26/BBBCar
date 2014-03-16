#ifndef _BBBCAR_H_
#include <BBB.h>
#include <BBB_GPIO.h>


typedef struct CarControlData_t{
	
	int carControlParam;
	
	int steering;
	int steeringState;//衝突電流対策
	BBB_gpio **steeringGpio;
	int steeringPinNum;
	int sPwm;
	
	int drive;
	int driveState;//衝突電流対策
	BBB_gpio **driveGpio;
	int drivePinNum;
	int dPwm;
	
} CCData;

void BBB_gpioArray_init(BBB_gpio **, char **, int);
void BBB_gpioArray_close(BBB_gpio **, int);
CCData *CCData_create(int, char **, int, char **, int);
void CCData_set(CCData *, int);
void CCData_close(CCData *);
void carControl(CCData *);
void motorDrive(BBB_gpio **, int);
void showCarControlStatus(CCData *);
void printDecToBit(int , int);

#define _BBBCAR_H_ 1
#endif
