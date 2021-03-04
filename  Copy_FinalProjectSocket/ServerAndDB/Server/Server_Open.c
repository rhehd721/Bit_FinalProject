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
#include "/usr/include/mysql/mysql.h"
// #include <mysql.h>
#include "../DB/DB_Connect.h"
#include "./Server_Recv_From_Camera.h"

#define BUF_SIZE 100
#define MAX_CLNT 2

#define PORT "8080"

void * handle_clnt(void * arg);
void send_ToCamera(char * msg, int len);
void send_ToClient(char * msg, int len);

int * Client = 0;
int * Camera = 0;

// 서버에 접속한 클라이언트와 라즈베리의 idx를 관리하는 변수
int clnt_cnt=0;

// 서버에 접속한 클라이언트와 라즈베리들의 소켓번호를 저장하는 배열
int clnt_socks[MAX_CLNT];

pthread_mutex_t mutx;	// 쓰레드를 안정적으로 관리할 mutex

// MYSQL *connection=NULL

int Server_Open()
{
	// server open에 이용될 변수들
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;

	MYSQL *connection=NULL;	// 나중에 추가
  
	pthread_mutex_init(&mutx, NULL);	// mutex 초기화
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);	// 서버는 IPv4(PF_INET) 프로토콜을 사용할거고 연결지향형(SOCK_STREAM) 소켓을 사용할거야

	// 넘겨받은 PORT번호로 서버 OPEN
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(PORT));
	
	// bind와 listen을 통해 client받을준비 완료
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		return 0;
	if(listen(serv_sock, 5)==-1)
		return 0;
		
	// Server를 열었으니 DB또한 연결
	connection = DB_Connect();

	while(1)
	{

		// 접속을 요청한 Client, Rasberry 접속 허용 및 소켓 지정
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutx);

		clnt_socks[clnt_cnt] = clnt_sock;
		if (clnt_cnt == 0){Client = &clnt_sock;}
		else{Camera = &clnt_sock;}
		clnt_cnt++;

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

void * handle_clnt(void * arg)
{
	int clnt_sock=*((int*)arg);
	int str_len=0, i;
	char msg[BUF_SIZE];
	
	if (clnt_sock == *Client){	// 클라이언트 (즉, 받은거 카메라에게)
		while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
		send_ToCamera(msg, str_len);
	}
	else if (clnt_sock == *Camera){	// 카메라 (즉, 받은거 클라이언트에게)
		while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
		send_ToClient(msg, str_len);
	}
	else{
		return NULL;
	}
	
	// 위에 while문을 빠져나왔다면 연결이 없어진 것이니 소켓을 지워준다
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)   // remove disconnected client
	{
		if(clnt_sock==clnt_socks[i])
		{
			// 소켓을 지웠으므로 다시 리스트 정렬
			while(i++<clnt_cnt-1)
				clnt_socks[i]=clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx); 
	close(clnt_sock);
	return NULL;
}
// 연결된 모든 클라이언트에게 메세지를 전송하는 함수
void send_ToCamera(char * msg, int len)   // send to all
{
	printf("%s", msg);
	pthread_mutex_lock(&mutx);
	write(clnt_socks[1], msg, len);
	pthread_mutex_unlock(&mutx);
}

void send_ToClient(char * msg, int len)   // send to all
{
	pthread_mutex_lock(&mutx);
	write(clnt_socks[0], msg, len);
	pthread_mutex_unlock(&mutx);
}

void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
