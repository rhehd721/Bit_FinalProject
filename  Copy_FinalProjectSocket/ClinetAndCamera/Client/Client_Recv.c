// Client Recv
// Client의 경우 Image, Txt파일을 받는다.
// 매번 새로운 FileName 생성 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int Recv(int socket);
int RcvFlie(int socket, int Type, char * FileName);

int Recv(int socket){
	int serv_sock = socket;
	char *msg = NULL;
	int str_len = 0;

	while(1){
		// 일단 받을 파일이 어떤 파일인지 분류작업을 거친다.
		while((str_len=read(serv_sock, msg, sizeof(msg)))!=0){
				// 파일에 따라 Recv를 달리한다.
				if (msg == "Txt"){
					while((str_len=read(serv_sock, msg, sizeof(msg)))!=0)
					if (str_len != -1){
						RcvFlie(serv_sock, 0, );
					}
				}
				// image
				else if (msg == "image"){
					while((str_len=read(serv_sock, msg, sizeof(msg)))!=0)
					if (str_len != -1){
						RcvFlie(serv_sock, 1, );
					}
				}
				// Thread 탈출
				else{
					printf("Recv 종료");
					return -1;
				}
		}
	}
	return 0;
}

int RcvFlie(int socket, int Type, char * FileName)
{
	int clnt_sock;	// 클라이언트 소셋
	char buf[256];	// 받을 메세지

	int nbyte = 256;
    size_t filesize = 0, bufsize = 0;
    FILE *file = NULL;

	// Type : 0은 Txt, 1은 Image
	if (Type == 0){
		file = fopen(FileName, "wt");
	}
	else if (Type == 0){
		file = fopen(FileName, "wb");
	}
	else{
		printf("error");
		return -1;
	}
    
    bufsize = 256;

    while(/*filesize != 0*/nbyte!=0) {
 		//if(filesize < 256) bufsize = filesize;
        nbyte = recv(clnt_sock, buf, bufsize, 0);
		// int recv(int s, void *buf, size_t len, int flags);
		// int s	: 소켓 디스크립터
		// void *buf	: 수신할 버퍼 포인터 데이터
		// size_t len	: 버퍼의 바이트 단위 크기
		// int flags	: 아래와 같은 옵션을 사용할 수 있습니다.

        fwrite(buf, sizeof(char), nbyte, file);		
        //nbyte = 0;
    }
 
	// 파일과 소켓을 닫아준다
	fclose(file);
	return 0;
}