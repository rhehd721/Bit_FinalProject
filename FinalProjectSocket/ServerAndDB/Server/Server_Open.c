// Server
// Server의 Port를 열어주는 코드

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "./Server_AppendUser.h"

#define BUF_SIZE 100
#define MAX_CLNT 256
#define MAX_RASPBERRY 256

void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
void error_handling(char * msg);

// 서버에 접속한 클라이언트의 소켓 관리를 위한 변수와 배열
int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
// 서버에 접속한 클라이언트의 이름 리스트
char * (clnt_NameList[16])[MAX_CLNT];

// 서버에 접속한 라즈베리의 소켓 관리를 위한 변수와 배열
int raspberry_cnt=0;
int raspberry_socks[MAX_RASPBERRY];
// 서버에 접속한 라즈베리의 이름 리스트
char * (raspberry_NameList[16])[MAX_RASPBERRY];

pthread_mutex_t mutx;

int Server_Open(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
  
	pthread_mutex_init(&mutx, NULL);
	// 서버는 IPv4(PF_INET) 프로토콜을 사용할거고 연결지향형(SOCK_STREAM) 소켓을 사용할거야
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	while(1)
	{
		char * UnknownName;

		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		UnknownName = RecvName(clnt_sock);
		if(UnknownName[0] == 'R'){
			raspberry_NameList[raspberry_cnt] = UnknownName;
		}
		else{
			sprintf(clnt_NameList[clnt_cnt], "%s", UnknownName);
		}
		
		pthread_mutex_lock(&mutx);
		// 새로운 연결이 형성될 때마다 변수 clnt_cnt와 배열 clnt_sock에 해당 정보를 등록한다.
		clnt_socks[clnt_cnt++]=clnt_sock;
		pthread_mutex_unlock(&mutx);
	
		// 추가된 클라이언트에게 서비스를 제공하기 위한 쓰레드를 생성
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		// pthread_detach 함수호출을 통해서 종료된 쓰레드가 메모리에서 완전히 소멸되도록 하고 있다.
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}