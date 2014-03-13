#include "BBBCar.h"
#include <BBB.h>
#include <BBB_GPIO.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void BBB_gpioArray_init(BBB_gpio **paramGpio, char **paramPinName, int paramPinNum){
	int i = 0;
	while(i < paramPinNum){
		if((paramGpio[i] = BBB_open_gpio(paramPinName[i])) == NULL ) {
			printf("Can not open %s\n", paramPinName[i] );
			BBB_gpioArray_close(paramGpio, i);
			exit(EXIT_FAILURE);
		}
		
		paramGpio[i]->set_direction(paramGpio[i], OUT);
		paramGpio[i]->put(paramGpio[i], 0);
		i++;
	}
}

void BBB_gpioArray_close(BBB_gpio **paramGpio, int paramPinNum){
	int i = 0;
	while(i < paramPinNum){
		BBB_close_gpio(paramGpio[i++]);
	}
}

CCData *CCData_create(int paramData, char **paramSteeringPinName, int paramSteeringPinNum, char **paramDrivePinName, int paramDrivePinNum){
	CCData *temp = (CCData *)malloc(sizeof(CCData));
	if(temp == NULL) return NULL;

	CCData_set(temp, paramData);
	BBB_gpioArray_init(temp->steeringGpio, paramSteeringPinName, paramSteeringPinNum);
	temp->steeringPinNum = paramSteeringPinNum;
	BBB_gpioArray_init(temp->driveGpio, paramDrivePinName, paramDrivePinNum);
	temp->drivePinNum = paramDrivePinNum;
	
	return temp;
}

void CCData_set(CCData *paramCCData, int paramData){
	if(paramCCData == NULL) return;
	
	int steering;
	int drive;
	int spwm;
	int dpwm;
	
	steering = paramData & 3;
	drive = (paramData >> 2) & 3;
	spwm = (paramData >> 4) & 255;
	dpwm = (paramData >> 12) & 255;
	
	paramCCData->carControlParam = paramData;
	paramCCData->steering = steering;
	paramCCData->spwm = spwm;
	paramCCData->drive = drive;
	paramCCData->dpwm = dpwm;
}

void CCData_close(CCData *paramCCData){
	if(paramCCData->steeringGpio != NULL){
		BBB_gpioArray_close(paramCCData->steeringGpio, paramCCData->steeringPinNum);
	}
	if(paramCCData->driveGpio != NULL){
		BBB_gpioArray_close(paramCCData->driveGpio, paramCCData->drivePinNum);
	}
	free(paramCCData);
}

void carControl(CCData *paramCCData){

	int s = paramCCData->steering;
	int ss = paramCCData->steeringState;
	int d = paramCCData->drive;
	int ds = paramCCData->driveState;
	
	/* 正転⇔逆転、正転or逆転⇔ブレーキ時を判定して100us待機 */
	int steeringWaitFlag = ((s < 3)&(ss < 3)&((s ^ ss) == 3)) | ((s > 0) & (ss == 3));
	int driveWaitFlag = ((d < 3)&(ds < 3)&((d ^ ds) == 3)) | ((d > 0) & (ds == 3));
	if(steeringWaitFlag | driveWaitFlag) usleep(100);
	
	motorDrive(paramCCData->steeringGpio, paramCCData->steering);
	motorDrive(paramCCData->driveGpio, paramCCData->drive);

	if(paramCCData->steering != NULL){
		paramCCData->steeringState = paramCCData->steering;
	}
	if(paramCCData->drive != NULL){
		paramCCData->driveState = paramCCData->drive;
	}
}

void motorDrive(BBB_gpio **paramGpio, int paramData){
/* **paramGpioはモーター制御用の信号ピンを制御するBBB_gpioの配列（要素は2つ）
	paramDataのビット値でモーターの制御をする
 		00 : フリー
 		01 : 正回転
 		10 : 逆回転
 		11 : ブレーキ		
*/
	switch(paramData){
		case 0:	//00
		paramGpio[0]->put(paramGpio[0], 0);
		paramGpio[1]->put(paramGpio[1], 0);
		break;

		case 1:	//01
		paramGpio[0]->put(paramGpio[0], 0);
		paramGpio[1]->put(paramGpio[1], 0);
		break;
	
		case 2:	//10
		paramGpio[0]->put(paramGpio[0], 0);
		paramGpio[1]->put(paramGpio[1], 0);
		break;

		case 3:	//11
		paramGpio[0]->put(paramGpio[0], 0);
		paramGpio[1]->put(paramGpio[1], 0);
		break;

		default:
		break;
	}
}



