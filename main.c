#include "BBBCar.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXPENDING 5
#define CONTROLPIN 2
#define STRINGLENGTH 8
#define QUEUE_LIMIT 5


char steeringPinName[CONTROLPIN][STRINGLENGTH] = {"P9_11", "P9_12"};
char drivePinName[CONTROLPIN][STRINGLENGTH] = {"P9_13", "P9_14"};
int port = 12345;

void init();
void signalCatch(int);
void endMsg(char *, int);
void endProcess();

CCData *ccData;
int carControlParam;
int carControlParamState;
int serverSock;
int clientSock;
int clientLength;
int yes = 1;
struct sockaddr_in addr;
struct sockaddr_in client;
int endFlag;

int main()
{
	init();
	
	ccData = CCData_create(carControlParam, (char **)steeringPinName, CONTROLPIN, (char **)drivePinName, CONTROLPIN);
	
	serverSock = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

	bind(serverSock, (struct sockaddr *)&addr, sizeof(addr));

	listen(serverSock, QUEUE_LIMIT);

	while (!endFlag) {
		clientLength = sizeof(client);
		printf("Connecting...\n");
		clientSock = accept(serverSock, (struct sockaddr *)&client, &clientLength);
		printf("Accepted connectionion from %s, Port=%d\n", (char *)inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	
		while(!endFlag){
			memset(&carControlParam, 0, sizeof(carControlParam));
			if(recv(clientSock, &carControlParam, sizeof(carControlParam), 0) < 0){
				perror("read");
				endMsg(NULL, EXIT_FAILURE);
			}
			CCData_set(ccData, carControlParam);
			showCarControlStatus(ccData);
			carControl(ccData);
			endFlag = (carControlParam >> 24) & 1;
		}
		close(clientSock);
	}
	close(serverSock);
	
	endProcess();
	
	return 0;
}	

void init(){
	
	if(SIG_ERR == signal(SIGINT, signalCatch)){
		endMsg("\nシグナルハンドラの登録に失敗しました", EXIT_FAILURE);
	}
	carControlParam = 0;
	endFlag = 0;
}

void signalCatch(int paramSig){
	if(paramSig == SIGINT){
		endProcess();
		endMsg("Ctrl+Cが入力されました", EXIT_SUCCESS);
	}
}

void endProcess(){
	CCData_close(ccData);
	close(clientSock);
	close(serverSock);
}

void endMsg(char *paramMsg, int paramStatus){
	printf("%s\n", paramMsg);
	exit(paramStatus);
}
	