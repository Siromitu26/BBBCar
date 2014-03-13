#include "BBBCar.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>

#define MAXPENDING 5
#define CONTROLPIN 2
#define STRINGLENGTH 8
#define QUEUE_LIMIT 5

char steeringPinName[CONTROLPIN][STRINGLENGTH] = {"P9_11", "P9_12"};
char drivePinName[CONTROLPIN][STRINGLENGTH] = {"P9_13", "P9_14"};
int port = 12345;

void init();
void showCarControlStatus(CCData *);
void printDecToBit(int, int);
int *serverStart(int, struct sockaddr_in *, size_t);
int *connection(int *, struct sockaddr_in *, size_t);
int receive(int *);
void signalCatch(int);
void endMsg(char *, int);
void endProcess();

CCData *ccData;
int carControlParam;
int carControlParamState;
int *serverSock;
int *clientSock;
struct sockaddr_in addr;
struct sockaddr_in client;
int endFlag;

int main()
{
	init();
	
	ccData = CCData_create(carControlParam, (char **)steeringPinName, CONTROLPIN, (char **)drivePinName, CONTROLPIN);
	
	serverSock = serverStart(port, &addr, sizeof(addr));

	while (!endFlag) {
		clientSock = connection(serverSock, &client, sizeof(client));
		while(!endFlag){
			carControlParam = receive(clientSock);
			CCData_set(ccData, carControlParam);
			showCarControlStatus(ccData);
			carControl(ccData);
		}
		close(*clientSock);
	}
	close(*serverSock);
	
	return 0;
}	

void init(){
	if(SIG_ERR == signal(SIGINT, signalCatch)){
		puts("シグナルハンドラの登録に失敗しました");
		exit(EXIT_FAILURE);
	}
	carControlParam = 0;
	endFlag = 0;
}

void showCarControlStatus(CCData *paramCCData){
	char steeringStatus[STRINGLENGTH];
	char driveStatus[STRINGLENGTH];
	int spwmPercent = (paramCCData->spwm) / 255;
	int dpwmPercent = (paramCCData->dpwm) / 255;
	
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
	printf("     2進数"); printDecToBit(paramCCData->carControlParam, sizeof(int) * 8);
	printf("                               \n");
	printf("                               \n");
	printf("操作データにデコードした結果         \n");
	printf("  ハンドル:%s                    \n", steeringStatus);
	printf("       PWM:%d%%                \n", spwmPercent);
	printf("                               \n");
	printf("  ドライブ:%s                     \n", driveStatus);
	printf("       PWM:%d%%                \n", dpwmPercent);	
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

int *serverStart(int paramPort, struct sockaddr_in *paramAddr, size_t paramSizeAddr){
	int socket0;
	int yes = 1;
	
	socket0 = socket(AF_INET, SOCK_STREAM, 0);

	paramAddr->sin_family = AF_INET;
	paramAddr->sin_port = htons(paramPort);
	(paramAddr->sin_addr).s_addr = INADDR_ANY;

	setsockopt(socket0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

	bind(socket0, (struct sockaddr *)paramAddr, paramSizeAddr);

	listen(socket0, QUEUE_LIMIT);
	
	return &socket0;
}

int *connection(int *paramServerSock, struct sockaddr_in *paramClient, size_t paramSizeClient){
	int sock;
	
	sock = accept(*paramServerSock, (struct sockaddr *)paramClient, &paramSizeClient);
	
	printf("Accepted connectionion from %s, Port=%d\n", (char *)inet_ntoa(paramClient->sin_addr), ntohs(paramClient->sin_port));
	
	return &sock;
}

int receive(int *paramSocket){
	int temp;
	memset(&temp, 0, sizeof(temp));
	if(recv(*paramSocket, &temp, sizeof(temp), 0) < 0){
		perror("read");
		endMsg(NULL, EXIT_FAILURE);
	}
	return temp;
}

void signalCatch(int paramSig){
	if(sig == SIGINT){
		endProcess();
		endMsg("Ctrl+Cが入力されました", EXIT_SUCCESS);
	}
	
}

void endProcess(){
	CCData_close(ccData);
}

void endMsg(char *paramMsg, int paramStatus{
	printf("%s\n", msg);
	exit(paramStatus);
}
	