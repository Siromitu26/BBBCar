#include "BBBCar.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXPENDING 5
#define CONTROLPIN 2
#define STRINGLENGTH 8
#define QUEUE_LIMIT 5



char *steeringPinName[STRINGLENGTH] = {"P9_11", "P9_12"};
char *drivePinName[STRINGLENGTH] = {"P9_13", "P9_14"};
int port = 12345;

void init(int, char **);
void signalCatch(int);
void endProcess();

CCData *ccData;
int carControlParam;
int carControlParamState;
int serverSock;
int clientSock;
socklen_t clientSock_len;
int yes = 1;
struct sockaddr_in addr;
struct sockaddr_in client;
int manualFlag;
int disconnectFlag;
int endFlag;

#ifdef DEBUG
int readState;
#endif

int main(int argc, char **argv){
#ifdef DEBUG
	printf("----------Debug Mode----------\n");
#endif

	init(argc, argv);
	
	ccData = CCData_create(carControlParam, (char **)steeringPinName, CONTROLPIN, (char **)drivePinName, CONTROLPIN);
	if(ccData == NULL){
#ifdef DEBUG
		printf("%s:%d ccData is NULL.\n", __func__, __LINE__);
#endif
		exit(EXIT_FAILURE);
	}
	if(manualFlag){
		int temp = 0;
		while(!endFlag){
			printf("SteeringParam:");
			scanf("%d", &temp);
			carControlParam = temp;
			printf("DriveParam:");
			scanf("%d", &temp);
			carControlParam += temp << 2;
			printf("endFlag:");
			scanf("%d", &endFlag);
			CCData_set(ccData, carControlParam);
			showCarControlStatus(ccData);
			carControl(ccData);
		}
		endProcess();
		return 0;
	}
	
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(serverSock < 0){
#ifdef DEBUG
		perror("socket");
#endif
		endProcess();
		exit(EXIT_FAILURE);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

	if(bind(serverSock, (struct sockaddr *)&addr, sizeof(addr)) != 0){
#ifdef DEBUG
		perror("bind");
#endif
		endProcess();
		exit(EXIT_FAILURE);
	}

	if(listen(serverSock, QUEUE_LIMIT) != 0){
#ifdef DEBUG
		perror("listen");
#endif
		endProcess();
		exit(EXIT_FAILURE);
	}

	while (!endFlag) {
		clientSock_len = sizeof(client);
#ifdef DEBUG
		printf("%s:%d Waiting for Connection...\n", __func__, __LINE__);
#endif
		clientSock = accept(serverSock, (struct sockaddr *)&client, &clientSock_len);
		if(clientSock < 0){
#ifdef DEBUG
			perror("accept");
#endif
			continue;
		}
		disconnectFlag = 0;
#ifdef DEBUG
		printf("%s:%d Accepted connection from %s, Port=%d\n", __func__, __LINE__, (char *)inet_ntoa(client.sin_addr), ntohs(client.sin_port));
#endif	
		while(!endFlag && !disconnectFlag){
			memset(&carControlParam, 0, sizeof(carControlParam));
#ifdef DEBUG 
			readState = read(clientSock, &carControlParam, sizeof(carControlParam));
			printf("%s:%d readState:%d\n", __func__, __LINE__, readState);
			if(readState <= 0){
#else
			if(read(clientSock, &carControlParam, sizeof(carControlParam)) <= 0){
#endif
#ifdef DEBUG
				perror("read");
#endif
				disconnectFlag = 1;
				carControlParam = 0;
			}
			CCData_set(ccData, carControlParam);
#ifdef DEBUG
			showCarControlStatus(ccData);
#endif
			carControl(ccData);
			endFlag = (carControlParam >> 24) & 1;
		}
		close(clientSock);
		carControlParam = 0;
	}
	close(serverSock);
	
	endProcess();
	
	return 0;
}	



void init(int paramArgc, char **paramArgv){
	
	carControlParam = 0;
	manualFlag = 0;
	disconnectFlag = 0;
	endFlag = 0;
	
	int c;
	while((c = getopt(paramArgc, paramArgv, "m")) != -1){
		switch(c){
			case 'm':
				manualFlag = 1;
				printf("----------Manual Mode----------\n");
			break;
			
			default:
				printf("Usage: %s [-m]\n", paramArgv[0]);
				exit(EXIT_SUCCESS);
			break;
		}
	}
	
	if(SIG_ERR == signal(SIGINT, signalCatch)){
#ifdef DEBUG
		printf("\nシグナルハンドラの登録に失敗しました\n");
#endif
		exit(EXIT_FAILURE);
	}
}



void signalCatch(int paramSig){

	printf("\n");
	if(paramSig == SIGINT){
		endProcess();
		printf("Ctrl+Cが入力されました\n");
		exit(EXIT_SUCCESS);
	}
}



void endProcess(){
	CCData_close(ccData);
	close(clientSock);
	close(serverSock);
}

	