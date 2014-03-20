#include "BBBCar.h"
#include <BBB.h>
#include <BBB_GPIO.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define DEBUG_MODE 0

void BBB_gpioArray_init(BBB_gpio **paramGpio, char **paramPinName, int paramPinNum){
	if(DEBUG_MODE) return;
	paramGpio = (BBB_gpio **)malloc(sizeof(BBB_gpio *) * paramPinNum);
		if(paramGpio == NULL) puts("null0");

	int i = 0;
	int temp = 0;
	int flag = 0;
	
	while(i < paramPinNum){
		paramGpio[i] = BBB_open_gpio(paramPinName[i]);
		if(paramGpio[i] == NULL ) {
			printf("GPIO \"%s\" がオープンできませんでしたn", paramPinName[i] );
			BBB_gpioArray_close(paramGpio, i);
			exit(EXIT_FAILURE);
		} else {
			printf("%s opened.\n", paramPinName[i]);
		}
		paramGpio[i]->set_direction(paramGpio[i], OUT);
		paramGpio[i]->put(paramGpio[i], 0);
		
		i++;
	}
	printf("%d\n", i);
	
	i = 0;
	while(!flag){
			printf("testDrive:");
			scanf("%d", &temp);
			motorDrive(paramGpio, temp);
			printf("flag:");
			scanf("%d", &flag);
	}
	
	if(paramGpio == NULL) puts("nulle");
}

void BBB_gpioArray_close(BBB_gpio **paramGpio, int paramPinNum){
	if(DEBUG_MODE) return;
	int i = 0;
	while(i < paramPinNum){
		BBB_close_gpio(paramGpio[i++]);
	}
	free(paramGpio);
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
/* ParamData内訳
  0 -  1bit ハンドル
  2 -  3bit ドライブ
  4 - 12bit 左PWM
 13 - 21bit 右PWM
      22bit LED
      23bit LED
      24bit LED
      25bit LED
	  26bit EndFlag
*/
	if(paramCCData == NULL) return;

	paramCCData->carControlParam = paramData;
	paramCCData->steering = paramData & 3;
	paramCCData->sPwm = (paramData >> 4) & 255;
	paramCCData->drive = (paramData >> 2) & 3;
	paramCCData->dPwm = (paramData >> 12) & 255;
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
	
	/* 正転⇔逆転、正転or逆転⇔ブレーキ時を判定して100us待機 */
	int steeringWaitFlag = ((paramCCData->steering < 3)&(paramCCData->steeringState < 3)&((paramCCData->steering ^ paramCCData->steeringState) == 3)) | ((paramCCData->steering > 0) & (paramCCData->steeringState == 3));
	int driveWaitFlag = ((paramCCData->drive < 3)&(paramCCData->driveState < 3)&((paramCCData->drive ^ paramCCData->driveState) == 3)) | ((paramCCData->drive > 0) & (paramCCData->driveState == 3));
	if(steeringWaitFlag | driveWaitFlag) usleep(100);
	puts("test");
	motorDrive(paramCCData->steeringGpio, paramCCData->steering);		
	motorDrive(paramCCData->driveGpio, paramCCData->drive);

	paramCCData->steeringState = paramCCData->steering;
	paramCCData->driveState = paramCCData->drive;
}

void motorDrive(BBB_gpio **paramGpio, int paramData){
/* **paramGpioはモーター制御用の信号ピンを制御するBBB_gpioの配列（要素は2つ）
	paramDataのビット値でモーターの制御をする
 		00 : フリー
 		01 : 正回転
 		10 : 逆回転
 		11 : ブレーキ		
*/
	if(paramGpio == NULL) return;
	puts("test");
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

void showCarControlStatus(CCData *paramCCData){

	if(paramCCData == NULL){
		puts("CCDataはNULLです");
		return;
	}
	char steeringStatus[9];
	char driveStatus[9];
	
	switch(paramCCData->steering){
		case 0:
		strcpy(steeringStatus, "直進");
		break;
		
		case 1:
		strcpy(steeringStatus, "左折");
		break;
		
		case 2:
		strcpy(steeringStatus, "右折");
		break;
		
		default:
		strcpy(steeringStatus, "不明な値");
		break;
	}
	switch(paramCCData->drive){
		case 0:
		strcpy(driveStatus, "フリー");
		break;
		
		case 1:
		strcpy(driveStatus, "前進");
		break;
		
		case 2:
		strcpy(driveStatus, "後退");
		break;
		
		case 3:
		strcpy(driveStatus, "ブレーキ");
		break;
		
		default:
		strcpy(driveStatus, "不明な値");
		break;
	}
	
	printf("-------------------------------\n");
	printf("受信データ:%d                    \n", paramCCData->carControlParam);
	printf("     2進数"); printDecToBit(paramCCData->carControlParam, 32);
	printf("                               \n");
	printf("                               \n");
	printf("操作データにデコードした結果         \n");
	printf("  ハンドル:%s                    \n", steeringStatus);
	printf("       PWM:%d%%                \n", (paramCCData->sPwm * 100) / 255);
	printf("                               \n");
	printf("  ドライブ:%s                     \n", driveStatus);
	printf("       PWM:%d%%                \n", (paramCCData->dPwm * 100) / 255);	
	printf(	"------------------------------\n");
}

void printDecToBit(int paramDec, int paramLength){
	
	int i = 0;
	while(i < paramLength){
		if(paramDec & (1 << (paramLength - 1 - i))){
			putchar('1');
		} else {
			putchar('0');
		}
		i++;
	}
}


