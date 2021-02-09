// Client

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define NAME_SIZE 20
void error_handling(char * msg);

char name[NAME_SIZE] = "[DEFAULT]";

void OpenSocket(char I[],char po[],char na[])
{
	int sock;
	struct sockaddr_in serv_addr;
	
	// 접속한 Client의 이름 출력
	sprintf(name, "[%s]", na);
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	// 주소 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(I);
	serv_addr.sin_port=htons(atoi(po));
	
	// 소켓을 이용해 서버의 정보를 지닌 구조체를 가지고 접속 요청을 한다
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		error_handling("connect() error");
	}
	
	return (void*)&sock, &name;
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}