#include <BBB.h>
#include <BBB_GPIO.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#define CONTROLPIN 2
#define MAXPENDING 5

void errMsg(char *);
void HandleTCPClient(int);
void motorControl(BBB_gpio **paramGpio, int data);
void gpioInit(BBB_gpio **, int);

const char frontPinName[CONTROLPIN][] = {"P9_11", "P9_12"};
const char rearPinName[CONTROLPIN][] = {"P9_13", "P9_14"};

int main(int argc, char *argv[])
{
	BBB_gpio **frontGpio = gpioInit(frontPinName, CONTROLPIN);
	BBB_gpio *rearGpio = gpioInit(rearPinName, CONTROLPIN);

	
	int i;



	
	gpioClose(frontGpio, CONTROLPIN);
	gpioClose(rearGpio, CONTROLPIN);
	
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
BBB_gpio gpioInit(char **paramPinName, int paramPinNum){
	int i = 0;
	BBB_gpio *temp[paramPinNum];
	while(i < paramPinNum){
		if((temp[i] = BBB_open_gpio(paramPinName[i])) == NULL ) {
			printf("Can not open %s\n", paramPinName[i] );
			exit(EXIT_FAILURE);
		}
		temp[i]->set_direction(temp[i], OUT);
		temp[i]->put(temp[i], 0);
		i++;
	}
	return temp;
}
void gpioClose(BBB_gpio **paramGpio, int paramPinNum){
	int i = 0;
	white(i < paramPinNum){
		BBB_close_gpio(paramGpio[i++]);
	}
}
