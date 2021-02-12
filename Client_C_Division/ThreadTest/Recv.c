void * recv_msg(void * arg)   // read thread main
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	int str_len;
	while(1)
	{
		str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
		if(str_len==-1) 
			return (void*)-1;
		name_msg[str_len]=0;
		fputs(name_msg, stdout);
	}
	return NULL;
}












/* server.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;	// 서버소켓
	int clnt_sock;	// 클라이언트 소셋
	char buf[256];	// 받을 메세지

	struct sockaddr_in serv_addr;	// 서버 소켓 정보를 저장할 구조체
	struct sockaddr_in clnt_addr;	// 클라이언트 소켓 정보를 저장할 구조체
	socklen_t clnt_addr_size;	// 클라이언트 소켓 정보를 저장할 구조체의 길이를 저장할 변수

	char message[]="Hello World!";
	
	// PORT번호 입력 안하면 error 메세지
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	// 소켓생성
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	// 생성 안될시 error 메세지
	if(serv_sock == -1){
		error_handling("socket() error");
	}
	// 주소정보 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));	//서버 소켓의 정보를 담을 구조체를 0으로 초기화 한다.
	serv_addr.sin_family=AF_INET;	//IPv4 를 사용
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);	//모든 아이피의 접근을 허용. 16진수로 변경해서 저장
	serv_addr.sin_port=htons(atoi(argv[1]));	 //프로그램 실행시 입력된 포트번호를 10진수로 변경해서 저장
	
	// 구조체의 정보를 이용해서 서버 소켓에 bind (주소정보 할당)
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 ){
		error_handling("bind() error"); // 할당 안될시 error
	}
	// 받아들일 준비를 한다. 인자에 5라는 숫자는 대기 클라이언트의 숫자가 최대 5개라는 의미이다.
	if(listen(serv_sock, 5)==-1){
		error_handling("listen() error");
	}

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
    file = fopen("aurora.jpg"/* 새로 만들 파일 이름 */, "wb");

    //ntohl(filesize);
	//recv filesize

	//recv(clnt_sock, &filesize, sizeof(filesize), 0);
    //ntohl(filesize);
	//printf("file size = [%ld]\n", filesize);
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
 
/*
	while((nbyte = recv(clnt_sock, buf, bufsize, 0) != 0)){
 		fwrite(buf, sizeof(char), nbyte, file);
	}
*/	

	// 파일과 소켓을 닫아준다
	fclose(file);
	close(clnt_sock);	
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}