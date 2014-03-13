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

char steeringPinName[CONTROLPIN][STRINGLENGTH] = {"P9_11", "P9_12"};
char drivePinName[CONTROLPIN][STRINGLENGTH] = {"P9_13", "P9_14"};

void init();
void signalCatch(int);
void errMsg(char *);
void HandleTCPClient(int);
void endProcess();

CCData *ccData;
int carControlParam = 0;

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
	ccData = CCData_create(carControlParam, (char **)steeringPinName, CONTROLPIN, (char **)drivePinName, CONTROLPIN);
}

void signalCatch(int sig){
	if(sig == SIGINT){
		endProcess();
	}
	exit(EXIT_SUCCESS);
}

void endProcess(){
	CCData_close(ccData);
}

void errMsg(char *msg){
	printf("Error. %s\n", msg);
	
	exit(EXIT_FAILURE);
}
	