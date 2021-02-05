// Client

// test3의 추가 완성버전

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void * send_command(void * arg);
void * recv_file(void * arg);
void error_handling(char * msg);

char name[NAME_SIZE]="[DEFAULT]";
char command[BUF_SIZE];

int main(char * IP_NUM,char * PORT_NUM,char * USER_NAME)
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	
	// 접속한 Client의 이름 출력
	sprintf(name, "[%s]", USER_NAME);
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	// 주소 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(IP_NUM);
	serv_addr.sin_port=htons(atoi(PORT_NUM));
	
	// 소켓을 이용해 서버의 정보를 지닌 구조체를 가지고 접속 요청을 한다
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		error_handling("connect() error");
	}
	
	pthread_create(&snd_thread, NULL, send_command, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_file, (void*)&sock);

	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);

	close(sock);  
	return 0;
}

// command를 보내는 함수
void * send_command(void * arg)   // send thread main
{
	int sock=*((int*)arg);
	char name_command[NAME_SIZE+BUF_SIZE];
	while(1) 
	{
		fgets(command, BUF_SIZE, stdin);
		sprintf(name_command,"%s %s", name, command);
		write(sock, name_command, strlen(name_command));
	}
	return NULL;
}

// // 파일을 받는 함수
// void * recv_file(void * arg)   // read thread main
// {
// 	int sock=*((int*)arg);
// 	char name_msg[NAME_SIZE+BUF_SIZE];
// 	int str_len;
// 	while(1)
// 	{
// 		str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
// 		if(str_len==-1) 
// 			return (void*)-1;
// 		name_msg[str_len]=0;
// 		fputs(name_msg, stdout);
// 	}
// 	return NULL;
// }

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}