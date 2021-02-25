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

// 서버에 접속한 클라이언트와 라즈베리의 idx를 관리하는 변수
int raspberry_cnt=0;
int clnt_cnt=0;

// 서버에 접속한 클라이언트와 라즈베리들의 소켓번호를 저장하는 배열
int raspberry_socks[MAX_RASPBERRY];
int clnt_socks[MAX_CLNT];

// 서버에 접속한 클라이언트와 라즈베리의 이름을 저장하는 포인터 배열
char* clnt_NameList [MAX_CLNT];
char* raspberry_NameList [MAX_RASPBERRY];

pthread_mutex_t mutx;	// 쓰레드를 안정적으로 관리할 mutex

int Server_Open(char * IP, char * PORT)
{
	// server open에 이용될 변수들
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
  
	pthread_mutex_init(&mutx, NULL);	// mutex 초기화
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);	// 서버는 IPv4(PF_INET) 프로토콜을 사용할거고 연결지향형(SOCK_STREAM) 소켓을 사용할거야

	// 서버 IP와 PORT번호로 서버 OPEN
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(IP);
	serv_adr.sin_port=htons(atoi(PORT));
	
	// bind와 listen을 통해 client받을준비 완료
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		return 0;
	if(listen(serv_sock, 5)==-1)
		return 0;
		
	while(1)
	{
		char * UnknownName; // Client, Raspberry이름을 받을 변수

		// 접속을 요청한 Client, Rasberry 접속 허용 및 소켓 지정
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		UnknownName = RecvName(clnt_sock);	// RecvName함수를 통해 접속자 이름 확인

		pthread_mutex_lock(&mutx);
		if(UnknownName[0] == 'R'){	// 만약 받아온 이름이 R로 시작하는 문자열이라면
			raspberry_NameList[raspberry_cnt] = UnknownName;
			raspberry_socks[raspberry_cnt] = clnt_sock;
			raspberry_cnt++;
		}
		else{
			clnt_NameList[clnt_cnt] = UnknownName;
			clnt_socks[clnt_cnt] = clnt_sock;
			clnt_cnt++;
		}
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