#include "BBBCar.h"
#include <BBB.h>
#include <BBB_GPIO.h>
#include <stdio.h>
#include <stdlib.h>

void BBB_gpioArray_init(BBB_gpio **paramGpio, char **paramPinName, int paramPinNum){
	int i = 0;
	while(i < paramPinNum){
		if((paramGpio[i] = BBB_open_gpio(paramPinName[i])) == NULL ) {
			printf("Can not open %s\n", paramPinName[i] );
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

void motorControl(BBB_gpio **paramGpio, int paramData){
/* *pinはモーター制御用の信号ピンを制御するBBB_gpioの配列（要素は2つ） */
/* dataのビット値でモーターの制御をする	*/
/* 		00 : フリー		*/
/* 		01 : 正回転		*/
/* 		10 : 逆回転		*/
/* 		11 : ブレーキ		*/
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
