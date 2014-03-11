#include <BBB.h>
#include <BBB_GPIO.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#define PIN 4
#define MAXPENDING 5

void errMsg(char *);
void HandleTCPClient(int);
void motorControl(BBB_gpio **pin, int data);
void gpioInit(BBB_gpio **, int);

int main(int argc, char *argv[])
{
	char pin_name[4][8] = {"P9_11", "P9_12", "P9_13", "P9_14"};
	BBB_gpio *gpio[PIN];
	BBB_gpio *front[2];
	BBB_gpio *rear[2];

	
	int i;

	for(i = 0; i < 4; i++){
		gpio[i] = BBB_open_gpio(pin_name[i]);
		if( gpio[i] == NULL ) {
			printf("Can not open %s\n", pin_name[i] );
			return 0;
		}
		gpio[i]->set_direction(gpio[i], OUT);
		gpio[i]->put(gpio[i], 0);
		if(i < 2){
			front[i] = gpio[i];
		} else {
			rear[i - 2] = gpio[i];
		}
	}

	
	
	for(i = 0; i < 4; i++){
		BBB_close_gpio(gpio[i]);
	}
}	

void motorControl(BBB_gpio **pin, int data){
/* *pinはモーター制御用の信号ピンを制御するBBB_gpioの配列（要素は2つ） */
/* dataのビット値でモーターの制御をする	*/
/* 		00 : フリー		*/
/* 		01 : 正回転		*/
/* 		10 : 逆回転		*/
/* 		11 : ブレーキ		*/
	switch(data){
		case 0:	//00
		pin[0]->put(pin[0], 0);
		pin[1]->put(pin[1], 0);
		break;

		case 1:	//01
		pin[0]->put(pin[0], 0);
		pin[1]->put(pin[1], 0);
		break;
	
		case 2:	//10
		pin[0]->put(pin[0], 0);
		pin[1]->put(pin[1], 0);
		break;

		case 3:	//11
		pin[0]->put(pin[0], 0);
		pin[1]->put(pin[1], 0);
		break;

		default:
		break;
	}
}
