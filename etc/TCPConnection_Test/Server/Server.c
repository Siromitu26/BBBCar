#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
data

32 0-31bit
 4 0-3 基板信号
 8 4-12 左PWM
 8 13-21 右PWM
 1 22 LED
 1 23 LED
 1 24 LED
 1 25 LED
*/

int main(){
	int sock0;
	int sock;
	int yes = 1;
	int len;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	int data;
	unsigned char buffer[4];
	int i = 1;
	
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = INADDR_ANY;

	setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	listen(sock0, 5);

	while (1) {
		len = sizeof(client);
		sock = accept(sock0, (struct sockaddr *)&client, &len);

		printf("accepted connection from %s, port=%d\n", (char *)inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		
	//	send(sock, "HELLO", 5, 0);
		while(1){
			memset(&data, 0, sizeof(data));
			if(recv(sock, &data, sizeof(data), 0) < 0){
				perror("read");
				exit(EXIT_FAILURE);
			}
			light = data & 1;
			right = (data & 2) >> 1;
			lpwm = (data & (255 << 3)) >> 2;
			rpwm = (data & (255 << 3)) >> 12;
			i = 0;
			int led[4];
			while(i < 4){
				led[i] = 20
			}
			printf(	"------------------------------\n"
					"Count: %2d Received Data:%d\n"
					"Decode to Control Data\n", i++, data);
			printf( "   Light:%d\n"
					"   Right:%d\n"
					"   L PWM:%d\n"
					"   R PWM:%d\n", light, right, lpwm, rpwm);
			i = 0;
			while(i < 4){
			printf(	"   LED %d:%d\n", (i + 1), led[i]);
			}	
			printf(	"------------------------------\n");
		}
		close(sock);
	}

	close(sock0);
}