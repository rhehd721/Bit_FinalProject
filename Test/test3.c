// Client

// 중간 완성작

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void error_handling(char * msg);

char Command[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];

int main(char IP[],char port[],char Commands[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	// 접속한 Client의 이름 출력
	sprintf(Command, "[%s]", Commands);
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	// 주소 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(IP);
	serv_addr.sin_port=htons(atoi(port));
	
	// 소켓을 이용해 서버의 정보를 지닌 구조체를 가지고 접속 요청을 한다
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		error_handling("connect() error");
	}
	
	write(sock, Command, strlen(Command));

	close(sock);  
	return 0;
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}