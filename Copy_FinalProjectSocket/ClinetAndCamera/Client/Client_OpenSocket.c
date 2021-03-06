// Client
// Client의 Socket을 열어준다.
// 완료

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>

void error_handling(char * msg);

// IP와 PORT 받아와서 소켓 열어 소켓 Return
int OpenSocket(char * I,char * po)
{
	int sock;
	struct sockaddr_in serv_addr;
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	// 주소 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(I);
	serv_addr.sin_port=htons(atoi(po));
	
	while(1){
		// 소켓을 이용해 서버의 정보를 지닌 구조체를 가지고 접속 요청을 한다
		if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
			sleep(30);
		}
		else{
			break;
		}
	}
	
	return sock;
}