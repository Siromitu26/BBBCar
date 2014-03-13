#include "BBBCar.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#define CONTROLPIN 2
#define MAXPENDING 5
#define STRINGLENGTH 8

char frontPinName[CONTROLPIN][STRINGLENGTH] = {"P9_11", "P9_12"};
char rearPinName[CONTROLPIN][STRINGLENGTH] = {"P9_13", "P9_14"};

void init();
void signalCatch(int);
void errMsg(char *);
void HandleTCPClient(int);
void endProcess();

BBB_gpio *frontGpio[CONTROLPIN];
BBB_gpio *rearGpio[CONTROLPIN];

int main()
{
	init();
	
	
	
	return 0;
}	

void init(){
	if(SIG_ERR == signal(SIGINT, signalCatch)){
		puts("シグナルハンドラの登録に失敗しました");
		exit(EXIT_FAILURE);
	}
	BBB_gpioArray_init(frontGpio, (char **)frontPinName, CONTROLPIN);
	BBB_gpioArray_init(rearGpio, (char **)rearPinName, CONTROLPIN);
}

void signalCatch(int sig){
	if(sig == SIGINT){
		endProcess();
	}
	exit(EXIT_SUCCESS);
}

void endProcess(){
	BBB_gpioArray_close(frontGpio, CONTROLPIN);
	BBB_gpioArray_close(rearGpio, CONTROLPIN);
}

void errMsg(char *msg){
	printf("Error. %s\n", msg);
	
	exit(EXIT_FAILURE);
}
	