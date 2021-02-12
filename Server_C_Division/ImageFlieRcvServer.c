/* server.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int RcvFlie(int argc, char *argv[])
{
	int serv_sock;	// 서버소켓
	int clnt_sock;	// 클라이언트 소셋
	char buf[256];	// 받을 메세지

	struct sockaddr_in serv_addr;	// 서버 소켓 정보를 저장할 구조체
	struct sockaddr_in clnt_addr;	// 클라이언트 소켓 정보를 저장할 구조체
	socklen_t clnt_addr_size;	// 클라이언트 소켓 정보를 저장할 구조체의 길이를 저장할 변수

	char message[]="Hello World!";

	clnt_addr_size=sizeof(clnt_addr);  
	//클라이언트의 소켓으로부터 데이터를 받아들인다.
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	// 데이터를 못읽어오면 error메세지
	if(clnt_sock==-1){
		error_handling("accept() error");  
	}

	write(clnt_sock, message, sizeof(message));
	int nbyte = 256;
    size_t filesize = 0, bufsize = 0;
    FILE *file = NULL;

	// 쓰기모드로 바이너리 파일 열기
    file = fopen("dog.jpg"/* 새로 만들 파일 이름 */, "wb");

    bufsize = 256;

    while(/*filesize != 0*/nbyte!=0) {
 		//if(filesize < 256) bufsize = filesize;
        nbyte = recv(clnt_sock, buf, bufsize, 0);
		// int recv(int s, void *buf, size_t len, int flags);
		// int s	: 소켓 디스크립터
		// void *buf	: 수신할 버퍼 포인터 데이터
		// size_t len	: 버퍼의 바이트 단위 크기
		// int flags	: 아래와 같은 옵션을 사용할 수 있습니다.


		//printf("filesize:%ld nbyte: %d\n", filesize, nbyte);

 		//filesize = filesize -nbyte;

        fwrite(buf, sizeof(char), nbyte, file);		
        //nbyte = 0;
    }
 

	// 파일과 소켓을 닫아준다
	fclose(file);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}